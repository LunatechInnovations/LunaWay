/*
 * Diff.h
 *
 *  Created on: Feb 12, 2014
 *      Author: john
 */

#ifndef DIFF_H_
#define DIFF_H_

#include "Motor.h"

class Diff
{
	public:
		Diff();
		virtual ~Diff();
		void setOutput( double output );
		double getLeftMotorOutput();
		double getRightMotorOutput();
		void stopMotors();

	private:
		Motor leftMotor;
		Motor rightMotor;
		double _p;
};

#endif /* DIFF_H_ */
