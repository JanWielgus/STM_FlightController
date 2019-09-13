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



//////////////////////////////////////////////////////////////////////////

// IMPLEMENTATION

// I don't know how to make generic code work if it is in separate cpp file

template <class SampleType, class SumType, class ReturnType>
FC_AverageFilter<SampleType, SumType, ReturnType>::~FC_AverageFilter()
{
	if (AveragedSamples > 0)
	delete [] sampleArray;
}


template <class SampleType, class SumType, class ReturnType>
void FC_AverageFilter<SampleType, SumType, ReturnType>::addNewSample(SampleType newSample)
{
	// Move arrayIndex into the next position
	arrayIndex++;
	if (arrayIndex >= AveragedSamples)
	arrayIndex -= AveragedSamples;
	
	// Subtract oldest value from the sum and add the new one
	sum -= sampleArray[arrayIndex];
	sum += newSample;
	sampleArray[arrayIndex] = newSample;
	
	average = sum / (ReturnType)AveragedSamples;
}


template <class SampleType, class SumType, class ReturnType>
ReturnType FC_AverageFilter<SampleType, SumType, ReturnType>::getAverage()
{
	return average;
}


template <class SampleType, class SumType, class ReturnType>
void FC_AverageFilter<SampleType, SumType, ReturnType>::reset()
{
	arrayIndex = 0;
	sum = 0;
	average = 0;
	
	for (int i=0; i<AveragedSamples; i++)
	sampleArray[i] = 0;
}


#endif

