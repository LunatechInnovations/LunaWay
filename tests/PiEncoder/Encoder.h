/*
 * Encoder.h
 *
 *  Created on: Jan 30, 2014
 *      Author: john
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <thread>
#include <vector>

extern "C"
{
#include <poll.h>
}

#define MAX_INT_DELAY_BUF_ENTRIES 10
#define PULSES_PER_REVOLUTION 15

class Encoder
{
	public:
		Encoder( int pin );
		virtual ~Encoder();
		void stop();
		double getRpm();

	private:
		void threaded_poll();
		struct pollfd pfd;
		std::thread poll_thread;
		volatile bool running;
		std::vector<double> int_delay_buf;
};

#endif /* ENCODER_H_ */
