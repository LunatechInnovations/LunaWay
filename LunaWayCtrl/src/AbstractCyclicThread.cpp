/*
 * AbstractCyclicThread.cpp
 *
 *  Created on: Feb 12, 2014
 *      Author: john
 */

#include "AbstractCyclicThread.h"

AbstractCyclicThread::AbstractCyclicThread() : running( false )
{
}

AbstractCyclicThread::~AbstractCyclicThread()
{
	stop();
}

void AbstractCyclicThread::start()
{
	if( !running )
	{
		running = true;
		thread = std::thread( AbstractCyclicThread::hlp_cyclic, this );
	}
}

void AbstractCyclicThread::stop()
{
	if( running )
	{
		running = false;
		thread.join();
	}
}

void AbstractCyclicThread::hlp_cyclic()
{
	while( running )
		cyclic();
}
