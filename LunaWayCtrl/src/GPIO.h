/*
 * GPIO.h
 *
 *  Created on: Feb 14, 2014
 *      Author: john
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "GPIOPin.h"

extern "C"
{
#include <inttypes.h>
}

class GPIO
{
	public:
		static GPIO *Instance();
		virtual ~GPIO();
		GPIOPin *getPin( int pin );

	private:
		GPIO();
		static GPIO *m_pInstance;
		volatile unsigned *gpio;
		GPIOPin **pins;
};

#endif /* GPIO_H_ */
