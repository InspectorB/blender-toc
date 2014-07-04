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
 * Contributor(s): Vincent Akkermans
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file BKE_usage.h
 *  \ingroup bke
 */


#ifndef __BKE_USAGE_H__
#define __BKE_USAGE_H__

#ifdef WITH_USAGE

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
#include "BLI_threads.h"
#include "DNA_windowmanager_types.h"
#include "UI_interface.h"
	
#ifdef __cplusplus
}
#endif //__cplusplus

#ifdef __cplusplus

#include <queue>

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/thread.hpp>

#include "intern/usage/data_types.h"

#include "intern/usage/TocService.h"


/*
 * only take a screenshot once per second
 */
#define ONE_SCREENSHOT_PER_MS 1000

/*
 * define maximum queue sizes so that things don't spin out of control when the
 * internet connection goes down
 */
#define MAX_SCREENSHOTS_IN_QUEUE 100
#define MAX_MESSAGES_IN_QUEUE 100000 // max size that can be reported, not the max size of the queue

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

/* 
 * Used to put image screenshots in the queue with the right hash
 * scaling and conversion is then done in the sending thread
 */
typedef struct ScreenshotQueueItem {
	ImBuf *buf;
	std::string hash;
	long timestamp;
	short subdivisions;
} ScreenshotQueueItem;

namespace usage {
	
	/*********************/
	/**** Queue class ****/
	/*********************/
	class Queue {
		
	protected:
		ThreadQueue* queue;
		boost::thread thread;
		
		long _size;
		
		boost::shared_ptr<TSocket> socket;
		boost::shared_ptr<TTransport> transport;
		boost::shared_ptr<TProtocol> protocol;
		wire::TocServiceClient *client;
		
		void* sendingObj;
		bool resetConnectionP;
		bool shutdownP;
		
		bool createConnection();
		void teardownConnection();
		
		void processQueue();
		virtual bool processQueueH() = 0;
		virtual void clearQueueItem(void *obj) = 0;
		
	public:
		Queue();
		~Queue();
		void start();
		void shutdown(long gracePeriodSeconds);
		void push(void *obj);
		void* pop();
		void* pop_timeout();
		bool empty();
		long size();
	};
	
	/****************************/
	/**** MessageQueue class ****/
	/****************************/
	class MessageQueue: public Queue {
	public:
		bool processQueueH();
		void clearQueueItem(void *obj);
	};
	
	/*******************************/
	/**** ScreenshotQueue class ****/
	/*******************************/
	class ScreenshotQueue: public Queue {
	private:
		ImageFormatData im_format;
		void readEntireFile(const char *filepath, std::string &str);
	public:
		ScreenshotQueue();
		bool processQueueH();
		void clearQueueItem(void *obj);
	};

	
	/*********************/
	/**** Usage class ****/
	/*********************/
	class Usage {
		
	private:
		
		bool enabledP;
		bool updateSettingsP;
		bool takeScreenshotP;
		long lastScreenshotTimestamp;
		
		std::string sessionKey;
		std::string frameUUID;
		wmWindow *frameWin;
		
		boost::uuids::random_generator uuidGenerator;
		
		MessageQueue messageQueue;
		ScreenshotQueue screenshotQueue;
		
		wire::data::WmEvMouseMoves* mouseMoves;
		
		Usage();
		~Usage();
		Usage(Usage const&);
		
		void operator=(Usage const&);
		
		ImBuf* take_screenshot(bContext *C, const bool crop);
		void read_entire_file(const char *filepath, std::string &str);
		
		wire::Message *getNewMessage();
		std::string p2s(void *p);
		
		wire::data::Context *getNewContext(const struct bContext *C);
		void setProperty(wire::data::RNAProperty *thriftProp, bContext *C, PointerRNA* ptr, PropertyRNA* prop);
		std::string generateUUID();
		
		bool shouldQueue();
		
	public:
		static Usage& getInstance();
		static long getTimestamp();
		
		bool isEnabled();
		void disable();
		void updateSettings();
		
		void mainPrepare();
		void mainTakeScreenshot(bContext *C);
		
		void ping();
		void queueOperator(bContext *C, wmOperator *op, int retval, int repeat);
		void queueEvent(bContext *C, const wmEvent *ev);
		void queueButtonPress(bContext *C, uiBut *but);
		void queueAssignment(bContext *C, PointerRNA *ptr, PropertyRNA *prop, int index);
		void queueStart();
		void queueEnd();
		
		void free();
	};
	
} /* namespace */
#endif //__cplusplus


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
		
/* C interface */
	
struct wmOperator;
struct bContext;
struct wmEvent;
struct uiBut;
struct PointerRNA;
struct PropertyRNA;
	
void BKE_usage_main_prepare(void);
void BKE_usage_main_take_screenshot(struct bContext *C);

void BKE_usage_queue_operator(struct bContext *C, struct wmOperator *op, int retval, int repeat);
void BKE_usage_queue_event(struct bContext *C, const struct wmEvent *ev);
void BKE_usage_queue_button(struct bContext *C, struct uiBut *but);
void BKE_usage_queue_assignment(struct bContext *C, struct PointerRNA *ptr, struct PropertyRNA *prop, int index);
void BKE_usage_queue_start(void);

void BKE_usage_update_settings(void);
void BKE_usage_shutdown(void);
	
#ifdef __cplusplus
}
#endif

#endif /* WITH_USAGE */

#endif /* __BKE_USAGE_H__ */
