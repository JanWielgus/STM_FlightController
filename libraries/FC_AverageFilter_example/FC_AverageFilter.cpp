/*
    Created:	11/09/2019 19:38:17
    Author:     Jan Wielgus
*/
// 
// 
// 

#include "FC_AverageFilter.h"


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

