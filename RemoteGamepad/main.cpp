//============================================================================
// Name        : RemoteGamepad.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#include "XBoxCtrl.h"

extern "C"
{
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
}

/* Parameters
 */
#define N_RETRIES 5 			//How many times we retry sending before calling it a failure
#define DELAY_TIME_US 10000		//Delay time for main loop
#define DELAY_TIME_S 0

using namespace std;
using namespace chrono;

//Global variables
struct addrinfo *g_host_info_list = NULL;
int g_sock_fd = -1;

/* connect
 * setup socket and connect
 */
bool connect( string host, string port )
{
	struct addrinfo host_info;
	memset( &host_info, 0, sizeof( struct addrinfo ) );

	host_info.ai_family = AF_UNSPEC; 		//Accept both ipv4 and ipv6
	host_info.ai_socktype = SOCK_STREAM;	//TCP socket

	if( (getaddrinfo( host.c_str(), port.c_str(), &host_info, &g_host_info_list )) != 0 )
	{
		cerr << "getaddrinfo failed." << endl;
		return false;
	}

	//Create socket
	//We use the first available interface.
	g_sock_fd = socket( g_host_info_list->ai_family, g_host_info_list->ai_socktype, g_host_info_list->ai_protocol );
	if( g_sock_fd == -1 )
	{
		cerr << "socket failed." << endl;
		return false;
	}

	//Connect
	if( (connect( g_sock_fd, g_host_info_list->ai_addr, g_host_info_list->ai_addrlen )) == -1 )
	{
		cerr << "connect failed." << endl;
		return false;
	}

	return true;
}

void exit_handler( int signal )
{
	if( g_sock_fd > -1 )
		close( g_sock_fd );

	if( g_host_info_list != NULL )
		freeaddrinfo( g_host_info_list );

	cout << endl << "Caught signal: ";

	switch( signal )
	{
		case SIGINT:
			cout << "SIGINT";
			break;
		case SIGABRT:
			cout << "SIGABRT";
			break;
		case SIGTERM:
			cout << "SIGTERM";
			break;
		default:
			cout << "UNKNOWN";
			break;
	}

	cout << " exiting..." << endl;

	exit( EXIT_SUCCESS );
}

/* is_valid_port
 * Determine if the string argument represents a valid port number
 */
bool is_valid_port( string str )
{
	if( str.size() <= 0 )
		return false;

	int port = atoi( str.c_str() );
	if( port < 0 || port > 65535 )
		return false;

	return true;
}

int main( int argc, char ** argv )
{
	//Handle program arguments
	if( argc < 4 )
	{
		cout << "Usage: RemoteGamepad [HOST] [PORT] [DEVICE]" << endl;
		return EXIT_FAILURE;
	}

	string host = argv[1];
	string port = argv[2];
	stringstream conv;
	int device;
	conv << argv[3];
	conv >> device;

	//Validate hostname and port
	if( host.size() <= 1 )
	{
		cerr << "Please enter a valid hostname or ip" << endl;
		return 1;
	}
	if( !is_valid_port( port ) )
	{
		cerr << "Please enter a port number between 0 - 65535" << endl;
		return 1;
	}

	//Setup exit handler
	signal( SIGINT, exit_handler );
	signal( SIGABRT, exit_handler );
	signal( SIGTERM, exit_handler );

	//Connect to server
	if( !connect( host, port ) )
	{
		cerr << "Failed to connect." << endl;
		return EXIT_FAILURE;
	}

	int fail_counter = 0;

	try
	{
		XBoxCtrl gamepad( device );

		while( true )
		{
			ostringstream tx;

			tx << gamepad.getJsLHorizontal() << ';' << gamepad.getJsLVertical() << '\n';

			if( (send( g_sock_fd, (void *)tx.str().c_str(), tx.str().size(), 0 )) == -1 )
			{
				cerr << "Failed to send..." << endl;
				fail_counter++;
			}
			else
			{
				fail_counter = 0;
				this_thread::sleep_for( milliseconds( 100 ) );
			}

			if( fail_counter >= N_RETRIES )
			{
				cerr << "Server has gone away." << endl;
				break;
			}
		}

	gamepad.stop();
	}
	catch( const string &e )
	{
		cerr << e << endl;
		return EXIT_FAILURE;
	}

	//Cleanup
	freeaddrinfo( g_host_info_list );
	close( g_sock_fd );

	cout << "Exiting..." << endl;

	return EXIT_SUCCESS;
}
