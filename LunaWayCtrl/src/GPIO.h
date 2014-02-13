/*
 * GPIO.h
 *
 *  Created on: Feb 13, 2014
 *      Author: john
 */

#ifndef GPIO_H_
#define GPIO_H_
#include <string>

class GPIO
{
	public:
		enum Direction
		{
			Input, Output
		};
		enum Edge
		{
			Rising, Falling
		};

		GPIO( int pin );
		virtual ~GPIO();
		void setupInterrupt( int edge );
		bool getValue();
		void setValue( bool value );
		void setupDir( int dir );

	private:
		int _fd;
		int _pin;
		int _dir;
};

#endif /* GPIO_H_ */
