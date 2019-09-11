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
	FC_AverageFilter<SampleType, SumType, ReturnType>(uint16_t averagedSamples)
		// averagedSamples - how many samples will be averaged
		: AveragedSamples(averagedSamples)
	{
		if (AveragedSamples > 0)
			sampleArray = new SampleType[AveragedSamples];
		
		// Reset the array
		reset();
	}
	
	~FC_AverageFilter();	
	void addNewSample(SampleType newSample);
	ReturnType getAverage();
	void reset();
	
	
 private:
	const uint16_t AveragedSamples; // amt of samples to average
	SampleType* sampleArray;
	uint16_t arrayIndex = 0;
	SumType sum = 0;
	ReturnType average = 0;
};


// Temporary example how to create filter for barometer
typedef FC_AverageFilter<int32_t, int32_t, float> baroAverageFilter;



#endif

