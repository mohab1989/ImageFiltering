#include "stdafx.h"
#include "Kernel.h"
#include <iostream>
#include <cmath>

Kernel::Kernel(unsigned long KernelLength)
{
	KernelMatrix = nullptr;
	mKernelLength = KernelLength;
	padding = (int)(KernelLength/2);
}


Kernel::~Kernel(void)
{
	if(KernelMatrix != nullptr)
	{
		for(int i = 0; i < mKernelLength; ++i)
		{
			delete [] KernelMatrix[i];
		}
		delete [] KernelMatrix;
		
	}
}

void Kernel::AverageKernel()
{
	KernelMatrix = new float*[mKernelLength];
	float  value = 1/(float)pow(mKernelLength, 2);
	for( int i = 0 ; i < mKernelLength ; i++ )
	{
		KernelMatrix[i] = new float[mKernelLength];
		for(int j = 0 ;  j <  mKernelLength ; j++)
		{
			KernelMatrix[i][j] = value;
		}
	}
}


void Kernel::GaussianKernel(float sigma)
{
	float sum=0;
	KernelMatrix = new float*[mKernelLength];
	for( int i=0 ,x=-padding ; i < mKernelLength,x<=padding ; i++,x++ )
	{
		KernelMatrix[i] = new float[mKernelLength];
		for(int j = 0 ,y=-padding;  j <  mKernelLength ,y<=padding; j++,y++)
		{
			//2D gaussian formula
			KernelMatrix[i][j] = (1/(2*(22/7)*pow(sigma,2)))*exp((-1*((x*x)+(y*y)))/(2*sigma*sigma));
			sum+=KernelMatrix[i][j];
		}
	}
	for( int i = 0 ; i < mKernelLength ; i++ )
	{
		for(int j = 0 ;  j <  mKernelLength ; j++)
		{
			KernelMatrix[i][j] = (KernelMatrix[i][j])/sum;
		}
	}
}