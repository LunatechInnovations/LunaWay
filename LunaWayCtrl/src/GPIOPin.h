/*
 * GPIOPin.h
 *
 *  Created on: Feb 14, 2014
 *      Author: john
 */

#ifndef GPIOPIN_H_
#define GPIOPIN_H_

#include <mutex>
#include <thread>
#include <functional>

extern "C"
{
#include <poll.h>
}

class GPIOPin
{
		enum PinModes
		{
			Unassigned, Input, Output, Interrupt
		};

	public:
		enum Edges
		{
			Rising, Falling
		};

		GPIOPin( int pin, volatile unsigned *gpio );
		virtual ~GPIOPin();
		void setupInput();
		void setupOutput();
		void setupInterrupt( std::function<void(bool)> callback, int edge );
		void setValue( bool value );
		bool getValue();


	private:
		void cyclic();
		volatile unsigned *_gpio;
		void poll_interrupt();
		int _mode;
		int _pin;
		std::mutex mem_mutex;
		struct pollfd pfd;	//Interrupt pollrequest
		std::function<void(bool)> interrupt_callback;
		volatile bool running;
		std::thread cyclic_thread;

};

#endif /* GPIOPIN_H_ */
