#include "blackjack.h"
using namespace std;

int rank_size = 13;
int type_size = 4;

// lists for the types, not including the zero-empt card;
Rank Rank_all[13] = { Rank::ACE, Rank::TWO, Rank::THREE, Rank::FOUR, Rank::FIVE,
					Rank::SIX, Rank::SEVEN, Rank::EIGHT, Rank::NINE, Rank::TEN,
					Rank::JACK, Rank::QUEEN, Rank::KING };
Type Type_all[4] = { Type::CLUBS, Type::DIAMONDS, Type::HEARTS, Type::SPADES };



// =============== Functions in the Card class =============== 

// card constructor
Card::Card(Rank r, Type t) {
	this->rank = r;
	this->type = t;
}
Card::Card() {
	this->rank = Rank::ZERO;
	this->type = Type::EMPT;
}
// get the pre-value of the card
int Card::getValue() {
	int value = 0;

	switch ((int)rank) {
	case 1:
		//A
		value += 1000;	// the value indicating there is an A in the card
		break;
	case 11:
		//J
		value += 10;
		break;
	case 12:
		//Q
		value += 10;
		break;
	case 13:
		//K
		value += 10;
		break;
	default:
		value += (int)rank;
		break;
	}

	return value;
}

// print card information
void Card::displayCard() {
	//get the rank text
	char rank_1[3] = { 'z', '\0', '\0' };
	if ((int)rank < 10) {
		rank_1[0] = '0' + (int)rank;
	}
	else {
		switch((int)rank){
		case 10:
			rank_1[0] = '1';
			rank_1[1] = '0';
			break;
		case 11:
			rank_1[0] = 'J';
			break;
		case 12:
			rank_1[0] = 'K';
			break;
		case 13:
			rank_1[0] = 'Q';
			break;
		}
	}

	// get the first of the type
	char type_1[2] = { 'e' , '\0'};
	switch ((int)type) {
	case 1:
		type_1[0] = 'C';
		break;
	case 2:
		type_1[0] = 'D';
		break;
	case 3:
		type_1[0] = 'H';
		break;
	case 4:
		type_1[0] = 'S';
		break;
	}

	cout << rank_1 << type_1;
}



// =============== Functions in the Hand class =============== 
//constructor


Hand::Hand() {
	/*Note:
		the maximum number of cards one can have before he lose would be 11:

		1*4 = 4
		2*4+4 = 12
		3*4+12 = 24 -> bust
		3*3+12 = 21 -> right there
		Therefore There is maximum of 4+4+3 = 11 cards.

		After getting 11 cards, the player will either win or lose.
	*/
	
	vector<Card> aHand(11);
	this->aHand = aHand;
}


// gets a card form the deck
void Hand::add(Card a) {
	//cout << "Added to Hand" << endl;
	//a.displayCard();
	//cout << endl;
	aHand.push_back(a);
}


// clean up the vector
void Hand::clear() {
	for (vector<Card>::iterator iter = aHand.begin(); iter != aHand.end(); iter++) {
		delete (&iter);
	}
	aHand.clear();
}

// get total score
int Hand::getTotal() {
	int score = 0;
	int raw_score = 0;

	vector<Card>::iterator iter, end;
	for (iter = aHand.begin(), end = aHand.end(); iter != end; ++iter) {
		Card pt = *iter;
		raw_score += pt.getValue();
	}

	// asess whether there are A inside
	int k = raw_score / 1000;

	if (k > 0) {
		// if there is, see which score is better
		int tmp = raw_score - k * 1000;	// what's the socre other than the ACE

		// 1. calculate A as 1
		int score_1 = k * 1 + tmp;
		// 2. calculate A as 11
		int score_11 = k * 11 + tmp;

		//check if either of them are 21
		if ((score_1 ==21) || (score_11 == 21)) {
			score = 21;
		}
		else {
			// else, use the minimum of the two
			if (score_1 < score_11) {
				score = score_1;
			}
			else {
				score = score_11;
			}
		}
		
	}
	else {
		score = raw_score;
	}

	return score;
}

