/*
 * Segway.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: john
 */

#include "Segway.h"
#include <iostream>

Segway::Segway( Motor* leftMotor, Motor* rightMotor ) : _leftMotor( leftMotor ), _rightMotor( rightMotor ), _p( 0.0f )
{
}

Segway::Segway( Motor* leftMotor, Motor* rightMotor, double p_gain ) : _leftMotor( leftMotor ), _rightMotor( rightMotor ), _p( p_gain )
{
}

Segway::~Segway()
{
	stopMotors();
}

void Segway::update( double output, double steering )
{
	double cleft, cright;
	double dRps = _leftMotor->getRPS() - _rightMotor->getRPS();

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

	std::cout << cleft << '\t' << cright << std::endl;

	_leftMotor->setOutput( cleft );
	_rightMotor->setOutput( cright );
}

void Segway::stopMotors()
{
	_leftMotor->stop();
	_rightMotor->stop();
}

double Segway::getLeftMotorOutput()
{
	return _leftMotor->getOutput();
}

double Segway::getRightMotorOutput()
{
	return _rightMotor->getOutput();
}
