/*
 * Diff.cpp
 *
 *  Created on: Feb 12, 2014
 *      Author: john
 */

#include "Diff.h"

Diff::Diff() : _p( 0.0f )
{
	Motor leftMotor( 10, 11, 11, 500.0f );
	Motor rightMotor( 12, 13, 17, 500.0f );
}

Diff::~Diff()
{
}

void Diff::setOutput( double output )
{
	double cleft, cright;
	double dRps = leftMotor.getRPS() - rightMotor.getRPS();

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

	leftMotor.setOutput( cleft );
	rightMotor.setOutput( cright );
}

double Diff::getLeftMotorOutput()
{
	return leftMotor.getOutput();
}

double Diff::getRightMotorOutput()
{
	return rightMotor.getOutput();
}

void Diff::stopMotors()
{
	leftMotor.stop();
	rightMotor.stop();
}
