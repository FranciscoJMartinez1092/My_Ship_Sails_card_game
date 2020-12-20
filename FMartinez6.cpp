#include<iostream>
#include<vector>
#include<random>
#include<iomanip>
#include<ctime>
#include<algorithm>

using namespace std;

struct Card 
{
	char suit; 
	string number;
};

class Player //base class
{
	
	protected:

	int selfIndex;        //player number
	int takeFrom;              //player who will give new card
	vector<Card> hand;         //vector that holds card structs
	static Card discardList[5];
	int discardIndex;   //hand position of card to be discarded
	
	public:
	Player(int SelfI, int TakeF):	selfIndex(SelfI),takeFrom(TakeF)
	{}
	
	virtual void  chooseDiscard(int);
	void replenish(); //replenishes card
	
	int getSelfIndex(); //accesor function for selfindex
	void addCard(Card ); 
	vector<Card> gethand(); //accesor for hand variable
	int winner(); //checks for winner
	
	
};
Card Player::discardList[5]={ };

/* takes position parameter and uses that value to discard the card at that position*/
void Player::chooseDiscard(int position) 
{
	discardIndex = position;
	discardList[selfIndex] = hand[position];
		
		cout << "\tPlayer " << selfIndex << " gave "<< selfIndex+1 << " " << hand[position].suit + hand[position].number <<endl;
		
}
  /*function that gives card to player in the position where they gave their card */
void Player::replenish() 
{
	hand[discardIndex] = discardList[takeFrom];
}

int Player::getSelfIndex() //accesor for selfIndex variable
{
	return selfIndex;
}

vector<Card> Player::gethand() // accesor for hand variable
{
	return hand;
}

void Player::addCard(Card newcard) // gets paramter new card and adds it to hand vector
{
	hand.push_back(newcard);
}

 int Player::winner() // checks to see if a player has all the suits to win 
{	int count = 0;
	int iswinner = 0;
		
		for(int i = 0;i <= 6;i++)
		{
			if(hand[i].suit == hand[i + 1].suit)
			{
				count++;
			}
		}
		if(count == 6)
		{
			cout<<"Player "<< selfIndex <<" is a winner!\n";
	
			iswinner = 1;
			
			
		}
		return iswinner;
		
}

class Autoplayer : public Player //derived class for ai
{
	public :
	Autoplayer( int SelfI, int TakeF):Player(SelfI,TakeF)
	{	}
	
	void chooseDiscard(int);
	
};
/*ai for autoplayer (players that are not 0) that decides which card to discard.
It checks for the lowest amount of suit to give*/
void Autoplayer::chooseDiscard(int pos) 
{	string suits = "SHDC"; //string with all types of suit
	vector<int> suitcount; //a vector that counts the amount of suit type in hand
	int count; 
	int oldlength = 0;
	
	for(int i = 0; i < suits.size(); i++)
	{	
		
		if(oldlength > suits.size())
		{	oldlength--;
			i = 0;
		}
		count = 0;
		
		for(int j = 0; j < hand.size();j++)
		{
			if(suits[i] == hand[j].suit)
			{
				count++;
			}
		}
		
		if(count == 0)
		{	
			oldlength = suits.size();
			suits.erase(suits.begin() + i);
			
		}
		
	}
	for(int i = 0;i < suits.size(); i++) //counts suits number and pushback to suitcount
	{	
		count = 0;
		for(int j = 0;j < hand.size();j++)
		{
			if(suits[i] == hand[j].suit)
			{
				count++;
			}
		}
		if(count == 0)
		{
			suits.erase(suits.begin() + i);
		}
		else
		{
			suitcount.push_back(count);
		}
	}
	/* loop that finds the index for the suit with the minimum count*/
	for(int i = 0; i < suitcount.size(); i++) 
	{
		if(suitcount[i] == *std::min_element(suitcount.begin(),suitcount.end()))
		{
			pos = i;
			break;
		}
	}
	//chooses the card to discard by comparing position. Then displays appropriate message
	for(int i = 0; i < hand.size();i++) 
	{
		if(suits[pos] == hand[i].suit)
		{
			discardList[selfIndex] = hand[i];
			discardIndex = i;
			if(selfIndex == 4)
				{
					cout<< "\tPlayer " << selfIndex <<" gave " << 0 << " " << hand[i].suit+hand[i].number<<endl;
				}
				else
				{
					cout << "\tPlayer " << selfIndex << " gave " << selfIndex + 1 << " " << hand[i].suit+hand[i].number <<endl;
				}
				break;
		}
	}
	

	
}

