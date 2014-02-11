/*
 * SegwayPlotterCom.cpp
 *
 *  Created on: Feb 5, 2014
 *      Author: john
 */

#include "SegwayPlotterCom.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <cstdlib>

extern "C"
{
#include <sys/types.h>
}

#define RECV_BUFFER_SIZE 1024

SegwayPlotterCom::SegwayPlotterCom()
{
	sock_fd = -1;
	host_info_list = nullptr;
	running = false;
}

SegwayPlotterCom::~SegwayPlotterCom()
{
	stop();
}

bool SegwayPlotterCom::conn( std::string host, int port )
{
	struct addrinfo host_info;
	memset( &host_info, 0, sizeof( struct addrinfo ) );
	std::stringstream port_str;
	port_str << port;

	host_info.ai_family = AF_UNSPEC; 		//Accept both ipv4 and ipv6
	host_info.ai_socktype = SOCK_STREAM;	//TCP socket

	if( (getaddrinfo( host.c_str(), port_str.str().c_str(), &host_info, &host_info_list )) != 0 )
	{
		std::cerr << "getaddrinfo failed." << std::endl;
		return false;
	}

	//Create socket
	//We use the first available interface.
	sock_fd = socket( host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol );
	if( sock_fd == -1 )
	{
		std::cerr << "socket failed." << std::endl;
		return false;
	}

	if( (connect( sock_fd, host_info_list->ai_addr, host_info_list->ai_addrlen )) == -1 )
	{
		std::cerr << "connect failed." << std::endl;
		return false;
	}

	recv_thread = std::thread( &SegwayPlotterCom::recv_cyclic, this );

	return true;
}

bool SegwayPlotterCom::sendData( std::string data )
{
	return send( sock_fd, (void *)data.c_str(), (int)data.size(), 0 ) != -1;
}

void SegwayPlotterCom::stop()
{
	if( running )
	{
		running = false;
		recv_thread.join();
	}
}

void SegwayPlotterCom::recv_cyclic()
{
	//Set timeout for recv function
	struct timeval recv_timeout;
	recv_timeout.tv_sec =  1;
	recv_timeout.tv_usec = 0;

	setsockopt( sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&recv_timeout, sizeof( struct timeval ) );

	running = true;
	while( running )
	{
		char rx[RECV_BUFFER_SIZE];
		int bytes_received = recv( sock_fd, &rx, sizeof( rx ), 0 );

		//Print received data to stdout if any.
		if( bytes_received > 0 )
		{
			std::cout << rx << std::endl;
		}
		else if( bytes_received == 0 )
		{
			std::cout << "recv timeout" << std::endl;
		}
		else
		{
			throw std::string( "recv failed." );
		}
	}
}
