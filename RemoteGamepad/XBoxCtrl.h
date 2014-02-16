/*
 * XBoxCtrl.h
 *
 *  Created on: Feb 15, 2014
 *      Author: john
 */

#ifndef XBOXCTRL_H_
#define XBOXCTRL_H_

#include <thread>
#include <mutex>

extern "C"
{
#include <inttypes.h>
#include <sys/time.h>
#include <poll.h>
}

class XBoxCtrl
{
	public:
		typedef struct
		{
			uint32_t time;
			int16_t value;
			uint8_t type;
			uint8_t number;
		}input_event_t;

		XBoxCtrl( int device );
		virtual ~XBoxCtrl();
		void stop();
		bool getButtA();
		bool getButtB();
		bool getButtX();
		bool getButtY();
		bool getButtLB();
		bool getButtRB();
		bool getButtStart();
		bool getButtBack();
		bool getButtJSL();
		bool getButtJSR();
		double getJsLVertical();
		double getJsLHorizontal();
		double getJsRVertical();
		double getJsRHorizontal();
		double getDpVertical();
		double getDpHorizontal();

	private:
		void updateValue( uint8_t type, uint8_t number, int16_t value );
		std::thread poll_thread;
		volatile bool running;
		void poll_dev();
		struct pollfd pfd;
		bool buttA, buttB, buttX, buttY, buttLB, buttRB, buttStart, buttBack, buttJSL, buttJSR;
		double jsL_vert, jsL_hor, jsR_vert, jsR_hor, dp_vert, dp_hor, jsLT, jsRT;
		std::mutex event_mutex;
};

#endif /* XBOXCTRL_H_ */
