#include <stdio.h>
#include <thread>
#include <vector>



class ThreadFunc
{
public:
	double area;
	double x0,x1,d;
	void Entry(void);
};
void ThreadFunc::Entry(void)
{
	area=0.0;
	for(double x=x0; x<x1; x+=d)
	{
		for(double y=-1.0; y<=1.0; y+=d)
		{
			if(x*x+y*y<1.0)
			{
				area+=d*d;
			}
		}
	}
}

int main(void)
{
	double d=0.00005;
	int nThread=8;

	std::vector <ThreadFunc> params;
	params.resize(nThread);
	for(int i=0; i<nThread; ++i)
	{
		params[i].x0=-1.0+((double) i   *(2.0/(double)nThread));
		params[i].x1=-1.0+((double)(i+1)*(2.0/(double)nThread));
		params[i].d=d;
	}

	std::vector <std::thread> t;
	t.resize(nThread);
	for(int i=0; i<nThread; ++i)
	{
		std::thread x(&ThreadFunc::Entry,&params[i]);
		t[i].swap(x);
	}

	for(auto &x : t)
	{
		x.join();
	}

	double area=0.0;
	for(auto &p : params)
	{
		area+=p.area;
	}
	printf("Area of unit circle=%lf\n",area);
	return 0;
}
