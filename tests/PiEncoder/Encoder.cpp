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

Encoder::Encoder( int pin, int mode ) : running( false ), _mode( mode )
{
	std::stringstream path;
	path << "/sys/class/gpio/gpio" << pin << "/value";

	pfd.fd = open( path.str().c_str(), O_RDONLY );
	if( pfd.fd < 0 )
		throw (std::string)"Failed to open: " + path.str();
	pfd.events = POLLPRI;

	running = true;
	poll_thread = std::thread( &Encoder::threaded_poll, this );
}

Encoder::~Encoder()
{
	close( pfd.fd );
	stop();
}

void Encoder::threaded_poll()
{
	char rdbuf[5];
	int ret = -1;
	std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();

	while( running )
	{
		memset( &rdbuf, 0, sizeof( rdbuf ) );
		lseek( pfd.fd, 0, SEEK_SET );	//Place file descriptor at beginning of file

		ret = poll( &pfd, 1, 1000 );	//Dont poll for -1 since we need to be able to stop()
		if( ret < 0 )		//Error
			throw std::string( "poll failed." );
		else if( ret == 0 )	//Timeout
			continue;

		//Have to perform a dummy read. Interrupt will be triggered otherwise.
		uint8_t dummy;
		ret = read( pfd.fd, &dummy, 1 );

		//Interrupt occurred
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		long diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
		last = now;

		std::cout << "Interrupt!" << std::endl << "Diff: " << diff << std::endl << std::endl;
	}
}

void Encoder::stop()
{
	if( running )
	{
		running = false;
		poll_thread.join();
	}
}
