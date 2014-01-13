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
#include "BLI_threads.h" 

#include "DNA_windowmanager_types.h"
}

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include "intern/usage/message_types.h"

#include "intern/usage/TocService.h"

using namespace wire;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

namespace usage {

	class Usage {
		
	private:
		ListBase threads;
		ThreadQueue *messageQueue, *screenshotQueue;
		bool shutdown;
		bool enabled;
		bool updateSettingsP;
		Message *sendingMessage;
		Screenshot *sendingScreenshot;
		
		boost::shared_ptr<TSocket> socket;
		boost::shared_ptr<TTransport> transport;
		boost::shared_ptr<TProtocol> protocol;
		TocServiceClient *client;
		
		boost::uuids::random_generator uuidGenerator;
		
		Usage();
		~Usage();
		Usage(Usage const&);
		void operator=(Usage const&);
		
		void createConnection();
		void teardownConnection();
		void read_entire_file(const char *filepath, std::string &str);
		long getTimestamp();
		Message *getNewMessage();
		
		void handleQueue();
		
	public:
		static Usage& getInstance();
		void updateSettings();
		void ping();
		void doThread();
		void queueOperator(bContext *C, wmOperator *op);
		void queueEvent(bContext *C, const wmEvent *ev);
		void free();
	};
	
} /* namespace */

extern "C" {
#else /* __cplusplus */

/* C interface */
void BKE_usage_update_settings(void);
void BKE_usage_queue_operator(struct bContext *C, struct wmOperator *op);
void BKE_usage_queue_event(struct bContext *C, const struct wmEvent *ev);
void BKE_usage_shutdown();
	
#endif /* __cplusplus */
#ifdef __cplusplus
}
#endif

#endif /* WITH_USAGE */

#endif /* __BKE_USAGE_H__ */
