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
		Diff( int l_pwm_pin, int l_dir_pin, int l_enc_pin, int r_pwm_pin, int r_dir_pin, int r_enc_pin, double freq, double p );
		virtual ~Diff();
		void setOutput( double output );
		double getLeftMotorOutput();
		double getRightMotorOutput();
		void stopMotors();

	private:
		Motor *leftMotor;
		Motor *rightMotor;
		double _p;
};

#endif /* DIFF_H_ */
