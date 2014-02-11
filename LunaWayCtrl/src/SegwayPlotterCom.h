/*
 * SegwayPlotterCom.h
 *
 *  Created on: Feb 5, 2014
 *      Author: john
 */

#ifndef SEGWAYPLOTTERCOM_H_
#define SEGWAYPLOTTERCOM_H_

#include <string>
#include <thread>
#include "PID.h"

extern "C"
{
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
}

class SegwayPlotterCom
{
	public:
		SegwayPlotterCom();
		SegwayPlotterCom( PID *pid );
		virtual ~SegwayPlotterCom();
		bool conn( std::string host, int port );
		bool sendData( std::string data );
		void stop();

	private:
		struct addrinfo *host_info_list;
		int sock_fd;
		void recv_cyclic();
		std::thread recv_thread;
		volatile bool running;
		PID *_pid;
		void setPID( std::string msg );
};

#endif /* SEGWAYPLOTTERCOM_H_ */
