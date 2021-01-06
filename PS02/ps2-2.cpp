#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

int FindAnswer(string question);
void SwapCards(string& a, string& b);
void ShuffleDeck(string MySet[],int SetTotal);

int main(){

  srand(time(nullptr));

  string MySet[144];
  int nRows=12;
  int nCols=12;

  //creating set
  for(int i=0; i<nRows; i++){
    for(int j=0; j<nCols; j++){
      MySet[i*nCols+j]=to_string(i+1)+'x'+to_string(j+1);
    }
  }

  ShuffleDeck(MySet,144);

  //Prompting User
  int numQuestions;
  cout<<"How many cards would you like to work on? ";

  while(true){
    cin>>numQuestions;

    if(numQuestions<=144 && numQuestions>=1) break;

    cout<< "The number of cards must be between 1 and 144. Enter another number: ";
  }

  int userAnswer;
  int correctAnswer;
  int numCorrect=0;;

  //Starting timer
  time_t begin_time=time(nullptr);

  //Looping until all desired questions are answered
  for(int i=0; i<numQuestions; i++){
    cout<<"What is "<< MySet[i]<<"? ";
    correctAnswer=FindAnswer(MySet[i]);
    cin>>userAnswer;

    if(userAnswer==correctAnswer){
      numCorrect++;
    }
    else{
      cout<<"Wrong answer. The correct answer is "<<correctAnswer<<endl;
    }
  }

  //Ending timer
  time_t dt=(time(nullptr) - begin_time);

  int score=100*numCorrect/numQuestions;

  cout<<"You answered " << numQuestions << " problems in "<< dt <<" seconds."<<endl;
  cout<<"You answered " << numCorrect << " problems correctly (" <<score<<"%)."<<endl;

}

void SwapCards(string& a, string& b){
  string c;
  c=b;
  b=a;
  a=c;
}

void ShuffleDeck(string MySet[],int SetTotal){

  int n;
  srand((unsigned)time(0));

  for(int i=0; i<SetTotal; i++){
    n=rand()%SetTotal;
    SwapCards(MySet[i],MySet[n]);
  }
}

int FindAnswer(string question){

  string arg1="";
  string arg2="";
  int correctAnswer;

  //Parsing first argument only
  int c=0;
  while(question[c]!='x'){
    arg1=arg1+question[c];
    c++;
  }

  //Skipping x character
  c++;

  //Parsing second argument
  for(int i=c; i<question.size(); i++){
    arg2=arg2+question[i];
  }

  //Convert string arguments to integer arguments to calculate
  correctAnswer=stoi(arg1)*stoi(arg2);

  return correctAnswer;

}
