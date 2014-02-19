/* Angles.cpp

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

#include "Angles.h"
#include <cmath>
#include <string>
#include <iostream>

/** Angles
 *
 * Default constructor
 */
Angles::Angles()
{
	sensors = MPU6050( 0x69 );
	pitch = 0.0f;
	roll = 0.0f;
	pitch_gyro_rate = 0.0f;

	prev_calculate = std::chrono::high_resolution_clock::now();

	sensors.initialize();

	if( !sensors.testConnection() )
		throw std::string( "MPU6050 connection failed" );
}

/** ~Angles
 *
 * Generic destructor
 */
Angles::~Angles()
{
}

/** calculate
 *
 * Fetch raw sensor data from MPU6050.
 * Calculate angle and angular rate.
 */
void Angles::calculate()
{
	int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;
	const double fc = 0.98f;
	std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds dT = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_calculate);

	//Read sensor values
	sensors.getMotion6( &accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ );

	//Calculate angles
	double acc_pitch = atan2( accZ, accX );
	double acc_roll = atan2( accY, accZ );

	//Radians to degrees
	acc_pitch = (acc_pitch * (180.0f / M_PI ) );
	acc_roll = (acc_roll * (180.0f / M_PI ) );

	pitch_gyro_rate = ((double)gyroY / (32768.0f))*250.0f;
//	double gyro_roll_rate = ((double)gyroZ / (32768.0f))*250.0f;

	pitch = fc * (pitch + (pitch_gyro_rate * (dT.count() / 1000.0f)))+ ((1.0f - fc) * acc_pitch);
//	roll = 0.98 * (roll + (gyro_roll_rate * (dT.count() / 1000.0f)))+ (0.02f * acc_roll);

	prev_calculate = current_time;
}

/** getPitch
 *
 * getter for pitch angle
 */
double Angles::getPitch()
{
	return pitch;
}

/** getRoll
 *
 * getter for roll angle
 */
double Angles::getRoll()
{
	return roll;
}

/** getPitchGyroRate
 *
 * getter for pitch rate
 */
double Angles::getPitchGyroRate()
{
	return pitch_gyro_rate;
}
