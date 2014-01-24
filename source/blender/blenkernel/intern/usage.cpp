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
#include "DNA_object_types.h"
#include "DNA_view3d_types.h"
	
#include "RNA_types.h"
#include "RNA_access.h"
//#include "RNA_blender.h"
	
#include "WM_api.h"
#include "WM_types.h"

#include "BKE_context.h"
#include "BKE_image.h"
#include "BKE_global.h"
#include "BKE_blender.h"
	
#include "BLI_string_utf8.h"
#include "BLI_string.h"
#include "BLI_path_util.h"
#include "BLI_fileops.h"
#include "BLI_rect.h"
#include "BLI_listbase.h"
	
#include "BIF_gl.h"
#include "BIF_glutil.h"
	
#include "IMB_imbuf.h"
#include "IMB_imbuf_types.h"
	
#include "UI_interface.h"
	
#include "wm_window.h"
//#include "GHOST_C-api.h"
}

#include "BKE_usage.h"

#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "usage/service_types.h"
#include "usage/data_types.h"
#include "usage/metadata_types.h"
#include "usage/message_types.h"

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <CoreServices/CoreServices.h>
#endif
#ifdef __linux__
#include <sys/utsname.h>
#endif

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
		
		sessionKey = generateUUID();
		
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
		// Give the queue some time to send last messages
		if (enabled && !emptyQueues() && transport->isOpen()) {
			long startTS = getTimestamp();
			long currentTS = startTS;
			while (currentTS - startTS < 60000 && !emptyQueues()) { // 1 minute
				usleep(1000000); // 1 second
				currentTS = getTimestamp();
			}
		}
		
		// Shut down
		shutdown = true;
		BLI_end_threads(&threads);
		
		while (BLI_thread_queue_size(messageQueue) > 0) {
			wire::Message *msg = (wire::Message*)BLI_thread_queue_pop(messageQueue);
			delete msg;
		}
		
		while (BLI_thread_queue_size(screenshotQueue) > 0) {
			ScreenshotQueueItem *sqi = (ScreenshotQueueItem*)BLI_thread_queue_pop(screenshotQueue);
			IMB_freeImBuf(sqi->buf);
			delete sqi;
		}
		
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
	
	bool Usage::createConnection()
	{
		try {
			// set up new connection
			boost::shared_ptr<TSocket> newSocket(new TSocket(U.usage_service_host, U.usage_service_port));
			boost::shared_ptr<TTransport> newTransport(new TFramedTransport(newSocket));
			boost::shared_ptr<TProtocol> newProtocol(new TBinaryProtocol(newTransport));
			newTransport->open();
			if (!newTransport->isOpen()) throw TException("Can't open transport");
			client = new wire::TocServiceClient(newProtocol);
			// transfer ownership of pointers to the singleton
			socket = newSocket;
			transport = newTransport;
			protocol = newProtocol;
			return true;
		}
		catch (TException e) {
			printf("%s\n", e.what());
			updateSettings();
			usleep(1000000);
			return false;
		}
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
	
	bool Usage::emptyQueues()
	{
		return (BLI_thread_queue_size(messageQueue) == 0
				&&
				BLI_thread_queue_size(screenshotQueue) == 0);
	}
	
	void Usage::handleQueue()
	{
		if (enabled) {
			// Check to see if settings have changed
			if (updateSettingsP) {
				teardownConnection();
				if (!createConnection())
					return;
				updateSettingsP = false;
			}
				
			// Try and handle last message, or try and send the new message
			try {
				unsigned char sent = 0;
				
				if (sendingMessage == NULL)
					sendingMessage = (wire::Message*)BLI_thread_queue_pop_timeout(messageQueue, 10);
				if (sendingScreenshot == NULL)
					sendingScreenshot = (ScreenshotQueueItem*)BLI_thread_queue_pop_timeout(screenshotQueue, 10);
				
				std::string token = U.usage_service_token;
				
				if (sendingMessage) {
					sendingMessage->__set_token(token); // set token here so it can be updated
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
					wire::Screenshot sshot;
					sshot.__set_hash(sendingScreenshot->hash);
					sshot.__set_token(token);
					sshot.__set_screenshot(content);
					sshot.__set_timestamp(sendingScreenshot->timestamp);
					
					client->sendScreenshot(sshot);
					
					IMB_freeImBuf(sendingScreenshot->buf);
					delete sendingScreenshot;
					sendingScreenshot = NULL;
					IMB_freeImBuf(ibufHalf);
					sent++;
				}
				
				if (!sent) usleep(100000);
			}
			catch (wire::UnknownToken e) {
				enabled = false;
				printf("TOKEN: %s\n", e.message.c_str());
			}
			catch (wire::Unavailable) {
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
		} else {
			updateSettings();
			usleep(1000000);
		}
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
			
			if (dumprect) {
				sa = CTX_wm_area(C);
				
				ImBuf *ibuf; //, *ibufHalf;
				
				/* operator ensures the extension */
				ibuf = IMB_allocImBuf(dumpsx, dumpsy, 24, 0);
				ibuf->rect = dumprect;
				
				/* crop to show only single editor */
				// copied from screenshot_crop
				if (sa) {
					cropRect = sa->totrct;
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
				}
				
				MEM_freeN(dumprect);
				return ibuf;
			}
		}
		
		return NULL;
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
	
	wire::Message *Usage::getNewMessage()
	{
		wire::Message *msg = new wire::Message();
		msg->__set_timestamp(getTimestamp());
		msg->__set_sessionKey(sessionKey);
		return msg;
	}
	
	std::string Usage::p2s(void *p)
	{
		char addressBuffer[32];
		sprintf(addressBuffer, "%p", p);
		std::string address = addressBuffer;
		return address;
	}
	
	wire::data::Context *Usage::getNewContext(const bContext *C)
	{
		wmWindow *win = CTX_wm_window(C);
		bScreen *bs = CTX_wm_screen(C);
		ScrArea *sa = CTX_wm_area(C);
		ARegion *ar = CTX_wm_region(C);

//		Main *main = CTX_data_main(C);
		Scene *scene = CTX_data_scene(C);
		
		RegionView3D *rvd = CTX_wm_region_view3d(C);
		
		wire::data::Context *ctx = new wire::data::Context();
		
		// set window related information
		if (win) {
			std::string name = win->screenname;
			ctx->__set_windowName(name);
			ctx->__set_windowAddress(p2s(win));
		}
		
		if (bs) {
			std::string name = bs->id.name;
			ctx->__set_screenName(name);
			ctx->__set_screenAddress(p2s(bs));
		}
		
		if (sa) {
			ctx->__set_spaceType(sa->spacetype);
			ctx->__set_spaceAddress(p2s(sa));
		}
		
		if (ar) {
			ctx->__set_regionType(ar->regiontype);
			ctx->__set_regionAddress(p2s(ar));
		}
		
		if (scene) {
			ctx->__set_sceneName(scene->id.name);
			ctx->__set_sceneAddress(p2s(scene));
		}
		
		// set view related information
		if (rvd) {
			wire::data::ViewOrientation vo;
			
			std::vector<double> ofs;
			ofs.push_back(rvd->ofs[0]);
			ofs.push_back(rvd->ofs[1]);
			ofs.push_back(rvd->ofs[2]);
			vo.__set_offset(ofs);
			
			vo.__set_distance(rvd->dist);

			std::vector<double> viewquat;
			viewquat.push_back(rvd->viewquat[0]);
			viewquat.push_back(rvd->viewquat[1]);
			viewquat.push_back(rvd->viewquat[2]);
			viewquat.push_back(rvd->viewquat[3]);
			vo.__set_viewquat(viewquat);

			vo.__set_camzoom(rvd->camzoom);
			vo.__set_camdx(rvd->camdx);
			vo.__set_camdy(rvd->camdy);
			
			vo.__set_is_persp(rvd->is_persp);
			vo.__set_persp(rvd->persp);					// persp: 0 for ortho, 1 for persp, 2 for camera
			vo.__set_view(rvd->view);					// view: RV3D_VIEW_*
			vo.__set_is_local(rvd->localvd != NULL);
			
			ctx->__set_viewOrientation(vo);
		}
		
		// set object related information
		Base *active = CTX_data_active_base(C);
		std::vector<wire::data::Object> objs;
		
		CTX_DATA_BEGIN (C, Base *, base, visible_bases)
		{
			if (base->object) {
				wire::data::Object obj;
				obj.__set_selected(base->flag & SELECT);
				obj.__set_active(base == active);
				std::string name = base->object->id.name;
				obj.__set_name(name);
				obj.__set_type(base->object->type);
				obj.__set_baseAddress(p2s(base));
				obj.__set_objectAddress(p2s(base->object));
				obj.__set_parentType(base->object->partype);
				obj.__set_parentAddress(p2s(base->object->parent));
				objs.push_back(obj);
			}
		}
		CTX_DATA_END;

		ctx->__set_visibleObjects(objs);
		
		std::string mode = CTX_data_mode_string(C);
		ctx->__set_dataMode(mode);
		
		return ctx;
	}
	
	void Usage::setProperty(wire::data::RNAProperty *thriftProp, bContext *C, PointerRNA* ptr, PropertyRNA* prop)
	{
		int type, subtype, len, i = 0;
		wire::data::RNAPropertyData thriftPropData;
		bool setDataP = true;
		char *cstr = NULL;
		
		const char* arg_name = RNA_property_identifier(prop);
		
		type = RNA_property_type(prop);
		subtype = RNA_property_subtype(prop);
		len = RNA_property_array_length(ptr, prop);
		
		thriftProp->__set_identifier(std::string(arg_name));
		thriftProp->__set_type(type);
		thriftProp->__set_subtype(subtype);
		thriftProp->__set_length(len);
		
		switch (type) {
			case PROP_BOOLEAN:
				if (len == 0)
					thriftPropData.__set_valueBoolean(RNA_property_boolean_get(ptr, prop));
				else {
					std::vector<bool> l;
					for (i = 0; i < len; i++)
						l.push_back(RNA_property_boolean_get_index(ptr, prop, i));
					thriftPropData.__set_listBoolean(l);
				}
				break;
			case PROP_INT:
				if (len == 0)
					thriftPropData.__set_valueInt(RNA_property_int_get(ptr, prop));
				else {
					std::vector<int> l;
					for (i = 0; i < len; i++)
						l.push_back(RNA_property_int_get_index(ptr, prop, i));
					thriftPropData.__set_listInt(l);
				}
				break;
			case PROP_FLOAT:
				if (len == 0)
					thriftPropData.__set_valueDouble(RNA_property_float_get(ptr, prop));
				else {
					std::vector<double> l;
					for (i = 0; i < len; i++)
						l.push_back(RNA_property_float_get_index(ptr, prop, i));
					thriftPropData.__set_listDouble(l);
				}
				break;
			case PROP_STRING:
			{
				cstr = RNA_property_string_get_alloc(ptr, prop, NULL, 0, NULL);
				thriftPropData.__set_valueString(std::string(cstr ? cstr : ""));
				if (cstr != NULL) {
					MEM_freeN(cstr);
					cstr = NULL;
				}
				break;
			}
			case PROP_ENUM:
			{
				const char *identifier;
				int val = RNA_property_enum_get(ptr, prop);
				
				// a set of enums
				if (RNA_property_flag(prop) & PROP_ENUM_FLAG) {
					
					std::set<std::string> s;
					
					if (val) {
						EnumPropertyItem *item = NULL;
						bool free;
						
						// start enum items
						RNA_property_enum_items(C, ptr, prop, &item, NULL, &free);
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
				else if (RNA_property_enum_identifier(C, ptr, prop, val, &identifier)) {
					thriftPropData.__set_valueEnum(std::string(identifier));
				}
				else {
					// Unknown enum, explicitly set the length field to -1
					setDataP = false;
					thriftProp->__set_length(-1);
				}
				break;
			}
			case PROP_POINTER:
			{
				// TODO: the pointer can be one of several things...
				PointerRNA tptr = RNA_property_pointer_get(ptr, prop);
				cstr = RNA_pointer_as_string(C, ptr, prop, &tptr);
				thriftPropData.__set_valuePointer(std::string(cstr ? cstr : ""));
				if (cstr != NULL) {
					MEM_freeN(cstr);
					cstr = NULL;
				}
				break;
			}
			case PROP_COLLECTION:
			{
				std::vector<std::string> c;
				
				CollectionPropertyIterator collect_iter;
				
				i = 0;
				
				for (RNA_property_collection_begin(ptr, prop, &collect_iter);
					 (i < INT_MAX) && collect_iter.valid;
					 RNA_property_collection_next(&collect_iter), i++)
				{
					PointerRNA itemptr = collect_iter.ptr;
					
					// now get every prop of the collection
					// TODO: the pointer can be one of several things...
					cstr = RNA_pointer_as_string(C, ptr, prop, &itemptr);
					c.push_back(std::string(cstr ? cstr : NULL));
					if (cstr != NULL) {
						MEM_freeN(cstr);
						cstr = NULL;
					}
				}
				
				RNA_property_collection_end(&collect_iter);
				
				thriftPropData.__set_collection(c);
				
				break;
			}
			default:
				thriftProp->__set_type(-1);
				break;
		}
		
		// Don't set the property's data if we've marked it as such
		// This happens for example with unknown enums
		if (setDataP)
			thriftProp->__set_data(thriftPropData);
	}
	
	std::string Usage::generateUUID()
	{
		// prepare uuid
		boost::uuids::uuid uuid = uuidGenerator();
		return boost::lexical_cast<std::string>(uuid);
	}
	
	void Usage::queueOperator(bContext *C, wmOperator *op, int retval, int repeat)
	{
		// TODO: perhaps filter out timer operations
		if (enabled) {
			
			wire::Message *msg = getNewMessage();
			wire::data::WmOp thriftOp;
			
			char *cstr = NULL;
			PropertyRNA *iterprop = RNA_struct_iterator_property(op->ptr->type);
			std::vector<wire::data::RNAProperty> thriftOpProperties;
			
			// check if there's a window, otherwise opening a file crashes
			if (!(op->type->flag & OPTYPE_NOSCREENSHOT) && CTX_wm_window(C)) {
				
				std::string uuidStr = generateUUID();
				
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
			cstr = WM_operator_pystring(C, op, true, true);
			thriftOp.__set_pythonRepresentation(std::string(cstr ? cstr : ""));
			MEM_freeN(cstr);
			cstr = NULL;
			
			thriftOp.__set_repeat(repeat);
			thriftOp.__set_retval(retval);
			
			// set reports
			if (op->reports) {
				std::vector<wire::data::Report> reports;
				Report *report = NULL; // N.B. blender Report, not thrift Report
				for (report = (Report*)op->reports->list.first; report; report = report->next) {
					wire::data::Report r;
					r.__set_type(report->type);
					r.__set_flag(report->flag);
					std::string typestr = report->typestr;
					r.__set_typestr(typestr);
					std::string message = report->message;
					r.__set_message(message);
					reports.push_back(r);
				}
				if (!reports.empty())
					thriftOp.__set_reports(reports);
			}
			
			// set properties
			RNA_PROP_BEGIN (op->ptr, propptr, iterprop)
			{
				wire::data::RNAProperty thriftProp;
				setProperty(&thriftProp, C, op->ptr, (PropertyRNA*)propptr.data);
				thriftOpProperties.push_back(thriftProp);
			}
			RNA_PROP_END;
			
			thriftOp.__set_properties(thriftOpProperties);
			
			// set the context
			wire::data::Context *ctx = getNewContext(C);
			thriftOp.__set_context(*ctx);
			delete ctx;
			
			// set enveloping message
			wire::metadata::Metadata metadata;
			wire::metadata::NoMetadata noMetadata;
			metadata.__set_noMetadata(noMetadata);
			msg->__set_metadata(metadata);

			wire::data::Data data;
			data.__set_wmOp(thriftOp);
			msg->__set_data(data);

			BLI_thread_queue_push(messageQueue, msg);
		}
	}
	
	void Usage::queueEvent(bContext *C, const wmEvent *event)
	{
		// TODO: might want to filter out simple mouse movements
		if (enabled) {
		
			wire::Message *msg = getNewMessage();
			wire::data::WmEv ev;
			
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
			
			wire::metadata::Metadata metadata;
			wire::metadata::NoMetadata noMetadata;
			metadata.__set_noMetadata(noMetadata);
			msg->__set_metadata(metadata);
			
			wire::data::Data data;
			data.__set_wmEv(ev);
			msg->__set_data(data);
			
			BLI_thread_queue_push(messageQueue, msg);
		}
	}
	
	void Usage::queueButtonPress(bContext *C, uiBut *but)
	{
		if (enabled) {
			// just send an empty ButtonPress
			wire::Message *msg = getNewMessage();
			wire::data::ButPress bp;
			
			wire::metadata::Metadata metadata;
			wire::metadata::NoMetadata noMetadata;
			metadata.__set_noMetadata(noMetadata);
			msg->__set_metadata(metadata);
			
			wire::data::Data data;
			data.__set_butPress(bp);
			msg->__set_data(data);
			
			BLI_thread_queue_push(messageQueue, msg);
		}
	}
	
	void Usage::queueAssignment(bContext *C, PointerRNA *ptr, PropertyRNA *prop, int index)
	{
		if (enabled) {
			wire::Message *msg = getNewMessage();
			wire::data::Assignment as;
			
			// set python reperesentation
			char *buf;
			
			buf = WM_prop_pystring_assign(C, ptr, prop, index);
			if (buf) {
				std::string bufS = buf;
				as.__set_pythonRepresentation(bufS);
				MEM_freeN(buf);
			}
			
			wire::data::RNAProperty thriftProp;
			setProperty(&thriftProp, C, ptr, prop);
			as.__set_property(thriftProp);
			
			wire::metadata::Metadata metadata;
			wire::metadata::NoMetadata noMetadata;
			metadata.__set_noMetadata(noMetadata);
			msg->__set_metadata(metadata);
			
			wire::data::Data data;
			data.__set_assignment(as);
			msg->__set_data(data);
			
			BLI_thread_queue_push(messageQueue, msg);
		}
	}
	
	void Usage::queueStart()
	{
		if (enabled) {
			wire::Message *msg = getNewMessage();
			wire::data::SessionStart ss;
			
			ss.__set_gui(!G.background);
			ss.__set_sessionKey(sessionKey);
			
			/* Set OS version information */
#ifdef _WIN32 // also defined if win64
			ss.__set_os("Windows");
			{
				std::stringstream ver;
				OSVERSIONINFO vi;
				memset (&vi, 0, sizeof(vi));
				vi.dwOSVersionInfoSize = sizeof(vi);
				GetVersionEx(&vi);
				
				ver	<< vi.dwPlatformId		<< "."
					<< vi.dwMajorVersion	<< "."
					<< vi.dwMinorVersion	<< "."
					<< vi.dwBuildNumber		<< "."
					<< " (" << vi.szCSDVersion << ")";
				ss.__set_osVersion(ver.str());
			}
#endif
#ifdef __APPLE__
			ss.__set_os("MacOSX");
			{
				std::stringstream ver;
				SInt32 majorVersion, minorVersion, bugFixVersion;
				Gestalt(gestaltSystemVersionMajor, &majorVersion);
				Gestalt(gestaltSystemVersionMinor, &minorVersion);
				Gestalt(gestaltSystemVersionBugFix, &bugFixVersion);
				
				ver << majorVersion << "." << minorVersion << "." << bugFixVersion;
				ss.__set_osVersion(ver.str());
			}
#endif
#ifdef __linux__
			ss.__set_os("Linux");
			{
				struct utsname unameData;
				if (!uname(&unameData)) {
					std::stringstream ver;
					ver << unameData.sysname << " - "
						<< unameData.release << " - "
						<< unameData.version;
					ss.__set_osVersion(ver.str());
				} else {
					ss.__set_osVersion("unknown");
				}
			}
#endif
#ifdef __NetBSD__
			ss.__set_os("NetBSD");
			ss.__set_osVersion("unknown");
#endif
#ifdef __OpenBSD__
			ss.__set_os("OpenBSD");
			ss.__set_osVersion("unknown");
#endif
#ifdef __FreeBSD__
			ss.__set_os("FreeBSD");
			ss.__set_osVersion("unknown");
#endif
			
			/* Set Blender version information */
			ss.__set_blenderVersion(BLENDER_VERSION);
			ss.__set_blenderSubversion(BLENDER_SUBVERSION);
			
			
			/* Set display information */
			if (!G.background) {
				int dispwidth;
				int dispheight;

				ss.__set_numDisplays(wm_get_num_displays());
				
				wm_get_desktopsize(&dispwidth, &dispheight);
				ss.__set_resolutionX(dispwidth);
				ss.__set_resolutionY(dispheight);
			}
			
			wire::metadata::Metadata metadata;
			wire::metadata::NoMetadata noMetadata;
			metadata.__set_noMetadata(noMetadata);
			msg->__set_metadata(metadata);
			
			wire::data::Data data;
			data.__set_sessionStart(ss);
			msg->__set_data(data);
			
			BLI_thread_queue_push(messageQueue, msg);
		}
	}
	
	void Usage::queueEnd()
	{
		if (enabled) {
			wire::Message *msg = getNewMessage();
			wire::data::SessionEnd se;
			
			se.__set_sessionKey(sessionKey);
			
			wire::metadata::Metadata metadata;
			wire::metadata::NoMetadata noMetadata;
			metadata.__set_noMetadata(noMetadata);
			msg->__set_metadata(metadata);
			
			wire::data::Data data;
			data.__set_sessionEnd(se);
			msg->__set_data(data);
			
			BLI_thread_queue_push(messageQueue, msg);
		}
	}
	
} /* namespace */

/* C interface */
extern "C" {
	
	void BKE_usage_queue_operator(bContext *C, wmOperator *op, int retval, int repeat)
	{
		// don't register internal operators
		if (op->type && (op->type->flag & OPTYPE_INTERNAL))
			return;
		usage::Usage::getInstance().queueOperator(C, op, retval, repeat);
	}
	
	void BKE_usage_queue_event(bContext *C, const wmEvent *ev)
	{
		usage::Usage::getInstance().queueEvent(C, ev);
	}
	
	void BKE_usage_queue_button(struct bContext *C, struct uiBut *but)
	{
		usage::Usage::getInstance().queueButtonPress(C, but);
	}
	
	void BKE_usage_queue_assignment(struct bContext *C, PointerRNA *ptr, PropertyRNA *prop, int index)
	{
		usage::Usage::getInstance().queueAssignment(C, ptr, prop, index);
	}
	
	void BKE_usage_queue_start(void)
	{
		usage::Usage::getInstance().queueStart();
	}
	
	void BKE_usage_update_settings(void)
	{
		usage::Usage::getInstance().updateSettings();
	}
	
	void BKE_usage_shutdown(void)
	{
		usage::Usage::getInstance().queueEnd();
		usage::Usage::getInstance().free();
	}
}

#endif /* WITH_USAGE */