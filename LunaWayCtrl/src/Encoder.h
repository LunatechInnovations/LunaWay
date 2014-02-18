/*
 * Encoder.h
 *
 *  Created on: Jan 30, 2014
 *      Author: john
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <vector>
#include "GPIOPin.h"
#include <chrono>

extern "C"
{
#include <poll.h>
}

#define MAX_INT_DELAY_BUF_ENTRIES 10
#define PULSES_PER_REVOLUTION 15

class Encoder
{
	public:
		Encoder();
		Encoder( GPIOPin *pin );
		virtual ~Encoder();
		double getRps();

	private:
		std::vector<double> int_delay_buf;
		void count( bool value );
		GPIOPin *_pin;
		std::chrono::high_resolution_clock::time_point last_int;
};

#endif /* ENCODER_H_ */
