/* Encoder.cpp

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

#include <sstream>
#include <cstdlib>
#include <exception>
#include <cstring>
#include <iostream>
#include <chrono>

#include "Encoder.h"

extern "C"
{
#include <fcntl.h>
#include <unistd.h>
}

#define MAX_INT_DELAY_BUF_ENTRIES 10
#define PULSES_PER_REVOLUTION 15

/** Encoder
 * Generic constructor
 */
Encoder::Encoder() : _pin( nullptr ), _int_value( false )
{
}

/** Encoder
 * Default constructor
 * @param pin GPIO pin for encoder interrupt
 */
Encoder::Encoder( GPIOPin *pin ) : _pin( pin )
{
	if( _pin == nullptr )
		throw std::string( "Pin is NULL" );

	last_int = std::chrono::high_resolution_clock::now();

	_pin->setupInterrupt( std::bind( &Encoder::count, this, false ), GPIOPin::Rising );

}

/** ~Encoder
 * Generic destructor
 */
Encoder::~Encoder()
{
}

/** getRps
 * Returns a filtered revolutions per secound value
 */
double Encoder::getRps()
{
	std::lock_guard<std::mutex> lock( int_delay_buf_mutex );

	if( int_delay_buf.size() <= 0 )
		return 0.0f;

	double avg_delay = 0.0f;
	for( auto d : int_delay_buf )
		avg_delay += d;

	return (1.0f / (avg_delay / int_delay_buf.size())) / PULSES_PER_REVOLUTION;
}

/** count
 * Callback function for interrupt pin
 * @param value Pin value
 */
void Encoder::count( bool value )
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;

	high_resolution_clock::time_point now = high_resolution_clock::now();
	int diff = duration_cast<std::chrono::microseconds>( now - last_int ).count();
	if( diff < 0 )
		return;

	//Lock mutex
	std::lock_guard<std::mutex> lock( int_delay_buf_mutex );

	//TODO clear buffer
	//Add delay to filtering buffer
	int_delay_buf.push_back( (double)((double)diff / 1000000.0f) );

	//Limit the buffer size
	if( int_delay_buf.size() > MAX_INT_DELAY_BUF_ENTRIES )
		int_delay_buf.erase( int_delay_buf.begin() );

	last_int = now;
}
