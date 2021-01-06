#include <iostream>
#include <stdlib.h>
#include <time.h>

int main(){

    srand(time(nullptr));

    int num1,num2,num3,ansCorrect,ansUser;

    num1=rand()% 10;
    num2=rand()% 10;
    num3=rand()% 10;

    ansCorrect=num1*num2*num3;

    std::cout<<num1<<'x'<<num2<<'x'<<num3<<"=?> ";

    time_t begin_time=time(nullptr);
    std::cin>>ansUser;
    time_t dt=(time(nullptr) - begin_time);

    if(ansCorrect==ansUser){
      std::cout<<"Correct!  You spent " << dt <<" seconds to calculate."<<std::endl;
    }
    else std::cout<<"Wrong!  Correct answer is " << ansCorrect <<'.'<<std::endl;

    return 0;
}
