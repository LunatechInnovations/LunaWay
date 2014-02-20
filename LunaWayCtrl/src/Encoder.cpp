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

/*! \fn Encoder::Encoder()
 * \brief Generic constructor
 */
Encoder::Encoder() : _pin( nullptr ), _int_value( false )
{
}

/*! \fn Encoder::Encoder( GPIOPin *pin )
 * \brief Default constructor
 * @param[in] pin GPIO pin for encoder interrupt
 */
Encoder::Encoder( GPIOPin *pin ) : _pin( pin ), _int_value( false )
{
	if( _pin == nullptr )
		throw std::string( "Pin is NULL" );

	last_int = std::chrono::high_resolution_clock::now();

	_pin->setupInterrupt( std::bind( &Encoder::count, this, false ),
						  std::bind( &Encoder::poll_timeout, this ),
						  GPIOPin::Rising );
}

/*! \fn Encoder::~Encoder()
 * \brief Generic destructor
 */
Encoder::~Encoder()
{
}

/*! \fn double Encoder::getRps()
 * \brief Calculate revolutions per second
 * \return A filtered revolutions per second value
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

/*! \fn void Encoder::count( bool value )
 * \brief Callback function for interrupt pin
 * @param[in] value Pin value
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

	//Add delay to filtering buffer
	int_delay_buf.push_back( (double)((double)diff / 1000000.0f) );

	//Limit the buffer size
	if( int_delay_buf.size() > MAX_INT_DELAY_BUF_ENTRIES )
		int_delay_buf.erase( int_delay_buf.begin() );

	last_int = now;
}

/*! \fn void Encoder::poll_timeout()
 * \brief Timeout function connected to interrupt pin
 * \details Clear buffer
 */
void Encoder::poll_timeout()
{
	int_delay_buf.clear();
}
