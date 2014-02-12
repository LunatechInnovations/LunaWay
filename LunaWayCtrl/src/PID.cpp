/*
 * PID.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: john
 */

#include "PID.h"
#include <iostream>

PID::PID() : _p( 0.0f ), _i( 0.0f ), _d( 0.0f ), _sv( 0.0f ), _istate( 0.0f ), _istate_max( 250.0f ), _istate_min( -250.0f )
{
}

PID::PID( double p, double i, double d, double sv )
	: _p( p ), _i( i ), _d( d ), _sv( sv ), _istate( 0.0f ), _istate_max( 250.0f ), _istate_min( -250.0f )
{
}

PID::PID( double p, double i, double d, double sv, double i_state_min, double i_state_max )
    : _p( p ), _i( i ), _d( d ), _sv( sv ), _istate( 0.0f ), _istate_max( i_state_max ), _istate_min( i_state_min )
{
}

PID::~PID()
{
}

void PID::setP( double p )
{
	p_mutex.lock();
	_p = p;
	p_mutex.unlock();
}

void PID::setI( double i )
{
	i_mutex.lock();
	_i = i;
	i_mutex.unlock();
}

void PID::setD( double d )
{
	d_mutex.lock();
	_d = d;
	d_mutex.unlock();
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

double PID::regulate( double pv, double gyro_rate )
{
	sv_mutex.lock();
	double error = pv - _sv;
	sv_mutex.unlock();

	p_mutex.lock();
	double p_term = error * _p;
	p_mutex.unlock();

	_istate += error;
	if( _istate > _istate_max )
		_istate = _istate_max;
	else if( _istate < _istate_min )
		_istate = _istate_min;
	i_mutex.lock();
	double i_term = _i * _istate;
	i_mutex.unlock();

	d_mutex.lock();
	double d_term = gyro_rate * _d;
	d_mutex.unlock();

	double output = p_term + i_term + d_term;
	if( output > 100.0f )
		output = 100.0f;
	else if( output < -100.0f )
		output = -100.0f;

	return output;
}

void PID::setSV( double sv )
{
	sv_mutex.lock();
	_sv = sv;
	sv_mutex.unlock();
}
