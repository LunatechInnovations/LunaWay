//============================================================================
// Name        : xbox.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "XBoxCtrl.h"
#include <unistd.h>

extern "C"
{
#include <signal.h>
}

using namespace std;

volatile bool running = true;

void exiting_callback( int )
{
	running = false;
	cout << endl;
}

int main()
{
	try
	{
		signal( SIGINT, exiting_callback );
		signal( SIGABRT, exiting_callback );
		signal( SIGTERM, exiting_callback );

		XBoxCtrl xbox;

		while( running )
		{
			cout << xbox.getJsLHorizontal() << " " << xbox.getJsLVertical() << " " << xbox.getButtA() << endl;
			usleep( 100000 );
		}

		xbox.stop();
	}
	catch( const string &e )
	{
		cerr << e << endl;
		return 1;
	}

	cout << "Exiting..." << endl;

	return 0;
}
