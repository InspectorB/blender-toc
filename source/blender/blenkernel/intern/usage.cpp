/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Vincent Akkermans
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/blenkernel/intern/usage.cpp
 *  \ingroup bke
 */



/*
what is a good design for the sender?
 
 * should be non-blocking
 * should cache messages if connection is slow or down
 
Therefore, the Usage singleton should
 
 * keep a single connection
 * send messages in a separate thread
 * a threadsafe queue that can be pushed to and read from in different threads
 
*/

#ifdef WITH_USAGE

extern "C" {
#include "MEM_guardedalloc.h"
	
#include "DNA_userdef_types.h"
#include "DNA_scene_types.h"
#include "DNA_screen_types.h"
	
#include "RNA_types.h"
#include "RNA_access.h"
	
#include "WM_api.h"
#include "WM_types.h"

#include "BKE_context.h"
#include "BKE_image.h"
	
#include "BLI_string_utf8.h"
#include "BLI_string.h"
#include "BLI_path_util.h"
#include "BLI_fileops.h"
#include "BLI_rect.h"
	
#include "BIF_gl.h"
#include "BIF_glutil.h"
	
#include "IMB_imbuf.h"
#include "IMB_imbuf_types.h"
}

#include <string>
#include <fstream>
#include <streambuf>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "BKE_usage.h"

#include "usage/service_types.h"
#include "usage/data_types.h"
#include "usage/metadata_types.h"
#include "usage/message_types.h"

using namespace wire::data;
using namespace wire::metadata;

extern "C" {
	static void *usage_do_thread(void *callerdata)
	{
		usage::Usage::getInstance().doThread();
		return NULL;
	}
}

namespace usage {

	Usage::Usage()
	{
		enabled = false;
		shutdown = false;
		updateSettings();
		sendingMessage = NULL;
		sendingScreenshot = NULL;
		client = NULL;
		
		im_format.planes = R_IMF_PLANES_RGBA;
		im_format.imtype = R_IMF_IMTYPE_JP2;
		im_format.depth = R_IMF_CHAN_DEPTH_8;
		im_format.jp2_codec = R_IMF_JP2_CODEC_JP2;
		im_format.quality = 50;
		
		messageQueue = BLI_thread_queue_init();
		screenshotQueue = BLI_thread_queue_init();
		BLI_init_threads(&threads, &usage_do_thread, 1);
		BLI_insert_thread(&threads, NULL);
	}
	
	Usage::~Usage()
	{
		// N.B. the singleton is cleaned up explicitly on shutdown
		//free();
	}
	
	void Usage::free()
	{
		shutdown = true;
		BLI_end_threads(&threads);
		BLI_thread_queue_free(messageQueue);
		BLI_thread_queue_free(screenshotQueue);
		teardownConnection();
	}
	
	Usage& Usage::getInstance()
	{
		static Usage instance;
		return instance;
	}
	
	void Usage::updateSettings()
	{
		// Don't enable if the user chose to not enable collection or if the token hasn't
		// been set.
		enabled = U.flag & USER_USAGE_ENABLED && U.usage_service_token[0] != 0;
		updateSettingsP = true;
	}
	
	void Usage::createConnection()
	{
		// set up new connection
		boost::shared_ptr<TSocket> newSocket(new TSocket(U.usage_service_host, U.usage_service_port));
		boost::shared_ptr<TTransport> newTransport(new TFramedTransport(newSocket));
		boost::shared_ptr<TProtocol> newProtocol(new TBinaryProtocol(newTransport));
		newTransport->open();
		client = new TocServiceClient(newProtocol);
		// transfer ownership of pointers to the singleton
		socket = newSocket;
		transport = newTransport;
		protocol = newProtocol;
	}
	
	void Usage::teardownConnection()
	{
		if (transport.get() && transport->isOpen())
			transport->close();
	}
	
