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
#include <vector>

extern "C"
{
#include <sys/types.h>
}

#define RECV_BUFFER_SIZE 1024

SegwayPlotterCom::SegwayPlotterCom()
{
	sock_fd = -1;
	host_info_list = nullptr;
	_pid = nullptr;
}

SegwayPlotterCom::SegwayPlotterCom( PID *pid ) : _pid( pid )
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

	start();

	return true;
}

bool SegwayPlotterCom::sendData( std::string data )
{
	return send( sock_fd, (void *)data.c_str(), (int)data.size(), 0 ) != -1;
}


void SegwayPlotterCom::cyclic()
{
	//Set timeout for recv function
	struct timeval recv_timeout;
	recv_timeout.tv_sec =  2;
	recv_timeout.tv_usec = 0;

	setsockopt( sock_fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&recv_timeout, sizeof( struct timeval ) );

	while( running )
	{
		char rx[RECV_BUFFER_SIZE];
		int bytes_received = recv( sock_fd, &rx, sizeof( rx ), 0 );

		//Print received data to stdout if any.
		if( bytes_received > 0 )
			setPID( std::string( rx ) );
	}
}

void SegwayPlotterCom::setPID( std::string msg )
{
	std::vector<std::string> entries;
	std::string current;
	//Split message string at ';'
	for( auto i : msg )
	{
		if( i == ';' )
		{
			entries.push_back( current );
			current.clear();
		}
		else
		{
			current.push_back( i );
		}
	}

	//Check entries for parameters
	for( auto i : entries )
	{
		std::stringstream conv;
		double value = 0.0f;

		if( i.substr( 0, 2 ) == "p=" )
		{
			conv << i.substr( 2, i.size() - 1 );
			conv >> value;

			_pid->setP( value );
		}
		else if( i.substr( 0, 2 ) == "i=" )
		{
			conv << i.substr( 2, i.size() - 1 );
			conv >> value;

			_pid->setI( value );
		}
		else if( i.substr( 0, 2 ) == "d=" )
		{
			conv << i.substr( 2, i.size() - 1 );
			conv >> value;

			_pid->setD( value );
		}
	}
}
