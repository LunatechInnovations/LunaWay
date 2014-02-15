/*
 * GPIO.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: john
 */

#include "GPIO.h"
#include <cstdlib>
#include <string>

extern "C"
{
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
}

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x00200000)

#define MAX_GPIO_PIN 27

GPIO *GPIO::m_pInstance = nullptr;

GPIO* GPIO::Instance()
{
	if( m_pInstance == nullptr )
		m_pInstance = new GPIO();

	return m_pInstance;
}

GPIO::~GPIO()
{
	for( int i = 0; i <= MAX_GPIO_PIN; i++ )
	{
		if( pins[i] != nullptr )
			delete pins[i];
	}

	delete pins;
}

GPIOPin* GPIO::getPin( int pin )
{
	return pins[pin];
}

GPIO::GPIO()
{
	int mem_fd = open( "/dev/mem", O_RDWR | O_SYNC );
	if( mem_fd < 0 )
		throw std::string( "Failed to open /dev/mem" );

	void *gpio_map = (uint32_t *)mmap( nullptr, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE );
	if( gpio_map == MAP_FAILED )
		throw std::string( "mmap failed" );

	close( mem_fd );

	gpio = (volatile unsigned *)gpio_map;

	pins = new GPIOPin*[MAX_GPIO_PIN + 1];
	for( int i = 0; i <= MAX_GPIO_PIN; i++ )
		pins[i] = new GPIOPin( i, gpio );
}
