/*
 * SegwayPlotterCom.h
 *
 *  Created on: Feb 5, 2014
 *      Author: john
 */

#ifndef SEGWAYPLOTTERCOM_H_
#define SEGWAYPLOTTERCOM_H_

#include <string>

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
		virtual ~SegwayPlotterCom();
		bool conn( std::string host, int port );
		bool sendData( std::string data );

	private:
		struct addrinfo *host_info_list;
		int sock_fd;
};

#endif /* SEGWAYPLOTTERCOM_H_ */
