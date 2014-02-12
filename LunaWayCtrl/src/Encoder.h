/*
 * Encoder.h
 *
 *  Created on: Jan 30, 2014
 *      Author: john
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <vector>
#include "AbstractCyclicThread.h"

extern "C"
{
#include <poll.h>
}

#define MAX_INT_DELAY_BUF_ENTRIES 10
#define PULSES_PER_REVOLUTION 15

class Encoder : public AbstractCyclicThread
{
	public:
		Encoder();
		Encoder( int pin );
		virtual ~Encoder();
		double getRps();

	private:
		void cyclic();
		struct pollfd pfd;
		std::vector<double> int_delay_buf;
};

#endif /* ENCODER_H_ */
