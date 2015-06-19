#pragma once
class Kernel
{
public:
	Kernel(unsigned long KernelLength);
	~Kernel(void);
	int padding;
	void AverageKernel();
	void GaussianKernel(float);
	float** KernelMatrix;
	unsigned long mKernelLength;
};

