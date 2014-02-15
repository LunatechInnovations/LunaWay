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
#include "GPIOPin.h"

class Motor : public AbstractCyclicThread
{
	public:
		Motor();
		Motor( GPIOPin *dir = nullptr, GPIOPin *pwm = nullptr, int encoderpin = -1, int freq = 0 );
		virtual ~Motor();
		void setOutput( double value );
		double getRPS();
		double getOutput();
		void stop();

	private:
		GPIOPin *_dir, *_pwm;
		void cyclic();
		double output;
		double _freq;
		std::mutex output_mutex;
		Encoder *encoder;
};

#endif /* MOTOR_H_ */
