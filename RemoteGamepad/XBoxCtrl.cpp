/*
 * XBoxCtrl.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: john
 */

#include "XBoxCtrl.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

extern "C"
{
#include <fcntl.h>
#include <unistd.h>
}

#define TYPE_BUTTON_PRESSED 0x01
#define TYPE_JOYSTICK_MOVED 0x02
#define NUM_BUTT_A 			0x00
#define NUM_BUTT_B			0x01
#define NUM_BUTT_X			0x02
#define NUM_BUTT_Y			0x03
#define NUM_BUTT_LB			0x04
#define NUM_BUTT_RB			0x05
#define NUM_BUTT_BACK		0x06
#define NUM_BUTT_START		0x07
#define NUM_BUTT_MENU 		0x08
#define NUM_BUTT_LEFT_JS 	0x09
#define NUM_BUTT_RIGHT_JS 	0x0A

#define NUM_JS_L_HOR		0x00
#define NUM_JS_L_VER		0x01
#define NUM_JS_LT			0x02
#define NUM_JS_R_HOR		0x03
#define NUM_JS_R_VER		0x04
#define NUM_JS_RT			0x05
#define NUM_DP_HOR			0x06
#define NUM_DP_VER			0x07

XBoxCtrl::XBoxCtrl()
{
	pfd.fd = open( "/dev/input/js0", O_RDONLY | O_NONBLOCK );
	if( pfd.fd < 0 )
		throw std::string( "Could not open /dev/input/js0 for reading." );
	pfd.events = POLLPRI | POLLIN;
	pfd.revents = 0;

	buttA = false;
	buttB = false;
	buttX = false;
	buttY = false;
	buttLB = false;
	buttRB = false;
	buttStart = false;
	buttBack = false;
	buttJSL = false;
	buttJSR = false;

	jsL_vert = 0;
	jsL_hor = 0;
	jsR_vert = 0;
	jsR_hor = 0;
	dp_vert = 0;
	dp_hor = 0;
	jsLT = 0;
	jsRT = 0;

	running = true;
	poll_thread = std::thread( &XBoxCtrl::poll_dev, this );
}

XBoxCtrl::~XBoxCtrl()
{
	stop();
	close( pfd.fd );
}

void XBoxCtrl::stop()
{
	if( running )
	{
		running = false;
		poll_thread.join();
	}
}

void XBoxCtrl::updateValue( uint8_t type, uint8_t number, int16_t value )
{
	std::lock_guard<std::mutex> lock( event_mutex );
	switch( type )
	{
		case TYPE_BUTTON_PRESSED:
			switch( number )
			{
				case NUM_BUTT_A:
					buttA = (bool)value;
					break;
				case NUM_BUTT_B:
					buttA = (bool)value;
					break;
				case NUM_BUTT_X:
					buttA = (bool)value;
					break;
				case NUM_BUTT_Y:
					buttA = (bool)value;
					break;
				case NUM_BUTT_BACK:
					buttBack = (bool)value;
					break;
				case NUM_BUTT_START:
					buttStart = (bool)value;
					break;
				case NUM_BUTT_LB:
					buttLB = (bool)value;
					break;
				case NUM_BUTT_RB:
					buttRB = (bool)value;
					break;
				case NUM_BUTT_RIGHT_JS:
					buttJSR = (bool)value;
					break;
				case NUM_BUTT_LEFT_JS:
					buttJSL = (bool)value;
					break;
				default:
					std::cerr << "Undefined button: " << number << std::endl;
					break;
			}
			break;
		case TYPE_JOYSTICK_MOVED:
			switch( number )
			{
				case NUM_JS_LT:
					jsLT = value / 327.67f;
					break;
				case NUM_JS_L_HOR:
					jsL_hor = value / 327.67f;
					break;
				case NUM_JS_L_VER:
					jsL_vert = value / 327.67f;
					break;
				case NUM_JS_RT:
					jsRT = value / 327.67f;
					break;
				case NUM_JS_R_HOR:
					jsR_hor = value / 327.67f;
					break;
				case NUM_JS_R_VER:
					jsR_vert = value / 327.67f;
					break;
				case NUM_DP_HOR:
					dp_hor = value / 327.67f;
					break;
				case NUM_DP_VER:
					dp_vert = value / 327.67f;
					break;
				default:
					std::cerr << "Undefined joystick: " << number << std::endl;
					break;
			}
			break;
		default:
			std::cerr << "Undefined type: " << (unsigned int)type << std::endl;
			break;
	}
}

bool XBoxCtrl::getButtA()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttA;
}

bool XBoxCtrl::getButtB()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttB;
}

bool XBoxCtrl::getButtX()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttX;
}

bool XBoxCtrl::getButtY()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttY;
}

bool XBoxCtrl::getButtLB()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttLB;
}

bool XBoxCtrl::getButtRB()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttRB;
}

bool XBoxCtrl::getButtStart()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttStart;
}

bool XBoxCtrl::getButtBack()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttBack;
}

bool XBoxCtrl::getButtJSL()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttJSL;
}

bool XBoxCtrl::getButtJSR()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return buttJSR;
}

double XBoxCtrl::getJsLVertical()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return jsL_vert;
}

double XBoxCtrl::getJsLHorizontal()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return jsL_hor;
}

double XBoxCtrl::getJsRVertical()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return jsR_vert;
}

double XBoxCtrl::getJsRHorizontal()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return jsR_hor;
}

double XBoxCtrl::getDpVertical()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return dp_vert;
}

double XBoxCtrl::getDpHorizontal()
{
	std::lock_guard<std::mutex> lock( event_mutex );

	return dp_hor;
}

void XBoxCtrl::poll_dev()
{
	int ret = -1;

	while( running )
	{
		ret = poll( &pfd, 1, 1000 );	//Dont poll for -1 since we need to be able to stop()
		if( ret < 0 )		//Error
			throw std::string( "poll failed." );
		else if( ret == 0 )	//Timeout
			continue;

		input_event_t rx;
		int br = read( pfd.fd, &rx, sizeof( rx ) );
		if( br != sizeof( input_event_t ) )
			throw std::string( "read failed" );

		updateValue( rx.type, rx.number, rx.value );
	}
}
