/* Encoder.h

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

#ifndef ENCODER_H_
#define ENCODER_H_

#include <vector>
#include "GPIOPin.h"
#include <chrono>

extern "C"
{
#include <poll.h>
}

#define MAX_INT_DELAY_BUF_ENTRIES 10
#define PULSES_PER_REVOLUTION 15

class Encoder
{
	public:
		Encoder();
		Encoder( GPIOPin *pin );
		virtual ~Encoder();
		double getRps();

	private:
		std::vector<double> int_delay_buf;
		void count( bool value );
		GPIOPin *_pin;
		std::chrono::high_resolution_clock::time_point last_int;
};

#endif /* ENCODER_H_ */
