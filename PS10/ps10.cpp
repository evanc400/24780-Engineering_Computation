#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;


bool isNumber(string c);
bool isSpare(string c);
bool isStrike(string c);
bool isMiss(string c);
int scoreFromFrame(vector<int> gameInt, string gameString,int &i, int &f);
int scoreFromLastFrame(vector<int> gameInt, string gameString,int &i);

int main(){

  cout<<"Enter bowling string: ";
  string gameString;
  getline(cin,gameString);
  //gameString="9/72X-X-8-919/9/9-XX6";
  //gameString="9-9-9-9-9-9-9-9-9-9--";


  if(gameString.size()<20){
    cout<<"Too Short\n";
    return 0;
  }

  std::vector<int> gameInt;
  string c;
  cout<<gameString<<endl;

  for(int i=0; i<gameString.size(); i++){
    c=gameString[i];
    if(isNumber(c)){
      gameInt.push_back(stoi(c));
    }
    else if(isSpare(c)){
      gameInt.push_back(10-gameInt[i-1]);
    }
    else if(isStrike(c)){
      gameInt.push_back(10);
    }
    else if(isMiss(c)){
      gameInt.push_back(0);
    }
    else{
      cout<<"Wrong Character \n";
      return 0;
    }
  }

  int iBall=0;
  int iFrame=0;
  int score=0;

  while(iFrame<9){

    score=score+scoreFromFrame(gameInt, gameString, iBall, iFrame);
    iFrame++;
    iBall=iBall+2;
  }
  score=score+scoreFromLastFrame(gameInt, gameString, iBall);


  cout<<"FINAL SCORE: "<< score << endl;

}

bool isNumber(string c){
  if(c=="0" || c=="1" ||c=="2" ||c=="3" ||c=="4" ||c=="5" ||c=="6" ||c=="7" ||c=="8" ||c=="9"){
    return true;
  }
  return false;
}

bool isSpare(string c){
  if(c=="/"){
    return true;
  }
  return false;

}

bool isStrike(string c){
  if(c=="X" || c=="x"){
    return true;
  }
  return false;

}

bool isMiss(string c){
  if(c=="-" || c==" "){
    return true;
  }
  return false;

}

int scoreFromFrame(vector<int> gameInt, string gameString,int &i, int &f){

    string c;

    int result=0;
    int first=i;
    int second=i+1;
    int firstNext=i+2;
    int secondNext=i+3;

    int XFirstNext=i+2;
    int XSecondNext=i+3;

    //no strike
    c=gameString[first];
    if(!isStrike(c)){
      result=result+gameInt[first];
      //second shot, no spare
      c=gameString[second];
      if(isNumber(c) || isMiss(c)){
        result=result+gameInt[second];

      }
      //second shot, spare
      if(isSpare(c)){
        //shots after spare
        c=gameString[firstNext];

        if(isStrike(c)){
          result=20;

        }
        else{
          result=10+gameInt[firstNext];

        }
      }

      //no strike, so uses both balls
      //i=i+2;
      //if(f==8) i=i+1;
      //else i=i+2;
      //f++;
      return result;
    }

    //strike
    else{
      c=gameString[XFirstNext];
      //first shot after strike

      if(isNumber(c) || isMiss(c)){
        result=10+gameInt[XFirstNext];


      }

      else if(isStrike(c)){
        result=20;
        XSecondNext++;

      }

      //second shot after strike
      c=gameString[XSecondNext];
      if(isNumber(c) || isMiss(c)){
        result=result+gameInt[XSecondNext];

      }
      //second shot if its a spare
      else if(isSpare(c)){
        result=result+gameInt[XSecondNext];

      }

      //second shot is a strike
      else{
        result=result+gameInt[XSecondNext];

      }

      //strike, only used one ball
      //i=i+1;
      //f++;
      return result;
    }

}


int scoreFromLastFrame(vector<int> gameInt, string gameString,int &i){
  string c;

  int result=0;
  int first=i;
  int second=i+1;
  int third=i+2;

  //no strike
  c=gameString[first];

  if(!isStrike(c)){
    result=result+gameInt[first];
    //second shot
    c=gameString[second];

    //second shot no spare, game ends
    if(isNumber(c) || isMiss(c)){
      result=result+gameInt[second];
      return result;
    }
    //second shot, spare, add third score
    if(isSpare(c)){
      //shots after spare
      c=gameString[third];

      if(isNumber(c) || isMiss(c)){
        result=10+gameInt[third];
      }

      if(isStrike(c)){
        result=20;

      }
    }
  }

  //strike
  else{
    c=gameString[second];
    //no strike first shot after final frame strike
    if(isNumber(c) || isMiss(c)){
      result=10+gameInt[second];
      c=gameString[third];
      if(isSpare(c)){
        result=20;
        return result;
      }
      if(isNumber(c) || isMiss(c)){
        result=result+gameInt[third];
        return result;
      }

    }
    else if(isStrike(c)){
      result=20;
      c=gameString[third];
      if(isStrike(c)){
        result=30;
        return result;
      }
      if(isNumber(c) || isMiss(c)){
        result=result+gameInt[third];
        return result;
      }

    }

  }

  return result;

}
