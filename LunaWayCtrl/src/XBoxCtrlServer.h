/*
 * XBoxCtrlServer.h
 *
 *  Created on: Feb 16, 2014
 *      Author: john
 */

#ifndef XBOXCTRLSERVER_H_
#define XBOXCTRLSERVER_H_

#include "AbstractCyclicThread.h"
#include <mutex>

extern "C"
{
#include <inttypes.h>
}

class XBoxCtrlServer : public AbstractCyclicThread
{
	public:
		XBoxCtrlServer( uint16_t port );
		virtual ~XBoxCtrlServer();
		double getVertical();
		double getHorizontal();
		void cyclic();
		void connect();

	private:
		uint16_t _port;
		double vertical, horizontal;
		std::mutex vertical_mutex, horizontal_mutex;
		int fd, cli_fd;
		struct addrinfo *host_info_list;
};

#endif /* XBOXCTRLSERVER_H_ */
