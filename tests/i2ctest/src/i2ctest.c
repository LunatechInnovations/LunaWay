//#include <stdio.h>
//#include <stdlib.h>
//#include <wiringPiI2C.h>
//#include <unistd.h>
//
//int main(void)
//{
//	int i2c_fd = wiringPiI2CSetup( 0x4d );
//
//	while( 1 )
//	{
//		printf( "värde: %d\n", wiringPiI2CRead( i2c_fd ) );
//		usleep( 1000 );
//	}
//
//	return EXIT_SUCCESS;
//}

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

double g_gyro = 0.0f;
int g_socket_fd;
struct addrinfo *g_host_info_list;

void conn()
{
	struct addrinfo host_info;
	memset( &host_info, 0, sizeof( struct addrinfo ) );

	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;


	if( (getaddrinfo( "192.168.0.137", "5555", &host_info, &g_host_info_list )) != 0 )
	{
		fprintf( stderr, "getaddrinfo failed\n" );
		exit( EXIT_FAILURE );
	}

	g_socket_fd = socket( g_host_info_list->ai_family, g_host_info_list->ai_socktype, g_host_info_list->ai_protocol );
	if( g_socket_fd == -1 )
	{
		fprintf( stderr, "socket failed\n" );
		exit( EXIT_FAILURE );
	}

	if( (connect( g_socket_fd, g_host_info_list->ai_addr, g_host_info_list->ai_addrlen )) == -1 )
	{
		fprintf( stderr, "connect failed\n" );
		exit( EXIT_FAILURE );
	}

	struct timeval recv_timeot;
	recv_timeot.tv_sec = 0;
	recv_timeot.tv_usec = 10000;
	setsockopt( g_socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&recv_timeot, sizeof( struct timeval ) );
}

int main( void )
{
	int file;
	char filename[20];
	const int addr = 0x4d; /* The I2C address */

	// Open a file giving access to i2c interface 0
	sprintf( filename, "/dev/i2c-1" );
	if ((file = open( filename, O_RDWR )) < 0 )
	{
		fprintf( stderr, "Failed to open device: %s\n", filename );
		return EXIT_FAILURE;
	}

	// Check that the chip is there
	if( ioctl( file, I2C_SLAVE, addr ) < 0 )
	{
		fprintf( stderr, "ioctl failed.\n" );
		return EXIT_FAILURE;
	}

	conn();

	while( 1 )
	{
		uint8_t values[2];
		uint16_t value = 0;
		char tx_buffer[28];
		char rx_buffer[128];

		memset( &values, 0, sizeof( values ) );
		memset( &tx_buffer, 0, sizeof( tx_buffer ) );
		memset( &rx_buffer, 0, sizeof( rx_buffer ) );

		read( file, &values, sizeof( values ) );

		value = (value<<8);
		value += values[1];
		g_gyro = ((double)(((int)value)) - 247.0f) * 0.1f;

		sprintf( tx_buffer, "%0.2f;\n", g_gyro );

		printf( "Value: %0.2f\n", g_gyro );

		if( (send( g_socket_fd, &tx_buffer, strlen( tx_buffer ), 0 )) <= 0 )
		{
			fprintf( stderr, "Server has gone away\n" );
			exit( EXIT_FAILURE );
		}

		if( (recv( g_socket_fd, rx_buffer, sizeof( rx_buffer ), 0 )) > 0 )
			printf( "Incoming command: %s\n", rx_buffer );
	}

	close(file);
	exit(0);
}
