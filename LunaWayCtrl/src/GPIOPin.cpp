/* GPIOPin.cpp

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

#include "GPIOPin.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstring>
#include <fstream>

extern "C"
{
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
}

#define INP_GPIO(g) *(_gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(_gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(_gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(_gpio+10) // clears bits which are 1 ignores bits which are 0
#define GPIO_LEV *(_gpio+13)                  // pin level


GPIOPin::GPIOPin( int pin, volatile unsigned *gpio ) : _gpio( gpio ), _mode( Unassigned ), _pin( pin ), running( false )
{
}

GPIOPin::~GPIOPin()
{
	if( _mode == Interrupt )
	{
		std::ofstream uexp_file;
		uexp_file.open( "/sys/class/gpio/unexport" );
		uexp_file << _pin;
		uexp_file.close();
	}
}

void GPIOPin::setupInput()
{
	if( _mode != Unassigned )
		throw std::string( "setupInput called on already assigned pin" );

	_mode = Input;
	INP_GPIO( _pin );
}

void GPIOPin::setupOutput()
{
	if( _mode != Unassigned )
		throw std::string( "setupOutput called on already assigned pin" );

	_mode = Output;
	INP_GPIO( _pin );
	OUT_GPIO( _pin );
}

void GPIOPin::setupInterrupt( std::function<void(bool)> callback, int edge )
{
	if( _mode != Unassigned )
		throw std::string( "setupInterrupt called on already assigned pin" );

	_mode = Interrupt;

	//Export pin
	std::ofstream export_file;
	export_file.open( "/sys/class/gpio/export" );
	export_file << _pin;
	export_file.close();

	//Setup edge
	std::ofstream edge_file;
	std::stringstream edge_file_path;
	edge_file_path << "/sys/class/gpio/gpio" << _pin << "/edge";
	edge_file.open( edge_file_path.str().c_str() );
	if( edge == Rising )
		edge_file << "rising";
	else if( edge == Falling )
		edge_file << "falling";
	edge_file.close();

	//Open value file
	std::stringstream gpio_path;
	gpio_path << "/sys/class/gpio/gpio" << _pin << "/value";

	pfd.fd = open( gpio_path.str().c_str(), O_RDONLY );
	if( pfd.fd < 0 )
		throw std::string( "Failed to open: " ) + gpio_path.str();
	pfd.events = POLLPRI;

	interrupt_callback = callback;

	running = true;
	cyclic_thread = std::thread( &GPIOPin::cyclic, this );
}

void GPIOPin::setValue( bool value )
{
	if( _mode == Output )
	{
		if( value )
			GPIO_SET = 1 << _pin;
		else
			GPIO_CLR = 1 << _pin;
	}
	else if( _mode == Unassigned )
	{
		throw std::string( "setValue called on unassigned pin" );
	}
}

bool GPIOPin::getValue()
{
	if( _mode == Unassigned )
		throw std::string( "getValue called on unassigned pin" );

	unsigned int value = GPIO_LEV;               // reading all 32 pins
	return ((value & (1 << _pin)) != 0);
}

void GPIOPin::cyclic()
{
	if( _mode == Interrupt )
		poll_interrupt();
	else
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
}

void GPIOPin::poll_interrupt()
{
	try
	{
		using std::chrono::high_resolution_clock;
		using std::chrono::duration_cast;

		int ret = -1;


		while( running )
		{
			lseek( pfd.fd, 0, SEEK_SET );	//Place file descriptor at beginning of file

			ret = poll( &pfd, 1, 100 );	//Dont poll for -1 since we need to be able to stop()
			if( ret < 0 )		//Error
				throw std::string( "poll failed." );
			else if( ret == 0 )	//Timeout
				continue;

			//Interrupt occurred
			//Read pin value
			char rx;
			ret = read( pfd.fd, &rx, 1 );

			//Execute callback function
//			interrupt_callback( rx == '1' ? true : false );
			interrupt_callback( true );
		}
	}
	catch( std::bad_function_call &e )
	{
		std::cerr << "Bad function call: " << e.what() << std::endl;
	}

	running = false;
}
