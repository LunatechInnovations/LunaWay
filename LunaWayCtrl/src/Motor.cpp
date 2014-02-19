/* Motor.cpp

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

#include "Motor.h"
#include <chrono>
#include <cmath>
#include <iostream>

using namespace std::chrono;

Motor::Motor() : _dir( nullptr ), _pwm( nullptr ), output( 0.0f ), _freq( 0 ), encoder( nullptr )
{
}

Motor::Motor( GPIOPin* dir, GPIOPin* pwm, GPIOPin *enc, int freq ) : _dir( dir ), _pwm( pwm ), _freq( freq )
{
	output = 0.0f;
	encoder = new Encoder( enc );

	_dir->setupOutput();
	_pwm->setupOutput();

	start();
}

Motor::~Motor()
{
	delete encoder;
}

void Motor::setOutput( double value )
{
	if( value < -100.0f || value > 100.0f )
		return;

	output_mutex.lock();
	output = value;
	output_mutex.unlock();
}

double Motor::getRPS()
{
	return encoder->getRps();
}

double Motor::getOutput()
{
	std::lock_guard<std::mutex> lock( output_mutex );
	return output;
}

void Motor::cyclic()
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	double pwm_time = 1.0f / _freq;		//period time

	while( running )
	{
		output_mutex.lock();
		double tmp_outp = output > 0.0f ? output : output * -1;
		bool dir = output > 0.0f;
		output_mutex.unlock();

		microseconds off_us( (int)(pwm_time * (tmp_outp / 100.0f) * 1000000.0f) );
		microseconds total_us( (int)(pwm_time * 1000000.0f) );

		if( tmp_outp == 0.0f )
		{
			_pwm->setValue( true );
			std::this_thread::sleep_for( total_us );
			continue;
		}

		if( dir )
			_dir->setValue( true );
		else
			_dir->setValue( false );


		_pwm->setValue( false );
		std::this_thread::sleep_for( off_us );

		_pwm->setValue( true );
		std::this_thread::sleep_for( total_us - off_us );
	}
}

void Motor::stop()
{
	if( running )
	{
		running = false;
		thread.join();
	}
	_pwm->setValue( true );
}

