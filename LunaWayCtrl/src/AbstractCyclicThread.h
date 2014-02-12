/*
 * AbstractCyclicThread.h
 *
 *  Created on: Feb 12, 2014
 *      Author: john
 */

#ifndef ABSTRACTCYCLICTHREAD_H_
#define ABSTRACTCYCLICTHREAD_H_

#include <thread>

class AbstractCyclicThread
{
	public:
		AbstractCyclicThread();
		virtual ~AbstractCyclicThread();
		void start();
		void stop();

	protected:
		std::thread thread;
		volatile bool running;
		void hlp_cyclic();
		virtual void cyclic() = 0;
};

#endif /* ABSTRACTCYCLICTHREAD_H_ */
