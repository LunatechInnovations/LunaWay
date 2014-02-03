/*
 * Motor.h
 *
 *  Created on: Feb 2, 2014
 *      Author: john
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#ifndef _GLIBCXX_USE_NANOSLEEP
#define _GLIBCXX_USE_NANOSLEEP
#endif

#include <thread>
#include <mutex>

class Motor
{
	public:
		Motor( int dirpin, int pwmpin, int freq );
		virtual ~Motor();
		void stop();
		void setOutput( double value );

	private:
		int _dirpin;
		int _pwmpin;
		void thread_cyclic();
		std::thread thread;
		volatile bool running;
		double output;
		double _freq;
		std::mutex output_mutex;
};

#endif /* MOTOR_H_ */
