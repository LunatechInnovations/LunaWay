/* AbstractCyclicThread.h

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

#ifndef ABSTRACTCYCLICTHREAD_H_
#define ABSTRACTCYCLICTHREAD_H_

#include <thread>

/*! \class AbstractCyclicThread
 * \brief This class provides an abstract baseclass for a cyclic threaded function.\n
 * \details The pure virtual function cyclic will be called repeatedly after start has been called
 */
class AbstractCyclicThread
{
	public:
		AbstractCyclicThread();
		virtual ~AbstractCyclicThread();
		void start();
		void stop();

	protected:
		std::thread thread;
		volatile bool running;
		void hlp_cyclic();
		virtual void cyclic() = 0;
};

#endif /* ABSTRACTCYCLICTHREAD_H_ */
