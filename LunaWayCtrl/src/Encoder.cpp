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

Encoder::Encoder()
{
}

Encoder::Encoder( int pin )
{
	std::stringstream path;
	path << "/sys/class/gpio/gpio" << pin << "/value";

	pfd.fd = open( path.str().c_str(), O_RDONLY );
	if( pfd.fd < 0 )
		throw (std::string)"Failed to open: " + path.str();
	pfd.events = POLLPRI;

	int_delay_buf.clear();

	start();
}

Encoder::~Encoder()
{
	close( pfd.fd );
	stop();
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

void Encoder::cyclic()
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;

	char rdbuf[5];
	int ret = -1;
	high_resolution_clock::time_point last_int = high_resolution_clock::now();

	while( running )
	{
		memset( &rdbuf, 0, sizeof( rdbuf ) );
		lseek( pfd.fd, 0, SEEK_SET );	//Place file descriptor at beginning of file

		ret = poll( &pfd, 1, 100 );	//Dont poll for -1 since we need to be able to stop()
		if( ret < 0 )		//Error
		{
			throw std::string( "poll failed." );
		}
		else if( ret == 0 )	//Timeout
		{
			int_delay_buf.clear();
			continue;
		}
		//Interrupt occurred

		//Have to perform a dummy read. Interrupt will be triggered otherwise.
		uint8_t dummy;
		ret = read( pfd.fd, &dummy, 1 );

		//Add current delay time to buffer
		high_resolution_clock::time_point now = high_resolution_clock::now();
		int diff = duration_cast<std::chrono::microseconds>( now - last_int ).count();
		if( diff < 0 )
			continue;

		int_delay_buf.push_back( (double)((double)diff / 1000000.0f) );

		//Limit the buffer size
		if( int_delay_buf.size() > MAX_INT_DELAY_BUF_ENTRIES )
			int_delay_buf.erase( int_delay_buf.begin() );

		last_int = now;
	}
}
