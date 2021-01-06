#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(){

  srand((unsigned)time(0));

  int guessNum;

  guessNum=rand()%6;

  switch(guessNum){
    case 0:
    cout<<"You should eat a hamburger today."<<endl;

    case 1:
    cout<<"You should eat a pizza today."<<endl;

    case 2:
    cout<<"You should eat a sub today."<<endl;

    case 3:
    cout<<"You should eat a hot dog today."<<endl;

    case 4:
    cout<<"You should eat a doughnut today."<<endl;

    case 5:
    cout<<"You should eat a pancake today."<<endl;

  }
  return 0;
}
