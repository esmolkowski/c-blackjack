#ifndef CARDS_H
#define CARDS_H

typedef struct Card {
	char face;
	char suit;
    char style; // filler character for text renderer

    // If hidden, the renderer will not show card value
	_Bool hidden;
} Card;

typedef struct CardNode {
	struct Card *cptr; // Pointer to card struct
	struct CardNode *next; // Next CardNode in list
	
    // For first node
	_Bool first;
	struct CardNode *head; // All nodes will have a head pointer
	int size; // Size of deck is stored in first node
} CardNode;

Card * randomCard();
int cardToInt(Card *card); // Fetch the 

CardNode * initNewList();
int getTotal(CardNode * nxt); // Get total value of a hand
void addNode(CardNode * fptr); // Add a new cardNode to a hand
void clearHand(CardNode * nxt); // Clears a given hand from memory

#endif