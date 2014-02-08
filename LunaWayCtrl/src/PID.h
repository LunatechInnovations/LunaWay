/*
 * PID.h
 *
 *  Created on: Feb 8, 2014
 *      Author: john
 */

#ifndef PID_H_
#define PID_H_

class PID
{
	public:
		PID();
		PID( double p, double i );
		PID( double p, double i, double i_state_min, double i_state_max );
		virtual ~PID();
		void setP( double p );
		void setI( double i );
		void setMaxIState( double max );
		void setMinIState( double min );
		void setIStateLimits( double min, double max );
		double regulate( double error, double d_term );

	private:
		double _p, _i;
		double _istate, _istate_max, _istate_min;
};

#endif /* PID_H_ */
