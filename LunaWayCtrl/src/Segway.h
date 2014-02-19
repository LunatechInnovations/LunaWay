/* Segway.h

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

#ifndef SEGWAY_H_
#define SEGWAY_H_

#include "Motor.h"

/*! \class Segway
 * \brief This class is responsible for distributing the PID regulator output to the motors.\n
 * \details It also handles steering
 */
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
