/*
 * Encoder.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: john
 */

#include "Encoder.h"
#include <sstream>
#include <cstdlib>
#include <exception>
#include <cstring>
#include <iostream>
#include <chrono>

extern "C"
{
#include <fcntl.h>
#include <unistd.h>
}

Encoder::Encoder() : _pin( nullptr )
{
}

Encoder::Encoder( GPIOPin *pin ) : _pin( pin )
{
	if( _pin == nullptr )
		throw std::string( "Pin is NULL" );

	last_int = std::chrono::high_resolution_clock::now();

	_pin->setupInterrupt( std::bind( &Encoder::count, this, false ), GPIOPin::Rising );

}

Encoder::~Encoder()
{
}

double Encoder::getRps()
{
	if( int_delay_buf.size() <= 0 )
		return 0.0f;

	double avg_delay = 0.0f;
	for( auto d : int_delay_buf )
		avg_delay += d;

	return (1.0f / (avg_delay / int_delay_buf.size())) / PULSES_PER_REVOLUTION;
}

void Encoder::count( bool value )
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;

	high_resolution_clock::time_point now = high_resolution_clock::now();
	int diff = duration_cast<std::chrono::microseconds>( now - last_int ).count();
	if( diff < 0 )
		return;

	int_delay_buf.push_back( (double)((double)diff / 1000000.0f) );

	//Limit the buffer size
	if( int_delay_buf.size() > MAX_INT_DELAY_BUF_ENTRIES )
		int_delay_buf.erase( int_delay_buf.begin() );

	last_int = now;
}
