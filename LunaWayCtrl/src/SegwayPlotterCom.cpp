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

SegwayPlotterCom::SegwayPlotterCom()
{
	sock_fd = -1;
	host_info_list = nullptr;
}

SegwayPlotterCom::~SegwayPlotterCom()
{
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

	return true;
}

bool SegwayPlotterCom::sendData( std::string data )
{
	return send( sock_fd, (void *)data.c_str(), (int)data.size(), 0 ) != -1;
}
