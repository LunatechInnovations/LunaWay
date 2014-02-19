/* main.cpp

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

//#define DEBUG
#undef DEBUG

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <sstream>
#include "Angles.h"
#include "PID.h"
#include <chrono>
#include <thread>
#include <system_error>
#include "GPIO.h"
#include "Segway.h"
#include "XBoxCtrlServer.h"

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


/*Program parameters
 *
 */
//sample time = reg interval = 20mS
#define START_P 27.0f					//Start value for p gain in PID regulator
#define START_I 0.0f					//Start value for i gain in PID regulator
#define START_D 0.05f					//Start value for d gain in PID regulator
#define START_IRANGE 30					//Min and max value for i state in PID regulator
#define START_SV 0.0f					//Start set value
#define SAMPLE_TIME 20					//The rate of which to fetch new angle values. Time in Ms
#define REG_INTERVAL 20 / SAMPLE_TIME	//The interval of witch the regulator calculates new output. Time in n SAMPLE_TIME
#define START_DIFF_P 3					//Start value for p gain in diff

#ifdef DEBUG
#define PLOT_INTERVAL 20 / SAMPLE_TIME  //The interval of sending plot data. Time in n SAMPLE_TIME
#endif

#define LEFT_MOTOR_PWM_PIN 		8
#define LEFT_MOTOR_DIR_PIN 		7
#define RIGHT_MOTOR_PWM_PIN 	10
#define RIGHT_MOTOR_DIR_PIN 	9
#define ENABLE_SWITCH_PIN 		4
#define INTERRUPT_LEFT_ENCODER 	11
#define INTERRUPT_RIGHT_ENCODER 17
#define MOTOR_PWM_FREQUENCY 	500

using namespace std;
using namespace chrono;

/* Global variables
 *
 */
volatile bool g_running = true;

/* signal_callback
 * Attatched to SIGINT and SIGTERM
 * Break main loop
 */
void signal_callback( int )
{
	g_running = false;
	cout << endl;
}

/* main
 *
 */
int main()
{
	try
	{
		//Initiate system log
		openlog( "LunaWayCtrl", LOG_PID | LOG_CONS | LOG_NDELAY | LOG_PERROR, LOG_LOCAL1 );
		syslog( LOG_INFO, "Application started" );

		//Make sure we are root
		if( getuid() != 0 )
			throw string( "You have to be root to play with the LunaWay." );

		//Setup signal callback
		signal( SIGINT, signal_callback );
		signal( SIGTERM, signal_callback );
		milliseconds sample_time( SAMPLE_TIME );

		//Set scheduler priority
		sched_param sp;
		sp.__sched_priority = 49;
		if( sched_setscheduler( 0, SCHED_FIFO, &sp ) == -1 )
			throw string( "Failed to set scheduler." );

		PID pid( START_P, START_I, START_D, START_SV, -START_IRANGE, START_IRANGE );

		//Create gpio object
		GPIO *gpio = GPIO::Instance();

		//Setup enable switch
		GPIOPin *inpEnable = gpio->getPin( ENABLE_SWITCH_PIN );
		inpEnable->setupInput();

		//Setup motors (Pins will outputs inside Motor object)
		Motor leftMotor( gpio->getPin( LEFT_MOTOR_PWM_PIN ),
						 gpio->getPin( LEFT_MOTOR_DIR_PIN ),
						 gpio->getPin( INTERRUPT_LEFT_ENCODER ),
						 MOTOR_PWM_FREQUENCY );
		Motor rightMotor( gpio->getPin( RIGHT_MOTOR_PWM_PIN ),
						  gpio->getPin( RIGHT_MOTOR_DIR_PIN ),
						  gpio->getPin( INTERRUPT_RIGHT_ENCODER ),
						  MOTOR_PWM_FREQUENCY );

		//Setup segway
		Segway segway( &leftMotor, &rightMotor, START_DIFF_P );

		//Setup sensors
		Angles angles;

		XBoxCtrlServer xcs( 2345 );
		xcs.connect();

#ifdef DEBUG
		SegwayPlotterCom spc( &pid );
		if( !spc.conn( "192.168.0.137", 5555 ) )
			throw string( "Failed to connect" );
		int plot_interval = 1;
#endif

		//Don't swap memory. Start all threads before this.
		if( (mlockall( MCL_CURRENT|MCL_FUTURE )) == -1 )
			throw string( "mlockall failed." );

		int reg_interval = 1;

		while( g_running )
		{
			high_resolution_clock::time_point start_time = high_resolution_clock::now();

			if( !inpEnable->getValue() )
				segway.update( 0.0f, 0.0f );
			else
			{
				angles.calculate();
				if( reg_interval >= REG_INTERVAL )
				{
					pid.setSV( START_SV - (xcs.getVertical() * 0.05f) );
					segway.update( pid.regulate( angles.getPitch(), angles.getPitchGyroRate() ),
								   xcs.getHorizontal() );
					reg_interval = 1;
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
				sockdata << angles.getPitch() << ";" << segway.getLeftMotorOutput() << ";" << endl;
				spc.sendData( sockdata.str() );

				plot_interval = 1;
			}
			else
			{
				plot_interval++;
			}
#endif

			this_thread::sleep_until( start_time + sample_time );
		}

		//Join all threads
#ifdef DEBUG
		spc.stop();
#endif
		segway.stopMotors();
	}
	/* Handle exceptions
	 *
	 */
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

	//Final log entry
	syslog( LOG_INFO, "Application terminated" );
	closelog();

	return 0;
}
