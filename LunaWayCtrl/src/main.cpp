//============================================================================
// Name        : LunaWayCtrl.cpp
// Author      : John Erlandsson
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <wiringPi.h>
#include "Encoder.h"
#include "Motor.h"
#include "Switch.h"
#include "MPU6050.h"
#include <iomanip>
#include <cmath>
#include "SegwayPlotterCom.h"
#include <sstream>

extern "C"
{
#include <unistd.h>
#include <signal.h>
}

/* Wiringpi	GPIO	Physical
 * 0		17		11		interrupt left encoder
 * 1		18		12
 * 2		21		13
 * 3		22		15
 * 4		23		16
 * 5		24		18
 * 6		25		22
 * 7		4		7		enable switch
 * 8		0		3
 * 9		11		23 		interrupt right encoder
 * 10		8		24		Dir left motor
 * 11		7		26		PWM left motor
 * 12		10		19		Dir right motor
 * 13		9		21		PWM right motor
 */

using namespace std;

MPU6050 g_accelgyro( 0x69 );
/* alpha = accelerometer fw/back
 * alphadot = gyro fw/back
 * beta = accelerometer right/left
 * betadot = gyro right/left
 */

double g_alpha, g_alphadot, g_beta, g_betadot;

void read_sensors()
{
	int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;
	g_accelgyro.getMotion6( &accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ );

	if( accZ > 16383 )
		accZ = 16383;
	if( accY > 16383 )
		accY = 16383;
	if( accX > 16383 )
		accX = 16383;

	g_alphadot = ((double)gyroY / 250.0f) * (M_PI / 180.0f);
	g_betadot = ((double)gyroZ / 250.0f) * (M_PI / 180.0f);

	g_alpha = asin( (double)accZ / 16383.75f );
	g_beta = asin( (double)accY / 16383.75f );
}

int main()
{
	if( wiringPiSetup() == -1 )
	{
		cerr << "wiringPiSetup failed!" << endl;
		return 1;
	}

	try
	{
		SegwayPlotterCom spc;
		if( spc.conn( "192.168.0.137", 5555 ) )
		{
			cerr << "Failed to connect" << endl;
		}

		Encoder leftEncoder( 11 );
		Encoder rightEncoder( 17 );
		Motor rightMotor( 12, 13, 100.0f );
		Motor leftMotor( 10, 11, 100.0f );

		rightMotor.setOutput( 70.0f );
		leftMotor.setOutput( -70.0f );

		Switch enableSwitch( 7 );

		g_accelgyro.initialize();

		if( !g_accelgyro.testConnection() )
		{
			cerr << "I2C Error" << endl;
			return 1;
		}

		while( true )
		{
			usleep( 100000 );

			if( !enableSwitch.getValue() )
			{
				leftMotor.setOutput( 0.0f );
				rightMotor.setOutput( 0.0f );

				continue;
			}

			read_sensors();
			stringstream sockdata;
			sockdata << g_beta << ';' << g_betadot << ';' << g_alpha << ';' << g_alphadot << ';';
			spc.sendData( sockdata.str() );
		}
	}
	catch( string &e )
	{
		cout << "Error: " << e << endl;
		return 1;
	}
	catch( ... )
	{
		cout << "Undefined error." << endl;
		return 1;
	}

	return 0;
}
