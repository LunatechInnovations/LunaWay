/*
 * Diff.cpp
 *
 *  Created on: Feb 12, 2014
 *      Author: john
 */

#include "Diff.h"

Diff::Diff( int l_pwm_pin, int l_dir_pin, int l_enc_pin, int r_pwm_pin,
		    int r_dir_pin, int r_enc_pin, double freq, double p ) : _p( p )
{
	leftMotor = new Motor( l_dir_pin, l_pwm_pin, l_enc_pin, freq );
	rightMotor = new Motor( r_dir_pin, r_pwm_pin, r_enc_pin, freq );

}

Diff::~Diff()
{
	delete leftMotor;
	delete rightMotor;
}

void Diff::setOutput( double output )
{
	double cleft, cright;
	double dRps = leftMotor->getRPS() - rightMotor->getRPS();

	double p_term = dRps * _p;


	if( p_term > output )
	{
		cleft = output * 2.0f;
		cright = 0.0f;
	}
	else if( p_term < output && p_term > 0.0f )
	{
		cleft = output + p_term;
		cright = output - p_term;
	}
	else if( p_term < output && p_term < 0.0f )
	{
		cleft = 0.0f;
		cright = output * 2.0f;
	}
	else
	{
		cleft = output;
		cright = output;
	}

	leftMotor->setOutput( cleft );
	rightMotor->setOutput( cright );
}

double Diff::getLeftMotorOutput()
{
	return leftMotor->getOutput();
}

double Diff::getRightMotorOutput()
{
	return rightMotor->getOutput();
}

void Diff::stopMotors()
{
	leftMotor->stop();
	rightMotor->stop();
}
