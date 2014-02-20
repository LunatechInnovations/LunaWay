/* AbstractCyclicThread.cpp

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

#include "AbstractCyclicThread.h"

/*! \fn AbstractCyclicThread::AbstractCyclicThread()
 * \brief Constructor
 */
AbstractCyclicThread::AbstractCyclicThread() : running( false )
{
}

/* \fn AbstractCyclicThread::~AbstractCyclicThread()
 * \brief Destructor.
 * \details Stop and join the thread.
 */
AbstractCyclicThread::~AbstractCyclicThread()
{
	stop();
}

/*! \fn AbstractCyclicThread::start()
 * \brief Start the thread
 */
void AbstractCyclicThread::start()
{
	if( !running )
	{
		running = true;
		thread = std::thread( &AbstractCyclicThread::hlp_cyclic, this );
	}
}

/*! \fn AbstractCyclicThread::stop()
 * \brief Stop and join the tread.
 */
void AbstractCyclicThread::stop()
{
	if( running )
	{
		running = false;
		thread.join();
	}
}

/*! \fn AbstractCyclicThread::hlp_cyclic()
 * \brief Keep executing cyclic as long as running is true
 */
void AbstractCyclicThread::hlp_cyclic()
{
	while( running )
		cyclic();
}
