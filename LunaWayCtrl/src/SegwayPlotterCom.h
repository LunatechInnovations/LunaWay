/*
 * SegwayPlotterCom.h
 *
 *  Created on: Feb 5, 2014
 *      Author: john
 */

#ifndef SEGWAYPLOTTERCOM_H_
#define SEGWAYPLOTTERCOM_H_

#include <string>

class SegwayPlotterCom
{
	public:
		SegwayPlotterCom();
		virtual ~SegwayPlotterCom();
		bool connect( std::string host, int port );
		bool send( std::string data );
};

#endif /* SEGWAYPLOTTERCOM_H_ */
