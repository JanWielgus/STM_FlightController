/*
    Created:	11/09/2019 19:38:17
    Author:     Jan Wielgus
*/
// FC_AverageFilter.h

#ifndef _FC_AVERAGEFILTER_h
#define _FC_AVERAGEFILTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

template <class SampleType, class SumType, class ReturnType>
	// Type of the sample;
	// SumType-data type to fit sum of all samples;
	// Return type
class FC_AverageFilter
{
 public:
	FC_AverageFilter<SampleType, SumType, ReturnType>(uint16_t averagedSamples);
		// averagedSamples - how many samples will be averaged
	~FC_AverageFilter();
	void addNewSample(SampleType newSample);
	SampleType getAverage();
	void reset();
	
	
 private:
	uint16_t AveragedSamples; // amt of samples to average
	SumType sum;
	ReturnType average;
};


typedef FC_AverageFilter<int32_t, int32_t, float> baroAverageFilter;



#endif

