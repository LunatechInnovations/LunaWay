/*
 * Motor.h
 *
 *  Created on: Feb 2, 2014
 *      Author: john
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <mutex>
#include "Encoder.h"
#include "AbstractCyclicThread.h"

class Motor : public AbstractCyclicThread
{
	public:
		Motor( int dirpin, int pwmpin, int encoderpin, int freq );
		virtual ~Motor();
		void setOutput( double value );
		double getRPS();
		double getOutput();
		void stop();

	private:
		int _dirpin;
		int _pwmpin;
		void cyclic();
		double output;
		double _freq;
		std::mutex output_mutex;
		Encoder *encoder;
};

#endif /* MOTOR_H_ */
