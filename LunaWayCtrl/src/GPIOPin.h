/* GPIOPin.h

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

#ifndef GPIOPIN_H_
#define GPIOPIN_H_

#include <mutex>
#include <thread>
#include <functional>
#include "AbstractCyclicThread.h"

extern "C"
{
#include <poll.h>
}

/*! \class GPIOPin
 * \brief An interface for the Raspberry Pi's GPIO pins.
 * \details No instances should be created from outside the singleton class GPIO. Use GPIO::getPin().
 */
class GPIOPin : public AbstractCyclicThread
{
		enum PinModes
		{
			Unassigned, Input, Output, Interrupt
		};

	public:
		enum Edges
		{
			Rising, Falling
		};

		GPIOPin( int pin, volatile unsigned *gpio );
		virtual ~GPIOPin();
		void setupInput();
		void setupOutput();
		void setupInterrupt( std::function<void(bool)> int_callback, int edge );
		void setupInterrupt( std::function<void(bool)> int_callback, std::function<void()> timeo_callback, int edge );
		void setValue( bool value );
		bool getValue();


	private:
		void cyclic();
		volatile unsigned *_gpio;
		void poll_interrupt();
		int _mode;
		int _pin;
		std::mutex mem_mutex;
		struct pollfd pfd;	//Interrupt pollrequest
		std::function<void(bool)> interrupt_callback;
		std::function<void(void)> timeout_callback;
};

#endif /* GPIOPIN_H_ */
