/*
 * Segway.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: john
 */

#include "Segway.h"
#include <iostream>
#include <iomanip>

Segway::Segway( Motor* leftMotor, Motor* rightMotor ) : _leftMotor( leftMotor ), _rightMotor( rightMotor ), _p( 1.0f )
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
	if( output > 100.0f )
		output = 100.0f;
	else if( output < -100.0f )
		output = -100.0f;

	static int tmp = 0;

	double cleft = output;
	double cright = output;

	double lrps = _leftMotor->getRPS();
	double rrps = _rightMotor->getRPS();

	//Differential
	double dRps = rrps - lrps + steering * 0.2f;
	double p_term = dRps * _p;

	if( output > 0.0f && dRps >= 0.0f ) 		//Moving forward, rightMotor slow
	{
		cright += p_term;
		cleft -= p_term;

		if( cright > 100.0f )
		{
			cleft -= (cright - 100.0f);
			cright -= (cright - 100.0f);
		}
	}
	else if( output > 0.0f && dRps < 0.0f )		//Moving forward, leftMotor slow
	{
		cleft -= p_term;
		cright += p_term;

		if( cleft > 100.0f )
		{
			cright -= (cleft - 100.0f);
			cleft -= (cleft - 100.0f);
		}
	}
	else if( output < 0.0f && dRps >= 0.0f )	//Moving back, rightMotor slow
	{
		cright -= p_term;
		cleft += p_term;

		if( cright < -100.0f )
		{
			cleft -= (cright + 100.0f);
			cright -= (cright + 100.0f);
		}
	}
	else if( output < 0.0f && dRps < 0.0f )		//Moving back, leftMotor slow
	{
		cright -= p_term;
		cleft += p_term;

		if( cleft < -100.0f )
		{
			cright -= (cleft + 100.0f);
			cleft -= (cleft + 100.0f);
		}
	}


	_leftMotor->setOutput( cleft );
	_rightMotor->setOutput( cright );

	if( tmp == 8 )
	{
		std::cout << "L: " << std::setw( 7 ) << cleft
				  << " R: " << std::setw( 7 ) << cright
				  << " p_term: " << std::setw( 7 ) << p_term << std::endl;
		tmp = 0;
	}
	else
	{
		tmp++;
	}
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
