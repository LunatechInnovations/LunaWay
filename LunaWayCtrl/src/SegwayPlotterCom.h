/* SegwayPlotterCom.h

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
#ifndef SEGWAYPLOTTERCOM_H_
#define SEGWAYPLOTTERCOM_H_

#include <string>
#include "PID.h"
#include "AbstractCyclicThread.h"

extern "C"
{
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
}

class SegwayPlotterCom : public AbstractCyclicThread
{
	public:
		SegwayPlotterCom();
		SegwayPlotterCom( PID *pid );
		virtual ~SegwayPlotterCom();
		bool conn( std::string host, int port );
		bool sendData( std::string data );

	private:
		struct addrinfo *host_info_list;
		int sock_fd;
		void cyclic();
		PID *_pid;
		void setPID( std::string msg );
};

#endif /* SEGWAYPLOTTERCOM_H_ */
