/*
 * Switch.cpp
 *
 *  Created on: Feb 5, 2014
 *      Author: john
 */

#include "Switch.h"
#include <wiringPi.h>

Switch::Switch( int pin ) : _pin( pin )
{
	pinMode( _pin, INPUT );
}

Switch::~Switch()
{
}

bool Switch::getValue()
{
	return (bool)digitalRead( _pin );
}