	void Usage::doThread()
	{
		
		while(!shutdown) {
			try {
				handleQueue();
			}
			catch (TTransportException) {
				updateSettings();
			}
		}
	}
	
	void Usage::handleQueue()
	{
		if (enabled) {
			// Check to see if settings have changed
			if (updateSettingsP) {
				teardownConnection();
				createConnection();
				updateSettingsP = false;
			}
				
			// Try and handle last message, or try and send the new message
			try {
				unsigned char sent = 0;
				
				if (sendingMessage == NULL)
					sendingMessage = (Message*)BLI_thread_queue_pop_timeout(messageQueue, 10);
				if (sendingScreenshot == NULL)
					sendingScreenshot = (ScreenshotQueueItem*)BLI_thread_queue_pop_timeout(screenshotQueue, 10);
				
				std::string token = U.usage_service_token;
				
				if (sendingMessage) {
					sendingMessage->__set_token(token); // set token here so it can be updated
					if (sendingMessage->data.__isset.wmOp) {
						printf("wmOp is set\n");
					}
					client->sendMessage(*sendingMessage);
					delete sendingMessage; // message has been sent, delete it
					sendingMessage = NULL;
					sent++;
				}
				
				if (sendingScreenshot) {
					ImBuf *ibufHalf;
					
					char filename[41];
					char filepath[128];
					BLI_snprintf(filename, sizeof(filename), "%s.jpg", sendingScreenshot->hash.c_str());
					BLI_make_file_string("/", filepath, BLI_temporary_dir(), filename);

					// half the image and write to disk
					ibufHalf = IMB_onehalf(sendingScreenshot->buf);
					BKE_imbuf_write(ibufHalf, filepath, &im_format);
				
					// load jpeg file into screenshot struct
					std::string content;
					read_entire_file(filepath, content);
					BLI_delete(filepath, FALSE, FALSE);

					std::string token = U.usage_service_token;
					Screenshot *sshot = new Screenshot();
					sshot->__set_hash(sendingScreenshot->hash);
					sshot->__set_token(token);
					sshot->__set_screenshot(content);
					sshot->__set_timestamp(sendingScreenshot->timestamp);
					
					client->sendScreenshot(*sshot);
					delete sshot;
					IMB_freeImBuf(sendingScreenshot->buf);
					delete sendingScreenshot;
					sendingScreenshot = NULL;
					IMB_freeImBuf(ibufHalf);
					sent++;
				}
				
				if (!sent) usleep(100000);
			}
			catch (UnknownToken e) {
				enabled = false;
				printf("TOKEN: %s\n", e.message.c_str());
			}
			catch (Unavailable) {
				printf("TODO: service unavailable\n");
			}
			catch (TApplicationException e) {
				switch (e.getType()) {
					case TApplicationException::INTERNAL_ERROR:
						printf("INTERNAL_ERROR: %s\n", e.what());
						break;
					case TApplicationException::PROTOCOL_ERROR:
						printf("PROTOCOL_ERROR: %s\n", e.what());
						break;
					default:
						printf("OTHER (%i): %s\n", e.getType(), e.what());
				}
				// If an error occurred, let's reset the connection
				updateSettings();
			}
			catch (TException e) {
				printf("Scary town... %s.\n", e.what());
				updateSettings();
			}
		}
	}
	
	void Usage::read_entire_file(const char *filepath, std::string &str)
	{
		std::ifstream in(filepath);
		
		in.seekg(0, std::ios::end);
		str.reserve(in.tellg());
		in.seekg(0, std::ios::beg);
		
		str.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	}

	// returns timestamp in milliseconds
	long Usage::getTimestamp()
	{
		long msec, sec;
#ifdef WIN32
		struct _timeb now;
		_ftime(&now);
		sec = now.time;
		msec = now.millitm;
#else
		struct timeval now;
		gettimeofday(&now, NULL);
		sec = now.tv_sec;
		msec = now.tv_usec / 1000;
#endif
		return (long)sec * 1000L + msec;
	}
	