void Hand::printHand() {
	//print the hand with the sum at the end
	vector<Card>::iterator iter, end;
	for (iter = aHand.begin(), end = aHand.end(); iter != end; ++iter) {
		Card pt = *iter;
		if (pt.getValue() > 0) {
			pt.displayCard();
			cout << " ";
		}
	}

	cout << "[" << getTotal() << "]" << endl;
}

// =============== Functions in the Deck class =============== 

int dealt = 0;
// Constructor
Deck::Deck() {
	// get the decks fist
	Populate();

	// a vector of pointser to the card
	vector<Card*> aDeckp;

	// get a flat list of pointers to all decks, easier to shuffle
	for (Rank r : Rank_all) {
		for (Type t : Type_all) {
			aDeck[((int)r) - 1][((int)t) - 1] = Card(r, t);
			Card* ptr = &(aDeck[((int)r) - 1][((int)t)-1]);
			aDeckp.push_back(ptr);
		}
	}

	// copy the deck to our private attributes
	this->aDeckp = aDeckp;

	//shuffle the pointers, ensure all decks are shuffled before game
	shuffle();

	//check deck
	//printDeck();
}

// create a deck
void Deck::Populate() {
	// initialize the vector of all cards
	vector<vector<Card> > aDeck (rank_size, vector <Card> (type_size, Card(Rank::ACE, Type::CLUBS)));

	// singleton design pattern
	for (Rank r : Rank_all) {
		for (Type t : Type_all) {
			aDeck[((int)r)-1][((int)t) - 1] = Card(r, t);
		}
	}
	this->aDeck = aDeck;
}

// display the deck, used to check
void Deck::printDeck() {

	// printing with deck
	/*for (Rank r : Rank_all) {
		for (Type t : Type_all) {
			Card a = aDeck[((int)r)-1][(int)t];
			a.displayCard();
		}
	}*/
	// printing with card pointers
	for (vector<Card*>::iterator pObj = aDeckp.begin();
		pObj != aDeckp.end(); ++pObj) {
		Card* pt = *pObj;
		Card p = *pt;
		p.displayCard();
	}

}

// shuffle the cards
void Deck::shuffle() {

	// shuffle the pointblackjack.h:76:1:er to the cards
	srand(unsigned(time(0)));
	random_shuffle(aDeckp.begin(), aDeckp.end());
}

// return one card for the hand
Card Deck::deal() {

	//find the last element
	Card* pt = aDeckp.back();
	Card p = *pt;
	aDeckp.pop_back();

	//resize to avoid null pointers
	dealt++;
	aDeckp.resize(rank_size*type_size - dealt);

	// return a card
	return p;
}

void Deck::clear() {
	for (auto p : aDeckp) {
		delete p;
	}
	aDeckp.clear();
}



// =============== Functions in the AbstractPlayer class =============== 


bool AbstractPlayer::isBusted() {
	if (the_Hand.getTotal() > 21) {
		return true;
	}
	else {
		return false;
	}
}

void AbstractPlayer::drawOne(Card a) {
	the_Hand.add(a);
}

void AbstractPlayer::display_all_hand() {
	the_Hand.printHand();
}

int AbstractPlayer::getTotalScore() {
	return the_Hand.getTotal();
}

void AbstractPlayer::clearHand() {
	the_Hand.clear();
}


// =============== Functions in the HumanPlayer class =============== 

// disply the hand of human
void HumanPlayer::displayHand() {
	cout << "Player: ";
	display_all_hand();
}

//isDrawing function
bool HumanPlayer::isDrawing() const{
	char answer;
	//bool isType = false;
	cout << "Do you want to draw? (y/n):";
	while (true) {
		cin >> answer;
		if (answer == 'y') {
			return true;
		}
		else if (answer == 'n') {
			return false;
		}
		else {
			// if no return occurs, loop the while
			cout << "Please select y/n";
		}
	}
}

