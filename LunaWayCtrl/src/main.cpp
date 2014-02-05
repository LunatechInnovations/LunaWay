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

enum Outputs
{
	OutpPwmLeft = 26,
	OutpPwmRight = 23,
	OutpDirLeft = 24,
	OutpDirRight = 21
};

using namespace std;

int main()
{
	if( wiringPiSetup() == -1 )
	{
		cerr << "wiringPiSetup failed!" << endl;
		return 1;
	}

	try
	{
		Encoder leftEncoder( 11 );
		Encoder rightEncoder( 17 );
		Motor rightMotor( 12, 13, 100.0f );
		Motor leftMotor( 10, 11, 100.0f );

		rightMotor.setOutput( 70.0f );
		leftMotor.setOutput( -70.0f );

		Switch enableSwitch( 7 );

		while( true )
		{
//			cout << leftEncoder.getRps() << '\t' << rightEncoder.getRps() << endl;
			sleep( 1 );
			cout << (int)enableSwitch.getValue() << endl;
//			double newoutp = 0.0f;
//			cout << "Output: ";
//			cin >> newoutp;
//			cout << "New value: " << newoutp << endl;
//			rightMotor.setOutput( newoutp );
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