	Message *Usage::getNewMessage()
	{
		Message *msg = new Message();
		msg->__set_timestamp(getTimestamp());
		return msg;
	}
	
	void Usage::queueOperator(bContext *C, wmOperator *op)
	{
		// TODO: perhaps filter out timer operations
		printf("we're in the c++ queue operator!\n");
		
		Message *msg = getNewMessage();
		WmOp thriftOp;
		
		PropertyRNA *iterprop = RNA_struct_iterator_property(op->ptr->type);
		PropertyRNA *prop;
		int i;
		const char *arg_name;
		char *cstring = NULL;
		
		std::vector<RNAProperty> thriftOpProperties;
		
		if (!(op->type->flag & OPTYPE_NOSCREENSHOT)) {
			// prepare uuid
			boost::uuids::uuid uuid = uuidGenerator();
			const std::string uuidStr = boost::lexical_cast<std::string>(uuid);

			ImBuf *ibuf;
			ScreenshotQueueItem *sqi;
					
			// take screenshot
			ibuf = take_screenshot(C, 0);
			sqi = new ScreenshotQueueItem;
			sqi->buf = ibuf;
			sqi->hash = uuidStr;
			sqi->timestamp = msg->timestamp;
			BLI_thread_queue_push(screenshotQueue, sqi);
			thriftOp.__set_screenshotHash(uuidStr);
		}
		
		// create op
		thriftOp.__set_operatorId(std::string(op->idname));
		cstring = WM_operator_pystring(C, op, true, true);
		thriftOp.__set_pythonRepresentation(std::string(cstring ? cstring : ""));
		MEM_freeN(cstring);
		cstring = NULL;
		
		RNA_PROP_BEGIN (op->ptr, propptr, iterprop)
		{
			int type, len;
			RNAProperty thriftProp;
			RNAPropertyData thriftPropData;
			bool setDataP = true;
			
			prop = (PropertyRNA*)propptr.data;
			arg_name = RNA_property_identifier(prop);
			
			type = RNA_property_type(prop);
			len = RNA_property_array_length(op->ptr, prop);
			
			thriftProp.__set_identifier(std::string(arg_name));
			thriftProp.__set_type(type);
			thriftProp.__set_length(len);
			
			switch (type) {
				case PROP_BOOLEAN:
					if (len == 0)
						thriftPropData.__set_valueBoolean(RNA_property_boolean_get(op->ptr, prop));
					else {
						std::vector<bool> l;
						for (i = 0; i < len; i++)
							l.push_back(RNA_property_boolean_get_index(op->ptr, prop, i));
						thriftPropData.__set_listBoolean(l);
					}
					break;
				case PROP_INT:
					if (len == 0)
						thriftPropData.__set_valueInt(RNA_property_int_get(op->ptr, prop));
					else {
						std::vector<int> l;
						for (i = 0; i < len; i++)
							l.push_back(RNA_property_int_get_index(op->ptr, prop, i));
						thriftPropData.__set_listInt(l);
					}
					break;
				case PROP_FLOAT:
					if (len == 0)
						thriftPropData.__set_valueDouble(RNA_property_float_get(op->ptr, prop));
					else {
						std::vector<double> l;
						for (i = 0; i < len; i++)
							l.push_back(RNA_property_float_get_index(op->ptr, prop, i));
						thriftPropData.__set_listDouble(l);
					}
					break;
				case PROP_STRING:
				{
					cstring = RNA_property_string_get_alloc(op->ptr, prop, NULL, 0, NULL);
					thriftPropData.__set_valueString(std::string(cstring ? cstring : ""));
					if (cstring != NULL) {
						MEM_freeN(cstring);
						cstring = NULL;
					}
					break;
				}
				case PROP_ENUM:
				{
					const char *identifier;
					int val = RNA_property_enum_get(op->ptr, prop);
					
					// a set of enums
					if (RNA_property_flag(prop) & PROP_ENUM_FLAG) {
						
						std::set<std::string> s;
						
						if (val) {
							EnumPropertyItem *item = NULL;
							bool free;
							
							// start enum items
							RNA_property_enum_items(C, op->ptr, prop, &item, NULL, &free);
							if (item) {
								for (; item->identifier; item++) {
									if (item->identifier[0] && item->value & val) {
										s.insert(item->identifier);
									}
								}
								
								if (free)
									MEM_freeN(item);
							}
						}
						
						thriftPropData.__set_listEnum(s);
					}
					else if (RNA_property_enum_identifier(C, op->ptr, prop, val, &identifier)) {
						thriftPropData.__set_valueEnum(std::string(identifier));
					}
					else {
						// Unknown enum, explicitly set the length field to -1
						setDataP = false;
						thriftProp.__set_length(-1);
					}
					break;
				}
				case PROP_POINTER:
				{
					// TODO: the pointer can be one of several things...
					PointerRNA tptr = RNA_property_pointer_get(op->ptr, prop);
					cstring = RNA_pointer_as_string(C, op->ptr, prop, &tptr);
					thriftPropData.__set_valuePointer(std::string(cstring ? cstring : ""));
					if (cstring != NULL) {
						MEM_freeN(cstring);
						cstring = NULL;
					}
					break;
				}
				case PROP_COLLECTION:
				{
					std::vector<std::string> c;
					
					CollectionPropertyIterator collect_iter;
					
					i = 0;

					for (RNA_property_collection_begin(op->ptr, prop, &collect_iter);
						 (i < INT_MAX) && collect_iter.valid;
						 RNA_property_collection_next(&collect_iter), i++)
					{
						PointerRNA itemptr = collect_iter.ptr;
						
						// now get every prop of the collection
						// TODO: the pointer can be one of several things...
						cstring = RNA_pointer_as_string(C, op->ptr, prop, &itemptr);
						c.push_back(std::string(cstring ? cstring : NULL));
						if (cstring != NULL) {
							MEM_freeN(cstring);
							cstring = NULL;
						}
					}
					
					RNA_property_collection_end(&collect_iter);
					
					thriftPropData.__set_collection(c);
					
					break;
				}
				default:
					thriftProp.__set_type(-1);
					break;
			}
			
			// Don't set the property's data if we've marked it as such
			// This happens for example with unknown enums
			if (setDataP)
				thriftProp.__set_data(thriftPropData);
			thriftOpProperties.push_back(thriftProp);
		}
		RNA_PROP_END;
		
		thriftOp.__set_properties(thriftOpProperties);

		Metadata metadata;
		NoMetadata noMetadata;
		metadata.__set_noMetadata(noMetadata);
		msg->__set_metadata(metadata);

		Data data;
		data.__set_wmOp(thriftOp);
		msg->__set_data(data);

		printf("we're queueing the wmop\n");
		BLI_thread_queue_push(messageQueue, msg);
	}
	
