#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
using namespace std;

enum class Rank {
	ZERO = 0,
	ACE = 1, TWO, THREE, FOUR, FIVE,
	SIX, SEVEN, EIGHT, NINE, TEN,
	JACK, QUEEN, KING

};
enum class Type { EMPT = 0, CLUBS, DIAMONDS, HEARTS, SPADES };



// Card: with rank and type
class Card {
private:
	Rank rank;
	Type type;
public:
	Card();
	Card(Rank r, Type t);
	int getValue();
	void displayCard();
	
};

// Hand: represents a set of cards that the players holds.
class Hand{
private:
	// a hand of the vector
	vector<Card> aHand;
public:
	Hand();
	// add a card to the hand
	void add(Card a);
	// clear
	void clear();
	// get total points
	int getTotal();
	//print the hand with the sum at the end
	void printHand();
};


// Deck: represents the deck of cards and the actions that can be performed on the cards
class Deck {
private:
	// a deck of cards
	vector<vector<Card> > aDeck ;
	// pointers, easier to shuffle and re-use
	vector<Card*> aDeckp;
	// call Populate to initialize a deck
	void Populate();
public:
	//constructor
	Deck();
	// shuffle the pointer
	void shuffle();
	// print the deck, used for checking
	void printDeck();
	// deal a card from deck
	Card deal();
	// claen up pointers
	void clear();
};



// AbstractPlayer: an abstract class represending general players
class AbstractPlayer{
protected:
	Hand the_Hand;
public:
	// whether the play decide to draw or not
	virtual bool isDrawing() const = 0;
	//destructor
	virtual ~AbstractPlayer() {};
	// whether the player is busted
	bool isBusted();
	// method to add one card
	void drawOne(Card a);
	// print the hand
	void display_all_hand();
	// get total score
	int getTotalScore();
        // clear the hands
        void clearHand();
	
};


// HumanPlayer: a class represents human player,
class HumanPlayer: public AbstractPlayer {
private: 
	Hand the_Hand;
public:
	// method override
	virtual bool isDrawing() const override;// implicitly virtual
	// announce for win / loss / push
	void announce(char situation);
	// score getter const
	int getScore();
	//dis play hand
	void displayHand();
};

//Computer Player: a class represents computer player
class ComputerPlayer: public AbstractPlayer {
private:
	Hand the_Hand;
	int score;
public:
	// method override
	virtual bool isDrawing() const override; // implicitly virtual
	// score getter const
	int getScore() const;
	// update the score of computer
	void updateScore();
	// display hand
	void displayHand();
	
	
};


// Black Jack Game: the class for the whole process
class BlackJackGame{

private:
	// a deck shared by both players
	Deck m_deck;
	// a human player
	HumanPlayer m_human;
	// a computer player
	ComputerPlayer m_casino;
public:
	BlackJackGame();
	// play function
	void play();

};













