#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

class FlashCard
{
public:
 int a,b;
 void PrintCard(void);
 int CorrectAnswer(void);
};

void FlashCard::PrintCard(void){
	std::cout<<a<<'x'<<b<<'=';
}

int FlashCard::CorrectAnswer(void){
	return a*b;
}

void SwapFlashCard(FlashCard &Card1, FlashCard &Card2)
{
	FlashCard Card3;

	Card3.a=Card1.a;
	Card3.b=Card1.b;

	Card1.a=Card2.a;
	Card1.b=Card2.b;

	Card2.a=Card3.a;
	Card2.b=Card3.b;
}

void ShuffleCards(FlashCard card[144])
{

	//creating set
	int nRows=12;
	int nCols=12;
	for(int i=0; i<nRows; i++){
		for(int j=0; j<nCols; j++){
			card[i*nCols+j].a=i+1;
			card[i*nCols+j].b=j+1;
		}
	}

	for(int i=0; i<144; i++)
	{
		int r=rand()%144;
		SwapFlashCard(card[i],card[r]);
	}
}

int main(void)
{
	FlashCard card[144];
	int nCard;
	char nCardString[64];
	int i;
	unsigned int t0;

	for(;;)
	{
		printf("How many cards to work on?\n");
		printf(">");
		fgets(nCardString,64,stdin);
		nCard=atoi(nCardString);

		if(nCard<1 || 144<nCard)
		{
			printf("The number of cards must be between 1 and 144.\n");
		}
		else
		{
			break;
		}
	}

	t0=time(NULL);
	srand(t0);

	ShuffleCards(card);

	int nCorrect=0;
	for(i=0; i<nCard; i++)
	{

		card[i].PrintCard();

		int answer;
		char answerString[64];

		
		fgets(answerString,64,stdin);
		answer=atoi(answerString);

		if(card[i].CorrectAnswer()==answer)
		{
			printf("Correct!\n");
			nCorrect++;
		}
		else
		{
			printf("Wrong!  Correct answer is %d\n",card[i].CorrectAnswer());
		}
	}

	printf("You have worked on %d problems.\n",nCard);
	printf("You answered %d problems correctly. (%d%%)\n",nCorrect,nCorrect*100/nCard);
	printf("You spent %d seconds to answer all problems.\n",time(NULL)-t0);

	return 0;
}
