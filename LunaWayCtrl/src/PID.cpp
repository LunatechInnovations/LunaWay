/*
 * PID.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: john
 */

#include "PID.h"
#include <iostream>

PID::PID() : _p( 0.0f ), _i( 0.0f ), _d( 0.0f ), _istate( 0.0f ), _istate_max( 250.0f ), _istate_min( -250.0f )
{
}

PID::PID( double p, double i, double d )
	: _p( p ), _i( i ), _d( d ), _istate( 0.0f ), _istate_max( 250.0f ), _istate_min( -250.0f )
{
}

PID::PID( double p, double i, double d, double i_state_min, double i_state_max )
    : _p( p ), _i( i ), _d( d ), _istate( 0.0f ), _istate_max( i_state_max ), _istate_min( i_state_min )
{
}

PID::~PID()
{
}

void PID::setP( double p )
{
	_p = p;
}

void PID::setI( double i )
{
	_i = i;
}

void PID::setD( double d )
{
	_d = d;
}

void PID::setMaxIState( double max )
{
	_istate_max = max;
}

void PID::setMinIState( double min )
{
	_istate_min = min;
}

void PID::setIStateLimits( double min, double max )
{
	_istate_max = max;
	_istate_min = min;
}

double PID::regulate( double error, double gyro_rate )
{
	double p_term = error * _p;

	_istate += error;
	if( _istate > _istate_max )
		_istate = _istate_max;
	else if( _istate < _istate_min )
		_istate = _istate_min;
	double i_term = _i * _istate;

	double d_term = gyro_rate * _d;

	double output = p_term + i_term + d_term;
	if( output > 100.0f )
		output = 100.0f;
	else if( output < -100.0f )
		output = -100.0f;

	return output;
}
