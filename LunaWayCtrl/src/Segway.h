/*
 * Segway.h
 *
 *  Created on: Feb 15, 2014
 *      Author: john
 */

#ifndef SEGWAY_H_
#define SEGWAY_H_

#include "Motor.h"

class Segway
{
	public:
		Segway( Motor *leftMotor, Motor *rightMotor );
		Segway( Motor *leftMotor, Motor *rightMotor, double p_gain );
		virtual ~Segway();
		void update( double output, double steering );
		void stopMotors();
		double getLeftMotorOutput();
		double getRightMotorOutput();

	private:
		Motor *_leftMotor, *_rightMotor;
		double _p;
};

#endif /* SEGWAY_H_ */