/* function used to show hands*/
void showhands(vector<Player*> players, int amount) 
{

	for(int i = 0;i < amount;i++)
	{
		cout << players.at(i)->getSelfIndex() << ":\t"; //displayes self index by derefencing players
		
		for(int j = 0; j < 7; j++)
		{
		
			cout << setw(6) << players.at(i)->gethand()[j].suit+players.at(i)->gethand()[j].number;
			
		}
		cout<< endl;
	}

	return;
	
}

int main()
{
	Card buffercard;
	int wincount = 0;
	vector<Card> deck;
	string suits = "SHDC";
	string index = "abcdefg";
	
	
	srand((unsigned) time(0));
	//create a vector consisting of pointers of player class
	vector<Player*> players;
	//creates the players
	Player zero(0,4);
	Autoplayer one(1,0);
	Autoplayer two(2,1);
	Autoplayer three(3,2);
	Autoplayer four(4,3);
	//puts all players pointers in vector of player pointers
	players.push_back(&zero);
	players.push_back(&one);
	players.push_back(&two);
	players.push_back(&three);
	players.push_back(&four);
	
	//loop that creates the deck of 52 cards
	for (int i = 0; i < 4;i++) 
	{
		buffercard.suit = suits[i];
		for(int j = 1;j <= 13;j++)
		{
			if(j > 10)
			{
				switch(j%10)
				{
					case 1: buffercard.number = 'J';break;
					case 2: buffercard.number = 'Q';break;
					case 3: buffercard.number = 'K';break;
					default: break;
				};
			}
			else
			{
				buffercard.number = to_string(j);
			}
			deck.push_back(buffercard);
		}
	}
	
	
	random_shuffle(deck.begin(),deck.end()); // shuffles the deck
	
	for(int i = 0;i < 5; i++) //assigns the players their cards
	{
		
		for(int j = 0;j < 7;j++)
		{
			int randomcards = rand()%deck.size();
			players.at(i)->addCard(deck[randomcards]);
			deck.erase(deck.begin()+randomcards);
			
			
		}
	}
	char input;
	int count = 0;
	
	do
	{
		/*outputs the game on the screen and asks for input. Can take ? or from a to g.
		if ? displays the hand vectors. If a to g then it discards the card and hands it to next player*/
		cout << "Your hand:\n";
		showhands(players,1);
		cout << "\t";
		
		for(int i = 0;i < index.length(); i++) // showing indices in alphabets
			{
				cout << setw(6) << index[i];
			}
			cout << endl;
		do
		{	
		
		cout << "cmd: ";
			cin >> input;
			if(input == '?') 
			{
				showhands(players,5);
				continue;
			}
			else if((int)input > 96 && (int)input < 104)
			{
				count++;
			}
			else if ((int)input <= 96 || (int)input >= 104)
			{
				cout<< "Invalid choice\n";
			}
		}while(count < 1);
		count = 0;
		//discards card
		for(int i = 0; i< 5; i++)
		{
			players.at(i)->chooseDiscard(index.find(input));
			
		}
		for(int i = 0; i < 5; i++) //replenishes at poistion where the card was discarded
		{
			players.at(i)->replenish();	
		}
		for(int i = 0;i < 5;i++)
		{	
			if(players.at(i)->winner())// gets win counter if player collects all of one suit in hand 
			{
				
				wincount++;
			}
		}
	}while(wincount == 0); //exits game when you get a win counter when player collect all of one suit in hand
	cout<< "At the end of the game the players had:\n";
	showhands(players,5);//shows the hands that players hand at the end
return 0;
}