/* GPIO.cpp

Copyright (C) 2014 Lunatech Innovations

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
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

/*! \fn GPIO* GPIO::Instance()
 * \brief This function creates and returns a pointer to the one and only instance of GPIO.
 * \return Pointer to GPIO instance
 */
GPIO* GPIO::Instance()
{
	if( m_pInstance == nullptr )
		m_pInstance = new GPIO();

	return m_pInstance;
}

/*! \fn GPIO::~GPIO()
 * \brief Destructor
 * \details Free memory
 */
GPIO::~GPIO()
{
	for( int i = 0; i <= MAX_GPIO_PIN; i++ )
	{
		if( pins[i] != nullptr )
			delete pins[i];
	}

	delete pins;
}

/*! \fn GPIOPin* GPIO::getPin( int pin )
 * \brief Getter for the pins
 * @param[in] pin The pin number to return
 * \return A GPIOPin pointer
 */
GPIOPin* GPIO::getPin( int pin )
{
	return pins[pin];
}

/*! \fn GPIO::GPIO()
 * \brief Constructor
 * \details Constructor is private.\n
 * This is done to prevent users from creating more than one instance.
 */
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
