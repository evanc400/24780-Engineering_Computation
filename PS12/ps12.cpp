#include <stdio.h>
#include <time.h>
#include <vector>
#include <thread>
#include<iostream>

class ThreadFunc{
public:
  int M;
  double x0,x1;
  void Entry(void);
};

void ThreadFunc::Entry(void){
  const int res=40000;
  M=0;
  for(int x=x0; x<x1; ++x)
  {
      for(int y=0; y<res; ++y)
      {
          const double xx=(double)x/(double)res;
          const double yy=(double)y/(double)res;

          if(xx*xx+yy*yy<=1.0)
          {
              M++;
          }
      }
  }
}

int main(void)
{
    const int res=40000;

    auto t0=time(nullptr);

    const int N=res*res;

    int nThread=8;

    std::vector <ThreadFunc> params;
    params.resize(nThread);
    for(int i=0; i<nThread; ++i)
    {
  		params[i].x0=res*i/8;
  		params[i].x1=res*(i+1)/8;
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

    int M=0;

    for(auto &p : params)
  	{
  		M+=p.M;
  	}

    const double pi=2.0*2.0*(double)M/(double)N;

    printf("Estimated PI=%lf\n",pi);
    printf("Time Elapsed=%d\n",(int)(time(nullptr)-t0));

    return 0;
}
