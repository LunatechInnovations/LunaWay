/* PID.cpp

Copyright (C) 2014 Lunatech Innovations

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "PID.h"
#include <iostream>

/*! \fn PID::PID()
 * \brief Generic constructor
 */
PID::PID() : _p( 0.0f ), _i( 0.0f ), _d( 0.0f ), _sv( 0.0f ), _istate( 0.0f ), _istate_max( 250.0f ), _istate_min( -250.0f )
{
}

/*! \fn PID::PID( double p, double i, double d, double sv )
 * \brief Constructor with regulator parameters and set value
 * @param[in] p P-gain
 * @param[in] i I-gain
 * @param[in] d D-gain
 * @param[in] sv Set value
 */
PID::PID( double p, double i, double d, double sv )
	: _p( p ), _i( i ), _d( d ), _sv( sv ), _istate( 0.0f ), _istate_max( 250.0f ), _istate_min( -250.0f )
{
}

/*! \fn PID::PID( double p, double i, double d, double sv, double i_state_min, double i_state_max )
 * \brief Constructor with regulator parameters, set value and istate limits
 * @param[in] p P-gain
 * @param[in] i I-gain
 * @param[in] d D-gain
 * @param[in] sv Set value
 * @param[in] i_state_min Minimum value for istate
 * @param[in] i_state_max Maximum value for istate
 */
PID::PID( double p, double i, double d, double sv, double i_state_min, double i_state_max )
    : _p( p ), _i( i ), _d( d ), _sv( sv ), _istate( 0.0f ), _istate_max( i_state_max ), _istate_min( i_state_min )
{
}

/*! \fn PID::~PID()
 * \brief Destructor
 */
PID::~PID()
{
}

/*! \fn PID::setP( double p )
 * \brief Setter for P-gain
 * @param[in] p New P-gain
 */
void PID::setP( double p )
{
	p_mutex.lock();
	_p = p;
	p_mutex.unlock();
}

/*! \fn PID::setI( double i )
 * \brief Setter for I-gain
 * @param[in] i New I-gain
 */
void PID::setI( double i )
{
	i_mutex.lock();
	_i = i;
	i_mutex.unlock();
}

/*! \fn PID::setD( double d )
 * \brief Setter for D-gain
 * @param[in] d New D-gain
 */
void PID::setD( double d )
{
	d_mutex.lock();
	_d = d;
	d_mutex.unlock();
}

/*! \fn PID::setMaxIState( double max )
 * \brief Setter for maximum istate value
 * @param[in] max New max value
 */
void PID::setMaxIState( double max )
{
	_istate_max = max;
}

/*! \fn PID::setMinIState( double min )
 * \brief Setter for minimum istate value
 * @param[in] min New min value
 */
void PID::setMinIState( double min )
{
	_istate_min = min;
}

/*! \fn PID::setIStateLimits( double min, double max )
 * \brief Setter for both maximum and minimum istate values
 * @param[in] min New min value
 * @param[in] max New max value
 */
void PID::setIStateLimits( double min, double max )
{
	_istate_max = max;
	_istate_min = min;
}

/*! \fn PID::regulate( double pv, double gyro_rate )
 * \brief Calculate new ouput
 * @param[in] pv Current process value
 * @param[in] gyro_rate Gyro rate from sensor. Used for derivative term
 */
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

/*! \fn PID::setSV( double sv )
 * \brief Setter for set value
 * @param[in] sv New set value
 */
void PID::setSV( double sv )
{
	sv_mutex.lock();
	_sv = sv;
	sv_mutex.unlock();
}

/*! \fn PID::getSV()
 * \brief Getter for set value
 */
double PID::getSV()
{
	return _sv;
}
