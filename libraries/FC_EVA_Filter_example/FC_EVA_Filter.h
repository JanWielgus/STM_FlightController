// FC_EVA_Filter.h
//
// Author: Jan Wielgus
// Date: 30.12.2018
//

#ifndef _FC_EVA_FILTER_h
#define _FC_EVA_FILTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif



class FC_EVA_Filter
{
 private:
	static const float DefBeta; // default filter Beta
	
 public:
	FC_EVA_Filter(float filterBeta = DefBeta);
	const float& updateFilter(float& newData); // returns new calculated value based on newData
	bool setFilterBeta(float filterBeta); // change filter Beta filter uses
	
 private:
	float lastFilteredValue;
	float fBeta; // filter Beta (0 <= fBeta < 1) 0.99 - large filtering; 0 - no filtering
};

#endif

