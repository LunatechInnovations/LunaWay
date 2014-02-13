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
#include "GPIO.h"

class Motor : public AbstractCyclicThread
{
	public:
		Motor();
		Motor( int dirpin, int pwmpin, int encoderpin, int freq );
		virtual ~Motor();
		void setOutput( double value );
		double getRPS();
		double getOutput();
		void stop();

	private:
		GPIO *outp_pwm, *outp_dir;
		void cyclic();
		double output;
		double _freq;
		std::mutex output_mutex;
		Encoder *encoder;
};

#endif /* MOTOR_H_ */
