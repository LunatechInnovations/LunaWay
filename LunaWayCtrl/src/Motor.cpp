/*
 * Motor.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: john
 */

#include "Motor.h"
#include <chrono>
#include <cmath>
#include <iostream>

using namespace std::chrono;
Motor::Motor() : outp_pwm( nullptr ), outp_dir( nullptr ), output( 0.0f ), _freq( 0.0f ), encoder( nullptr )
{
}

Motor::Motor( int dirpin, int pwmpin, int encoderpin, int freq )
	  : _freq( freq )
{
	output = 0.0f;
	running = false;

	outp_pwm = new GPIO( pwmpin );
	outp_pwm->setupDir( GPIO::Output );
	outp_dir = new GPIO( dirpin );
	outp_dir->setupDir( GPIO::Output );

	encoder = new Encoder( encoderpin );

	start();
}

Motor::~Motor()
{
	delete outp_pwm;
	delete outp_dir;
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
//			outp_pwm->setValue( true );
			outp_pwm->setValue( false );
			std::this_thread::sleep_for( total_us );
			continue;
		}

		if( dir )
//			outp_dir->setValue( true );
			outp_dir->setValue( false );
		else
			outp_dir->setValue( true );
//			outp_dir->setValue( false );


		outp_pwm->setValue( false );
		std::this_thread::sleep_for( off_us );

		outp_pwm->setValue( true );
		std::this_thread::sleep_for( total_us - off_us );
	}
}

void Motor::stop()
{
	encoder->stop();
//	outp_pwm->setValue( true );
	outp_pwm->setValue( false );

	AbstractCyclicThread::stop();
}
