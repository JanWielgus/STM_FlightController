// Author: Jan Wielgus
// Date: 30.12.2018
//

#include "FC_EVA_Filter.h"


FC_EVA_Filter::FC_EVA_Filter(float filterBeta)
{
	if (filterBeta >= 0 && filterBeta < 1)
		fBeta = filterBeta;
	else
		fBeta = DefBeta; // default
	
	lastFilteredValue = 0;
}


float FC_EVA_Filter::updateFilter(float newData)
{
	lastFilteredValue = lastFilteredValue*(fBeta) + newData*(1-fBeta);
	return lastFilteredValue;
}


bool FC_EVA_Filter::setFilterBeta(float filterBeta)
{
	if (filterBeta < 0 && filterBeta >= 1)
		return false;
	fBeta = filterBeta;
	return true;
}

