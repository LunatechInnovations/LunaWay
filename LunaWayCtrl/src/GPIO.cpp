/*
 * GPIO.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: john
 */

#include "GPIO.h"
#include <iostream>
#include <sstream>
#include <fstream>

extern "C"
{
#include <fcntl.h>
#include <unistd.h>
}


GPIO::GPIO( int pin ) : _fd( -1 ), _pin( pin ), _dir( -1 )
{
	std::ofstream export_file;
	export_file.open( "/sys/class/gpio/export" );
	export_file << _pin;
	export_file.close();
}

GPIO::~GPIO()
{
	if( _fd > 0 )
		close( _fd );

	std::ofstream unexport_file;
	unexport_file.open( "/sys/class/gpio/unexport" );
	unexport_file << _pin;
	unexport_file.close();
}

bool GPIO::getValue()
{
	char rx;

	//Start reading from beginning of file
	lseek( _fd, SEEK_SET, 0 );
	size_t bytes_read = read( _fd, &rx, 1 );

	if( bytes_read < 1 )
	{
		std::stringstream e;
		e << "Failed to read value.\n bytes_read=" << bytes_read;

		throw e.str();
	}

	return rx == '1' ? true : false;
}

void GPIO::setupInterrupt( int edge )
{
}

void GPIO::setValue( bool value )
{
	if( _dir == Input )
		return;

	char tx = value ? '1' : '0';
	if( (write( _fd, (void *)&tx, 1 )) < 1 )
		throw std::string( "Failed to write value." );
}

void GPIO::setupDir( int dir )
{
	std::ofstream dir_file;
	std::stringstream dir_filename;
	dir_filename << "/sys/class/gpio/gpio" << _pin << "/direction";
	dir_file.open( dir_filename.str().c_str() );

	std::stringstream value_filename;
	value_filename << "/sys/class/gpio/gpio" << _pin << "/value";

	if( dir == Input )
	{
		dir_file << "in";

		if( (_fd = open( value_filename.str().c_str(), O_RDONLY )) < 0 )
			throw std::string( "Failed to open value file for reading." );
	}
	else if( dir == Output )
	{
		dir_file << "out";

		if( (_fd = open( value_filename.str().c_str(), O_RDWR )) < 0 )
			throw std::string( "Failed to open value file for reading." );
	}

	_dir = dir;

	dir_file.close();
}
