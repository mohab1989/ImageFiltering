#pragma once
#include "Undo.h"
class Filters
{
public:
	Filters(void);
	~Filters(void);
	void AverageFilter(CMyImage& image, int Rank);
	void MedianFilter(CMyImage& image,int Rank);
	void GaussianFilter(CMyImage& image,int Rank,float Sigma);
private:
	void swap(BYTE *pNum1 ,BYTE *Pnum2);
	int padding;
	unsigned long ImageWidth;
	unsigned long ImageHeight;
	void ExchangeSort(BYTE arr[], int size);
};