int HumanPlayer::getScore() {
	return getTotalScore();
}

//depending on input, announce
void HumanPlayer::announce(char situation) {
	switch (situation) {
	case 'w':
		cout << "Player wins." << endl;
		break;
	case 'l':
		cout << "Player loses." << endl;
		break;

	case 'b':
		cout << "Player busts." << endl;
		cout << "Casino wins." << endl;
		break;

	case 'c':
		cout << "Casino busts." << endl;
		cout << "Player wins." << endl;
		break;

	case 'd':
		cout << "Casino wins." << endl;
		break;
	case 'e':
		cout << "Casino loses." << endl;
		break;

	case 't':
		cout << "No one wins.";
		break;
	};
}


// =============== Functions in the ComputerPlayer class =============== 


// disply the hand of computer
void ComputerPlayer::displayHand() {
	cout << "Computer: ";
	display_all_hand();
}

//isDrawing method
bool ComputerPlayer::isDrawing() const {
	if (this->getScore() > 16) {
		return false;
	}
	else {
		return true;
	}
}


// const getter method for the isDrawing
int ComputerPlayer::getScore() const {
	return score;
}

// update score
void ComputerPlayer::updateScore() {
	score = getTotalScore();
}



// =============== Functions in the BlackJackGame class =============== 
// Constuctor
BlackJackGame::BlackJackGame() {

	// a deck shared by both players
	Deck m_deck;
	// a human player
	HumanPlayer m_human;
	// a computer player
	ComputerPlayer m_casino;


}

void BlackJackGame::play() {
    
	// a deck shared by both players
	m_deck = Deck();
	// a human player
	m_human = HumanPlayer();
	// a computer player
	m_casino = ComputerPlayer();

	// add two cards for player
	m_human.drawOne(m_deck.deal());
	m_human.drawOne(m_deck.deal());

	// deal two cards for computer
	m_casino.drawOne(m_deck.deal());

	m_human.displayHand();
	m_casino.displayHand();

	bool isEnded = false;
	//player's round
	while (true) {
		// check if busted
		if (m_human.isBusted()) {
			m_human.announce('b');
			return;
		}
		// check if 21
		if (m_human.getScore() == 21) {
			m_human.announce('w');
			return;
		}

		bool answer_hu = m_human.isDrawing();
		if (answer_hu) {
			m_human.drawOne(m_deck.deal());
			m_human.displayHand();
		}
		else{
			cout << "Player decided to sotp drawing" << endl;
			break;
		}
		

		// check if busted
		if (m_human.isBusted()) {
			m_human.announce('b');
			return;
		}
		// check if 21
		if (m_human.getScore() == 21) {
			m_human.announce('w');
			return;
		}
	}


	//Computer's round
	while (true) {
		// check if busted
		if (m_casino.isBusted()) {
			m_human.announce('c');
			return;
		}
		// check if 21
		if (m_casino.getScore() == 21) {
			m_human.announce('d');
			return;
		}

		bool answer_co = m_casino.isDrawing();
		if (answer_co) {
			m_casino.drawOne(m_deck.deal());
			m_casino.updateScore();
			m_casino.displayHand();
		}
		else {
			cout << "Casino decided to sotp drawing" << endl;
			break;
		}

		// check if busted
		if (m_casino.isBusted()) {
			m_human.announce('c');
			return;
		}
		// check if 21
		if (m_casino.getScore() == 21) {
			m_human.announce('d');
			return;
		}

	}


	// after the while loop
	// 1. the larger one wins
	if (m_human.getScore() > m_casino.getScore()) {
		
		m_human.announce('w');
	}
	else {
		m_human.announce('l');
	}

	// 2. smae, then no winners (tie)
	if (m_human.getScore() == m_casino.getScore()) {
		m_human.announce('t');
	}
	
	// depete pointer of deck
	//m_deck.clear();
        // delete hands of players
       // m_casino.clearHand();
       // m_human.clearHand();

}
