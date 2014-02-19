/* Angles.h

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

#ifndef ANGLES_H_
#define ANGLES_H_
#include "MPU6050.h"
#include <chrono>

/*! \class Angles
 * \brief Convert raw MPU6050 sensor data to angles and rates
 */
class Angles
{
	public:
		Angles();
		virtual ~Angles();
		void calculate();
		double getPitch();
		double getRoll();
		double getPitchGyroRate();

	private:
		MPU6050 sensors;
		double pitch;
		double roll;
		std::chrono::high_resolution_clock::time_point prev_calculate;
		double pitch_gyro_rate;
};

#endif /* ANGLES_H_ */
