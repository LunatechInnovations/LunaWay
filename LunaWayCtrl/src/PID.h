/* PID.h

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

#ifndef PID_H_
#define PID_H_

#include <mutex>

class PID
{
	public:
		PID();
		PID( double p, double i, double d, double sv );
		PID( double p, double i, double d, double sv, double i_state_min, double i_state_max );
		virtual ~PID();
		void setP( double p );
		void setI( double i );
		void setD( double i );
		void setSV( double sv );
		void setMaxIState( double max );
		void setMinIState( double min );
		void setIStateLimits( double min, double max );
		double regulate( double pv, double gyro_rate );
		double getSV();

	private:
		double _p, _i, _d, _sv;
		double _istate, _istate_max, _istate_min;
		std::mutex p_mutex, i_mutex, d_mutex, sv_mutex;
};

#endif /* PID_H_ */
