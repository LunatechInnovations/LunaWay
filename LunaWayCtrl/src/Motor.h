/*
 * Motor.h
 *
 *  Created on: Feb 2, 2014
 *      Author: john
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <thread>
#include <mutex>
#include "Encoder.h"

class Motor
{
	public:
		Motor( int dirpin, int pwmpin, int encoderpin, int freq );
		virtual ~Motor();
		void stop();
		void setOutput( double value );
		double getRPS();
		double getOutput();

	private:
		int _dirpin;
		int _pwmpin;
		void thread_cyclic();
		std::thread thread;
		volatile bool running;
		double output;
		double _freq;
		std::mutex output_mutex;
		Encoder *encoder;
};

#endif /* MOTOR_H_ */
