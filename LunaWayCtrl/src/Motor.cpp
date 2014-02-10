/*
 * Motor.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: john
 */

#include "Motor.h"
#include <wiringPi.h>
#include <chrono>
#include <cmath>
#include <iostream>

using namespace std::chrono;
Motor::Motor( int dirpin, int pwmpin, int encoderpin, int freq )
	  : _dirpin( dirpin ), _pwmpin( pwmpin ), _freq( freq )
{
	output = 0.0f;
	running = false;

	pinMode( dirpin, OUTPUT );
	pinMode( pwmpin, OUTPUT );

	encoder = new Encoder( encoderpin );

	thread = std::thread( &Motor::thread_cyclic, this );
}

Motor::~Motor()
{
	stop();
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
	return output;
}

void Motor::thread_cyclic()
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	double pwm_time = 1.0f / _freq;		//period time

	running = true;

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
			digitalWrite( _pwmpin, HIGH );
			std::this_thread::sleep_for( total_us );
			continue;
		}

		if( dir )
			digitalWrite( _dirpin, HIGH );
		else
			digitalWrite( _dirpin, LOW );


		digitalWrite( _pwmpin, LOW );
		std::this_thread::sleep_for( off_us );

		digitalWrite( _pwmpin, HIGH );
		std::this_thread::sleep_for( total_us - off_us );
	}
}

void Motor::stop()
{
	encoder->stop();

	digitalWrite( _pwmpin, HIGH );

	if( running )
	{
		running = false;
		thread.join();
	}
}
