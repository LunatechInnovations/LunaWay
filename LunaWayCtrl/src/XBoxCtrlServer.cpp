/* XBoxCtrlServer.cpp

Copyright (C) 2014 Lunatech Innovations

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "XBoxCtrlServer.h"
#include <iostream>
#include <cstring>
#include <sstream>

extern "C"
{
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
}

/*! \fn XBoxCtrlServer::XBoxCtrlServer( uint16_t port )
 * \brief Constructor
 * @param[in] port Port to open for communication with client
 */
XBoxCtrlServer::XBoxCtrlServer( uint16_t port )
			  : _port( port ), vertical( 0.0f ), horizontal( 0.0f ), fd( -1 ), cli_fd( -1 ), host_info_list( nullptr )
{
}

/*! \fn XBoxCtrlServer::~XBoxCtrlServer()
 * \brief Destructor
 */
XBoxCtrlServer::~XBoxCtrlServer()
{
}

/*! \fn XBoxCtrlServer::getVertical()
 * \brief getter for Vertical joystick axis
 */
double XBoxCtrlServer::getVertical()
{
	std::lock_guard<std::mutex> lock( vertical_mutex );
	return vertical;
}

/*! \fn XBoxCtrlServer::getHorizontal()
 * \brief getter for Horizontal joystick axis
 */
double XBoxCtrlServer::getHorizontal()
{
	std::lock_guard<std::mutex> lock( horizontal_mutex );
	return horizontal;
}

/*! \fn XBoxCtrlServer::cyclic()
 * \brief Listen for connections and receive data.
 * \details This is the implementation of AbstractCyclicThreads pure virtual function
 */
void XBoxCtrlServer::cyclic()
{
	std::cout << "Listen()ing for connections..."  << std::endl;
	if( (listen( fd, 5 )) < 0 )
		return;

	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof( their_addr );
	cli_fd = accept( fd, (struct sockaddr *)&their_addr, &addr_size );
	if( cli_fd == -1 )
		return;

	while( running )
	{
		char rx[128];
		memset( &rx, 0, sizeof( rx ) );
		int br = recv( cli_fd, &rx, sizeof( rx ), 0 );
		if( br < 0 )
		{
			throw std::string( "recv failed" );
			running = false;
			return;
		}
		else if( br == 0 )
		{
			break;
		}

		std::string data( rx );
		std::string hor, ver, curr;

		for( auto i : data )
		{
			if( i == ';' )
			{
				hor = curr;
				curr.clear();
			}
			else
			{
				curr.push_back( i );
			}
		}
		ver = curr;

		std::stringstream conv;
		conv << hor;
		conv >> horizontal;
		conv.clear();
		conv << ver;
		conv >> vertical;
	}
}

/*! \fn XBoxCtrlServer::connect()
 * \brief Setup connection
 */
void XBoxCtrlServer::connect()
{
	struct addrinfo host_info;
	memset( &host_info, 0, sizeof( struct addrinfo ) );

	host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
	host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
	host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

	std::stringstream port_conv;
	port_conv << _port;

	if( (getaddrinfo( nullptr, port_conv.str().c_str(), &host_info, &host_info_list )) != 0 )
		throw std::string( "getaddrinfo failed." );

	//Create socket
	//We use the first available interface.
	fd = socket( host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol );
	if( fd == -1 )
		throw std::string( "socket failed." );

	int yes = 1;
	if( (setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int) )) < 0 )
		throw std::string( "setsockopt failed" );

	struct timeval recv_timeout;
	recv_timeout.tv_sec = 1;
	recv_timeout.tv_usec = 0;
	if( (setsockopt( fd, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof( struct timeval ) )) < 0 )
		throw std::string( "setsockopt failed: recv timeout" );

	if( (bind( fd, host_info_list->ai_addr, host_info_list->ai_addrlen )) < 0 )
		throw std::string( "bind failed" );



	start();
}
