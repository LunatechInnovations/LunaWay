/*
 * Encoder.h
 *
 *  Created on: Jan 30, 2014
 *      Author: john
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <thread>

extern "C"
{
#include <poll.h>
}

#define MODE_RISING 1
#define MODE_FALLING 0


class Encoder
{
	public:
		Encoder( int pin, int mode );
		virtual ~Encoder();
		void stop();

	private:
		void threaded_poll();
		struct pollfd pfd;
		std::thread poll_thread;
		volatile bool running;
		int _mode;
};

#endif /* ENCODER_H_ */
