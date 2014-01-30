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
	int fd = pfd.fd;
	int ret = -1;

	while( running )
	{
		memset( &rdbuf, 0, sizeof( rdbuf ) );
		lseek( fd, 0, SEEK_SET );	//Place file descriptor at beginning of file

		ret = poll( &pfd, 1, 100 );	//Dont poll for -1 since we need to be able to stop()
		if( ret < 0 )		//Error
			throw std::string( "poll failed." );
		else if( ret == 0 )	//Timeout
			continue;

		ret = read( fd, rdbuf, sizeof( rdbuf ) - 1 );	//Read the pin value;
		if( ret < 0 )
			throw std::string( "read failed." );

		//Handle interrupt
		std::cout << "Interrupt!" << std::endl << "Value: " << rdbuf << std::endl << std::endl;
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