	void Usage::queueEvent(bContext *C, const wmEvent *event)
	{
		// TODO: might want to filter out simple mouse movements
		
		Message *msg = getNewMessage();
		WmEv ev;
		
		ev.__set_type(event->type);
		ev.__set_value(event->val);
		ev.__set_x(event->x);
		ev.__set_y(event->y);
		ev.__set_mval1(event->mval[0]);
		ev.__set_mval2(event->mval[1]);


		char str[BLI_UTF8_MAX + 1];
		size_t len;
		if (event->utf8_buf[0]) {
			len = BLI_str_utf8_size_safe(event->utf8_buf);
			memcpy(str, event->utf8_buf, len);
		} else str[0] = event->ascii;
		ev.__set_character(str);
		
		ev.__set_prevtype(event->prevtype);
		ev.__set_prevval(event->prevval);
		ev.__set_prevx(event->prevx);
		ev.__set_prevy(event->prevy);
		ev.__set_prevclicktime(event->prevclicktime);
		ev.__set_prevclickx(event->prevclickx);
		ev.__set_prevclicky(event->prevclicky);
		ev.__set_shift(event->shift);
		ev.__set_ctrl(event->ctrl);
		ev.__set_alt(event->alt);
		ev.__set_oskey(event->oskey);
		ev.__set_keymodifier(event->keymodifier);
		ev.__set_check_click(event->check_click);
		if (event->keymap_idname) ev.__set_keymap_idname(event->keymap_idname);
		
		// TODO: set tablet data
		
		Metadata metadata;
		NoMetadata noMetadata;
		metadata.__set_noMetadata(noMetadata);
		msg->__set_metadata(metadata);
		
		Data data;
		data.__set_wmEv(ev);
		msg->__set_data(data);
		
		BLI_thread_queue_push(messageQueue, msg);
	}
	
