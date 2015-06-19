#include "stdafx.h"
#include "Filters.h"
#include "Kernel.h"

using namespace std;

Filters::Filters(void)
{
}


Filters::~Filters(void)
{
}

typedef struct 
{
	//CImage is BGR
	BYTE B;
	BYTE G;
	BYTE R;
	BYTE A;
}Pixel;

void Filters::AverageFilter(CMyImage & image,int Rank)
{
	ImageWidth =image.GetWidth();
	ImageHeight=image.GetHeight();
	padding=(int)(Rank/2);
	Kernel myKernel(Rank);
	myKernel.AverageKernel();

	for( int j = 0 ; j < ImageHeight; j++ )
	{
		for(int i = 0 ;  i <ImageWidth ; i++)
		{	
			float r=0,g=0,b=0;
			float Rsum=0,Gsum=0,Bsum=0;

			for (int k = -padding; k <= padding; k++)
			{
				for (int l = -padding; l <=padding; l++)
				{
					if ((j+k>=0) && (i+l>=0) &&(j+k<ImageHeight) && (i+l<ImageWidth))
					{
						auto pixel = (Pixel*)image.GetPixelAddress(i+l,j+k);

						r=pixel->R;
						r=r* myKernel.KernelMatrix[k+padding][l+padding];

						g=pixel->G;
						g=g* myKernel.KernelMatrix[k+padding][l+padding];

						b=pixel->B;
						b=b* myKernel.KernelMatrix[k+padding][l+padding];

						Rsum+=r;
						Gsum+=g;
						Bsum+=b;
					}
				}

			}
				auto pixel = (Pixel*)image.GetPixelAddress(i,j);			
				pixel->B=Bsum;
				pixel->G=Gsum;
				pixel->R=Rsum;		
		}
	}
}

void Filters::swap(BYTE *pNum1 ,BYTE *Pnum2)
{
	int temp = *pNum1;
	*pNum1= *Pnum2;
	*Pnum2= temp;
}

void Filters::ExchangeSort(BYTE arr[], int size)
{
		for (int i = 0; i < size-1; i++)
		{
			for (int j = i+1; j < size; j++)
			{
				if (arr[i]>arr[j])
				{
					swap(&arr[i],&arr[j]);
				}
			}
		}
}

void Filters::MedianFilter(CMyImage& image,int Rank)
{
	BYTE *RDynamicArray =new BYTE[Rank*Rank];
	BYTE *GDynamicArray =new BYTE[Rank*Rank];
	BYTE *BDynamicArray =new BYTE[Rank*Rank];
	ImageWidth =image.GetWidth();
	ImageHeight=image.GetHeight();
	padding=(int)(Rank/2);

	for( int j = 0 ; j < ImageHeight; j++ )
	{
		for(int i = 0 ;  i <ImageWidth ; i++)
		{	
			int counter=-1;
			int index=0;

			for (int k = -padding; k <= padding; k++)
			{
				for (int l = -padding; l <=padding; l++)
				{
					if ((j+k>=0) && (i+l>=0) &&(j+k<ImageHeight) && (i+l<ImageWidth))
					{
						auto pixel = (Pixel*)image.GetPixelAddress(i+l,j+k);
						counter++;
						BDynamicArray[counter]=pixel->B;
						GDynamicArray[counter]=pixel->G;
						RDynamicArray[counter]=pixel->R;
					}
				}
			}

			ExchangeSort(RDynamicArray,counter+1);
			ExchangeSort(GDynamicArray,counter+1);
			ExchangeSort(BDynamicArray,counter+1);


			index=(int)((counter+1)/2);
			

			auto pixel = (Pixel*)image.GetPixelAddress(i,j);
			pixel->B=BDynamicArray[index];
			pixel->G=GDynamicArray[index];
			pixel->R=RDynamicArray[index];

		}
	}
	delete [] RDynamicArray;
	delete [] GDynamicArray;
	delete [] BDynamicArray;
}

void Filters::GaussianFilter(CMyImage& image,int Rank,float sigma)
{
	ImageWidth =image.GetWidth();
	ImageHeight=image.GetHeight();
	padding=(int)(Rank/2);
	Kernel myKernel(Rank);
	myKernel.GaussianKernel(sigma);

	for( int j = 0 ; j < ImageHeight; j++ )
	{
		for(int i = 0 ;  i <ImageWidth ; i++)
		{	
			float r=0,g=0,b=0;
			float Rsum=0,Gsum=0,Bsum=0;

			for (int k = -padding; k <= padding; k++)
			{
				for (int l = -padding; l <=padding; l++)
				{
					if ((j+k>=0) && (i+l>=0) &&(j+k<ImageHeight) && (i+l<ImageWidth))
					{
						auto pixel = (Pixel*)image.GetPixelAddress(i+l,j+k);

						r=pixel->R;
						r=r* myKernel.KernelMatrix[k+padding][l+padding];

						g=pixel->G;
						g=g* myKernel.KernelMatrix[k+padding][l+padding];

						b=pixel->B;
						b=b* myKernel.KernelMatrix[k+padding][l+padding];

						Rsum+=r;
						Gsum+=g;
						Bsum+=b;
					}
				}

			}
				auto pixel = (Pixel*)image.GetPixelAddress(i,j);			
				pixel->B=Bsum;
				pixel->G=Gsum;
				pixel->R=Rsum;		
		}
	}
}