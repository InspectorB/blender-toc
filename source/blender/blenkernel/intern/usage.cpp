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

#include "RNA_types.h"
#include "RNA_access.h"
	
#include "WM_api.h"
#include "WM_types.h"

#include "BLI_string_utf8.h"
}

#include "BKE_usage.h"

#include "usage/service_types.h"
#include "usage/data_types.h"
#include "usage/metadata_types.h"

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
		client = NULL;
		queue = BLI_thread_queue_init();
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
		BLI_thread_queue_free(queue);
		teardown();
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
	
	void Usage::teardown()
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
				teardown();
				createConnection();
				updateSettingsP = false;
			}
				
			// Try and handle last message, or try and send the new message
			try {
				if (sendingMessage == NULL) {
					sendingMessage = (Message*)BLI_thread_queue_pop_timeout(queue, 10);
					if (sendingMessage == NULL)
						return;
				}
								
				// ping for now
				//client->ping();
				client->sendMessage(*sendingMessage);
				
				// message has been sent, delete it
				delete sendingMessage;
				sendingMessage = NULL;
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
		std::string token = U.usage_service_token;
		msg->__set_token(token);
		return msg;
	}
	
	void Usage::queueOperator(bContext *C, wmOperator *op)
	{
		// TODO: perhaps filter out timer operations
		
		Message *msg = getNewMessage();
		WmOp thriftOp;
		
		PropertyRNA *iterprop = RNA_struct_iterator_property(op->ptr->type);
		PropertyRNA *prop;
		int i;
		const char *arg_name;
		char *cstring = NULL;
		
		std::vector<RNAProperty> thriftOpProperties;
				
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

		BLI_thread_queue_push(queue, msg);
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
		
		BLI_thread_queue_push(queue, msg);
	}
	
} /* namespace */

/* C interface */
extern "C" {
	
	void BKE_usage_queue_operator(bContext *C, wmOperator *op)
	{
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