	ImBuf* Usage::take_screenshot(bContext *C, const bool crop)
	{
		ScrArea *sa;
		rcti cropRect;
		unsigned int *dumprect = NULL;
		int dumpsx, dumpsy;
		
		// copied from screenshot
		wmWindow *win = CTX_wm_window(C);
		int x = 0, y = 0;
		
		WM_redraw_windows(C);
		
		dumpsx = WM_window_pixels_x(win);
		dumpsy = WM_window_pixels_y(win);
		
		if (dumpsx && dumpsy) {
			unsigned char *dumprectC;
			dumprect = (unsigned int*)MEM_mallocN(sizeof(int) * (dumpsx) * (dumpsy), "dumprect");
			dumprectC = (unsigned char*)dumprect;
			glReadBuffer(GL_FRONT);
			// copied from screenshot_read_pixels
			glReadPixels(x, y, dumpsx, dumpsy, GL_RGBA, GL_UNSIGNED_BYTE, dumprectC);
			glFinish();
			int i;
			for (i = 0, dumprectC += 3; i < dumpsx * dumpsy; i++, dumprectC += 4)
				*dumprectC = 255;
			glReadBuffer(GL_BACK);
		}
		
		sa = CTX_wm_area(C);
		if (sa) cropRect = sa->totrct;
		
		
		if (dumprect) {
			ImBuf *ibuf; //, *ibufHalf;
			
			/* operator ensures the extension */
			ibuf = IMB_allocImBuf(dumpsx, dumpsy, 24, 0);
			ibuf->rect = dumprect;
			
			/* crop to show only single editor */
			// copied from screenshot_crop
			if (crop) {
				unsigned int *to = ibuf->rect;
				unsigned int *from = ibuf->rect + cropRect.ymin * ibuf->x + cropRect.xmin;
				int crop_x = BLI_rcti_size_x(&cropRect);
				int crop_y = BLI_rcti_size_y(&cropRect);
				int y;
				
				if (crop_x > 0 && crop_y > 0) {
					for (y = 0; y < crop_y; y++, to += crop_x, from += ibuf->x)
						memmove(to, from, sizeof(unsigned int) * crop_x);
					
					ibuf->x = crop_x;
					ibuf->y = crop_y;
				}
			}
			if (dumprect) MEM_freeN(dumprect);
			
			return ibuf;
		}
		
		return NULL;
	}
	
} /* namespace */

/* C interface */
extern "C" {
	
	void BKE_usage_queue_operator(bContext *C, wmOperator *op)
	{
		// don't register internal operators
		if (op->type && (op->type->flag & OPTYPE_INTERNAL))
			return;
		usage::Usage::getInstance().queueOperator(C, op);
	}
	
	void BKE_usage_queue_event(bContext *C, const wmEvent *ev)
	{
		usage::Usage::getInstance().queueEvent(C, ev);
	}
	
	void BKE_usage_update_settings(void)
	{
		usage::Usage::getInstance().updateSettings();
	}
	
	void BKE_usage_shutdown()
	{
		usage::Usage::getInstance().free();
	}
}

#endif /* WITH_USAGE */