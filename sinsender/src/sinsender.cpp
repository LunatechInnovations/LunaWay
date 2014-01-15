//============================================================================
// Name        : sinsender.cpp
// Author      : John Erlandsson
// Version     : 0.1
// Description : Tool for testing the segwayPlotter
//============================================================================

#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ostream>

extern "C"
{
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
}

using namespace std;

//Global variables
struct addrinfo *g_host_info_list = NULL;
int g_sock_fd = -1;

/* exit_handler
 * close socket on exit
 */
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

	exit( 0 );
}

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

	if( (connect( g_sock_fd, g_host_info_list->ai_addr, g_host_info_list->ai_addrlen )) == -1 )
	{
		cerr << "connect failed." << endl;
		return false;
	}

	return true;
}

/* print_usage
 * Show how to use this program
 */
void print_usage()
{
	cout << "Usage: sinsender [HOST] [PORT]" << endl;
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

int main( int argc, char **argv )
{
	//Handle program arguments
	if( argc < 3 )
	{
		print_usage();
		return 1;
	}

	string host = argv[1];
	string port = argv[2];

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

	double value = 0.0f;

	//Setup exit handler
	ssignal( SIGINT, exit_handler );
	ssignal( SIGABRT, exit_handler );
	ssignal( SIGTERM, exit_handler );

	//Connect to server
	if( !connect( host, port ) )
	{
		cerr << "Failed to connect." << endl;
		return 1;
	}

	//Set timeout for recv function
	struct timeval recv_timeout;
	recv_timeout.tv_sec = 0;
	recv_timeout.tv_usec = 100000;

	setsockopt( g_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&recv_timeout, sizeof( struct timeval ) );

	int fail_counter = 0;

	while( true )
	{
		ostringstream tx;
		tx << sin( value ) * 100.0f << ';' << cos( value ) * 100.0f << ';';

		if( (send( g_sock_fd, (void *)tx.str().c_str(), tx.str().size(), 0 )) == -1 )
		{
			cerr << "Failed to send..." << endl;
			fail_counter++;
		}
		else
		{
			fail_counter = 0;
		}

		if( fail_counter >= 10 )
		{
			cerr << "Server has gone away." << endl;
			break;
		}

		//Receive command. recv will also cause the delay.
		char rx[1024];
		int bytes_received = recv( g_sock_fd, &rx, sizeof( rx ), 0 );

		if( bytes_received > 0 )
			cout << rx << endl;

		value = (value + 0.05f);
	}

	freeaddrinfo( g_host_info_list );
	close( g_sock_fd );

	cout << "Exiting..." << endl;

	return 0;
}
