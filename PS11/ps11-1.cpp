#include <stdio.h>

template<class T>


T Summation(const int nElem,T elem[])
{
	T sum=0;
	for(int i=0; i<nElem; ++i)
	{
		sum+=elem[i];
	}
	return sum;
}

int main(void)
{
	int intArray[5]={1,2,4,8,16};
	double doubleArray[5]={8.1,9.1,10.1,11.1,12.1};
	float floatArray[5]={1.5f,2.5f,3.5f,4.5f,5.5f};

	printf("Summation of 1,2,4,8,16=%d\n",Summation(5,intArray));
	printf("Summation of 8.1,9.1,10.1,11.1,12.1=%lf\n",Summation(5,doubleArray));
	printf("Summation of 1.5,2.5,3.5,4.5,5.5=%f\n",Summation(5,floatArray));

	return 0;
}
