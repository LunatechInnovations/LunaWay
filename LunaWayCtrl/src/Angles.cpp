/*
 * Angles.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: john
 */

#include "Angles.h"
#include <cmath>
#include <string>
#include <iostream>


Angles::Angles()
{
	sensors = MPU6050( 0x69 );
	pitch = 0.0f;
	roll = 0.0f;
	debug = 0.0f;
	pitch_gyro_rate = 0.0f;

	prev_calculate = std::chrono::high_resolution_clock::now();

	sensors.initialize();

	if( !sensors.testConnection() )
		throw std::string( "MPU6050 connection failed" );
}

Angles::~Angles()
{
}

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

	debug = acc_pitch;

	pitch_gyro_rate = ((double)gyroY / (32768.0f))*250.0f;
//	double gyro_roll_rate = ((double)gyroZ / (32768.0f))*250.0f;

	pitch = fc * (pitch + (pitch_gyro_rate * (dT.count() / 1000.0f)))+ ((1.0f - fc) * acc_pitch);
//	roll = 0.98 * (roll + (gyro_roll_rate * (dT.count() / 1000.0f)))+ (0.02f * acc_roll);

	prev_calculate = current_time;
}

double Angles::getPitch()
{
	return pitch;
}

double Angles::getRoll()
{
	return roll;
}

double Angles::getDebug()
{
	return debug;
}

double Angles::getPitchGyroRate()
{
	return pitch_gyro_rate;
}
