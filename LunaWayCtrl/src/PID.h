/*
 * PID.h
 *
 *  Created on: Feb 8, 2014
 *      Author: john
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
