/*
 * Angles.h
 *
 *  Created on: Feb 8, 2014
 *      Author: john
 */

#ifndef ANGLES_H_
#define ANGLES_H_
#include "MPU6050.h"
#include <chrono>

class Angles
{
	public:
		Angles();
		virtual ~Angles();
		void calculate();
		double getPitch();
		double getRoll();
		double getDebug();
		double getPitchGyroRate();

	private:
		MPU6050 sensors;
		double pitch;
		double roll;
		std::chrono::high_resolution_clock::time_point prev_calculate;
		double debug;
		double pitch_gyro_rate;
};

#endif /* ANGLES_H_ */
