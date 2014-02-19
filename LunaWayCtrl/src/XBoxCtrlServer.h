/* XBoxCtrlServer.h

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
