#ifndef CARDS_H
#define CARDS_H

#include <stdbool.h>

typedef struct Card {
	char face;
	char suit;
	char style; // filler character for text renderer

	// If hidden, the renderer will not show card value
	bool hidden;
} Card;

typedef struct CardNode {
	Card* card; // Pointer to card struct
	struct CardNode* next; // Next CardNode in list
	
	// For first node
	bool first;
	struct CardNode* head; // All nodes will have a head pointer
	int size; // Size of deck is stored in first node
} CardNode;

typedef struct Deck {
	Card** cards; // Array of card pointers
	int size; // Number of cards in the deck
	int position; // Current position in the deck
} Deck;

Card * randomCard();
int cardToInt(Card* card); // Fetch the 

CardNode* initNewHand(Deck* deck);
int getHandTotal(CardNode* nxt); // Get total value of a hand
void addCardNode(CardNode* fptr, Deck* deck); // Add a new cardNode to a hand
void clearHand(CardNode* nxt); // Clears a given hand from memory

Deck* newDeck(int numDecks);
void shuffleDeck(Deck* deck);
Card* drawCard(Deck* deck);
void clearDeck(Deck* deck);

#ifdef DEBUG
	void printDeck(Deck* deck);
#endif

#endif