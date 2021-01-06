#include <iostream>
#include <stdlib.h>
#include <math.h>

int main(){

  double h,z;
  double sum=0;
  double x0=0;
  double y0=0;

  std::cout<<"Enter integration step: ";
  std::cin>>h;

  for(double y=y0; y<=1; y=y+h){
    for(double x=x0; x<=1; x=x+h){
      if(pow(x,2)+pow(y,2)<=1) z=sqrt(1-pow(x,2)-pow(y,2));
      else z=0;
      sum=sum+z*pow(h,2);
    }
  }

  std::cout<<"Result is: "<<sum<<std::endl;

  return 0;
}
