/*
 * Switch.h
 *
 *  Created on: Feb 5, 2014
 *      Author: john
 */

#ifndef SWITCH_H_
#define SWITCH_H_

class Switch
{
	public:
		Switch( int pin );
		virtual ~Switch();
		bool getValue();

	private:
		int _pin;
};

#endif /* SWITCH_H_ */
