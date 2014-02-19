/* Motor.h

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

#ifndef MOTOR_H_
#define MOTOR_H_

#include <mutex>
#include "Encoder.h"
#include "AbstractCyclicThread.h"
#include "GPIOPin.h"

/*! \class Motor
 * \brief This class provides a PWM motor controller.
 */
class Motor : public AbstractCyclicThread
{
	public:
		Motor();
		Motor( GPIOPin *dir = nullptr, GPIOPin *pwm = nullptr, GPIOPin *enc = nullptr, int freq = 0 );
		virtual ~Motor();
		void setOutput( double value );
		double getRPS();
		double getOutput();
		void stop();

	private:
		GPIOPin *_dir, *_pwm;
		void cyclic();
		double output;
		double _freq;
		std::mutex output_mutex;
		Encoder *encoder;
};

#endif /* MOTOR_H_ */
