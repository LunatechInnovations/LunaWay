//============================================================================
// Name        : LunaWayCtrl.cpp
// Author      : John Erlandsson
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define DEBUG
//#undef DEBUG

#include <iostream>
#include <vector>
#include <wiringPi.h>
#include "Switch.h"
#include <iomanip>
#include <cmath>
#include <sstream>
#include "Angles.h"
#include "PID.h"
#include <chrono>
#include <thread>
#include <system_error>
#include "Diff.h"

#ifdef DEBUG
#include "SegwayPlotterCom.h"
#endif

extern "C"
{
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <sys/mman.h>
#include <syslog.h>
}

//sample time = reg interval = 20mS

#define START_P 27.0f
#define START_I 0.0f
#define START_D 0.05f
#define START_IRANGE 30
#define START_SV 0.0f
#define SAMPLE_TIME 20	//mS
#define REG_INTERVAL 40 / SAMPLE_TIME

#ifdef DEBUG
#define PLOT_INTERVAL 20 / SAMPLE_TIME
#endif

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
using namespace chrono;

volatile bool g_running = true;

void signal_callback( int )
{
	g_running = false;
}

int main()
{
	try
	{
		//Initiate system log
		openlog( "LunaWayCtrl", LOG_PID | LOG_CONS | LOG_NDELAY | LOG_PERROR, LOG_LOCAL1 );
		syslog( LOG_INFO, "Application started" );

		//TODO loose wiringpi dependency
		if( wiringPiSetup() == -1 )
			throw string( "Failed to setup wiringpi." );

		signal( SIGINT, signal_callback );
		signal( SIGTERM, signal_callback );
		milliseconds cycle_time( SAMPLE_TIME );

		//Set scheduler priority
		sched_param sp;
		sp.__sched_priority = 49;
		if( sched_setscheduler( 0, SCHED_FIFO, &sp ) == -1 )
			throw string( "Failed to set scheduler." );

		PID pid( START_P, START_I, START_D, START_SV, -START_IRANGE, START_IRANGE );

#ifdef DEBUG
		SegwayPlotterCom spc( &pid );
		if( !spc.conn( "192.168.0.137", 5555 ) )
			throw string( "Failed to connect" );
		int plot_interval = 0;
#endif

		Diff motor_diff;

		//Don't swap memory. Start all threads before this.
		if( (mlockall( MCL_CURRENT|MCL_FUTURE )) == -1 )
			throw string( "mlockall failed." );

		Switch enableSwitch( 7 );
		Angles angles;

		int reg_interval = 0;

		while( g_running )
		{
			high_resolution_clock::time_point start_time = high_resolution_clock::now();

			if( !enableSwitch.getValue() )
			{
				motor_diff.setOutput( 0.0f );
			}
			else
			{
				/* cycle_time=20
				 * p = 27.5
				 * I = 0.0
				 * gyro_rate * .1
				 */
				angles.calculate();
				if( reg_interval >= REG_INTERVAL )
				{
					double output = pid.regulate( angles.getPitch(), angles.getPitchGyroRate() );

					motor_diff.setOutput( output );

					reg_interval = 0;
				}
				else
				{
					reg_interval++;
				}
			}

#ifdef DEBUG
			//Send debug values
			if( plot_interval >= PLOT_INTERVAL )
			{
				stringstream sockdata;
				sockdata << angles.getPitch() << ";" << motor_diff.getLeftMotorOutput() << ";" << endl;
				spc.sendData( sockdata.str() );

				plot_interval = 0;
			}
			else
			{
				plot_interval++;
			}
#endif

			this_thread::sleep_until( start_time + cycle_time );
		}

		//Join all threads
#ifdef DEBUG
		spc.stop();
#endif
		motor_diff.stopMotors();
	}
	catch( const string &e )
	{
		syslog( LOG_ERR, e.c_str() );
		closelog();

		return 1;
	}
	catch( const system_error &e )
	{
		syslog( LOG_ERR, string( string( "System error: " ) + e.what() ).c_str() );
		closelog();

		return 2;
	}
	catch( ... )
	{
		syslog( LOG_ERR, "Undefined error" );
		closelog();

		return 3;
	}

	syslog( LOG_INFO, "Application terminated" );
	closelog();

	return 0;
}
