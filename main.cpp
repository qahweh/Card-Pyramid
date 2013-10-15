#include <ncurses.h>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <time.h>
using namespace std;

class Card
{
	private:
	int x;

	public:
	Card(int y)
	{
		x = y;
	};
	int getValue()
	{
		return (x%13)+2;
	};
	int getSuit()
	{
		return (x/13) +1;
	};
	int getRaw()
	{
		return x;
	};
	string* toString()
	{
		string *s = new string("]");
		if(this->getValue()==14)s->insert(0," A");
		if(this->getValue()==13)s->insert(0," K");
		if(this->getValue()==12)s->insert(0," Q");
		if(this->getValue()==11)s->insert(0," J");
		if(this->getValue()==10)s->insert(0,"10");
		if(this->getValue()==9)s->insert(0," 9");
		if(this->getValue()==8)s->insert(0," 8");
		if(this->getValue()==7)s->insert(0," 7");
		if(this->getValue()==6)s->insert(0," 6");
		if(this->getValue()==5)s->insert(0," 5");
		if(this->getValue()==4)s->insert(0," 4");
		if(this->getValue()==3)s->insert(0," 3");
		if(this->getValue()==2)s->insert(0," 2");
		if(this->getSuit()==1)s->insert(0,"[c");
		if(this->getSuit()==2)s->insert(0,"[s");
		if(this->getSuit()==3)s->insert(0,"[h");
		if(this->getSuit()==4)s->insert(0,"[d");
		return s;
	}

};

int pyramid[8][8];
	Card *currentCard = new Card(rand()%52);

bool canPlaceHere(int x, int y)
{
	if(pyramid[x][y]>-1)return false;
	if(x==0 && y==0)return true;
	if(pyramid[x-1][y]>-1 && y==0)return true;

	if(y>0)
	{
		if(pyramid[x+1][y-1]>-1 && pyramid[x][y-1]>-1)
		{
			Card* leftCard = new Card(pyramid[x][y-1]);
			Card* rightCard = new Card(pyramid[x+1][y-1]);

			if(leftCard->getValue() == currentCard->getValue())return true;
			if(rightCard->getValue() == currentCard->getValue())return true;

			if(leftCard->getValue() < currentCard->getValue() && currentCard->getValue() < rightCard->getValue())return true;
			if(leftCard->getValue() > currentCard->getValue() && currentCard->getValue() > rightCard->getValue())return true;
		}
	}

	return false;
}
	bool usedCards[52];
int getScore()
{
	int s=0;
	for(int x=0; x<52; x++)
	{
		if(usedCards[x])s++;
	}
	return s;
}

string getScoreString()
{
	int i = getScore();
	int i1 = i/10;
	int i2 = i%10;
	string s("");
	if(i2==0)s.insert(0,"0");
	if(i2==1)s.insert(0,"1");
	if(i2==2)s.insert(0,"2");
	if(i2==3)s.insert(0,"3");
	if(i2==4)s.insert(0,"4");
	if(i2==5)s.insert(0,"5");
	if(i2==6)s.insert(0,"6");
	if(i2==7)s.insert(0,"7");
	if(i2==8)s.insert(0,"8");
	if(i2==9)s.insert(0,"9");

	if(i1==0)s.insert(0,"0");
	if(i1==1)s.insert(0,"1");
	if(i1==2)s.insert(0,"2");
	if(i1==3)s.insert(0,"3");
	if(i1==4)s.insert(0,"4");
	if(i1==5)s.insert(0,"5");
	return s;
}

Card* drawNewCard()
{
    Card *c = new Card(rand()%52);
	while(usedCards[c->getRaw()])
	{
	    c = new Card(rand()%52);
	}
	usedCards[c->getRaw()] = true;
    return c;
}

int main()
{


	initscr();
	srand(time(NULL));

	restart:
	currentCard = new Card(rand()%52);
	bool firstFrame = true;

	int cx = 3;
	int cy = 2;

	for(int x=0; x<8; x++)
	{
		for(int y=0; y<8; y++)
		{
			pyramid[x][y]=-1;
		}
	}

	for(int x=0; x<52; x++)
	{
		usedCards[x] = false;
	}

	refresh();
	while(int f = ( firstFrame ? -1 : getch()))
	{
		if(f==100)cx++;
		if(f==97)cx--;
		if(f==119)cy++;
		if(f==115)cy--;
		if(f==110)
		{
			goto restart;
		}
		if(f==98 && canPlaceHere(cx,cy))
		{
			pyramid[cx][cy] = currentCard->getRaw();
			currentCard = drawNewCard();
		}
		if(cx<0)cx=0;
		if(cy<0)cy=0;
		if(cy>7)cy=7;

		if(cx>8-cy-1)cx=8-cy-1;

	for(int i=0; i<8; i++)
	{
		for(int u=0; u<8-i; u++)
		{
			move(8-u,i*6+u*3);	
			if(cx==i && cy==u)
			printw(currentCard->toString()->c_str());
			else
			{
				if(pyramid[i][u]==-1)
				{
					if(canPlaceHere(i,u)) printw("[   ] ");
					else printw("[ x ] ");
				}
				else 
				{
					Card* card = new Card(pyramid[i][u]);
					printw(card->toString()->c_str());
				}
			}
		}
	}
	move(10,10);
	printw("score: ");
	move(10,18);
	printw(getScoreString().c_str());
		firstFrame = false;
	move(15,15);

	}
	
	endwin();
	return 0;
}
