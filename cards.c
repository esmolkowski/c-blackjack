#include <stdlib.h>
#include "cards.h"

int getHandTotal(CardNode * nxt) {
	// Fetch score of a hand
	// nxt: first cardNode in hand

	CardNode * f;
	f = nxt;
	// Loop through all cards in hand
	int total = 0;
	while (nxt != NULL) {
		if (!(nxt->card->hidden)) {
			total = total + cardToInt(nxt->card);
		}
		nxt = nxt->next;
	}
	// If total is over 21 check for aces
	if (total > 21) {
		nxt = f;
		while (nxt != NULL) {
			if (nxt->card->face == 'a') {
				total = total - 10;
			}
			if (total < 22) {
				break;
			}
			nxt = nxt->next;
		}
	}
	return total;
}

int cardToInt(Card *card) {
	// Convert a card to int for scoring

	char face = card->face;
	int v = 0;
	switch(face) {
		case '2' :
			v = 2;
			break;
		case '3' :
			v = 3;
			break;
		case '4' :
			v = 4;
			break;
		case '5' :
			v = 5;
			break;
		case '6' :
			v = 6;
			break;
		case '7' :
			v = 7;
			break;
		case '8' :
			v = 8;
			break;
		case '9' :
			v = 9;
			break;
		case 't' :
			v = 10;
			break;
		case 'j' :
			v = 10;
			break;
		case 'q' :
			v = 10;
			break;
		case 'k' :
			v = 10;
			break;
		case 'a' :
			v = 11;
			break;
	}
	return v;
}

void addCardNode(CardNode* fptr, Deck* deck) {
	// Adds a new Card to a hand
	// fptr: first cardNode in hand
	// deck: deck to draw from

	CardNode *hptr = fptr->head; // Fetches current head in list
	CardNode *nptr = malloc(sizeof(CardNode)); // Allocates space for new node
	nptr->card = drawCard(deck); //randomCard();
	nptr->next = NULL;
	hptr->next = nptr; // Sets next node on the old head to the new node
	fptr->head = nptr; // Update head on first node
	fptr->size++; // Update size on first node
}

CardNode* initNewHand(Deck* deck) {
	// Initalizes new hand and returns first Node

	CardNode* firstNode = malloc(sizeof(CardNode));
	firstNode->card = drawCard(deck); //randomCard();
	firstNode->next = NULL; // No next card yet
	firstNode->size = 1;
	firstNode->first = true; // Define this Node as the first.
	firstNode->head = firstNode; // Sets head of list to this node

	return firstNode;
}

void clearHand(CardNode* nxt) {
	// Clears a hand from memory

	CardNode* f; // Temporary pointer
	while (nxt != NULL) {
		// No need to clear the card.
		// Will be done later by clearDeck()

		// Set temporary var
		f = nxt;
		// Set nxt for next iteration
		nxt = nxt->next;

		// Clear current node
		free(f);
	}
}

Deck* newDeck(int numDecks) {
	int size = numDecks*52;
	// Create array of card pointers
	Card** cards = malloc( size * sizeof(Card*) );

	char faces[13] = {'2','3','4','5','6','7','8','9','t','j','q','k','a'};
	char suits[4] = {'d','c','h','s'};

	int i,j,k;
	int index = 0;
	char suit,face;
	// for each deck
	for (i=0; i<numDecks; i++)
	{
		// for each face 
		for (j=0; j<13; j++)
		{
			face = faces[j];
			// for each face
			for (k=0; k<4; k++)
			{
				suit = suits[k];

				// Create new card
				Card* card = malloc(sizeof(Card));
				card->face = face;
				card->suit = suit;
				card->hidden = false;
				card->style = ' ';

				cards[index] = card;
				index++;
			}
		}
	}
	
	Deck* deck = malloc(sizeof(Deck));
	deck->cards = cards;
	deck->size = size;
	deck->position = 0;

	return deck;
}

void shuffleDeck(Deck* deck)
{
	// Fisher–Yates shuffle algorithm
	Card** a = deck->cards;
	int n = deck->size;

	int i,j;
	Card* t;
	for (i=n-1; i>0; i--)
	{
		j = rand()%(i+1);
		// swap a[i] a[j]
		t = a[i];
		a[i] = a[j];
		a[j] = t;
	}

	// Reset position to 0
	deck->position = 0;
}

Card* drawCard(Deck* deck)
{
	// Pick card at current position
	Card* card = deck->cards[deck->position];

	// Update position unless on final card
	if (deck->position < deck->size-1)
	{
		deck->position++;
	}
	else
	{
		// Reshuffle if deck has been completed
		// Won't run unless shuffleAt is set too high
		shuffleDeck(deck);
	}

	return card;
}

void clearDeck(Deck* deck)
{
	Card** a = deck->cards;

	// Free all the cards
	int i;
	for (i=0;i<deck->size;i++)
	{
		free(a[i]);
	}

	// Free the array and deck itself
	free(a);
	free(deck);
}

#ifdef DEBUG
	#include <stdio.h>
	void printDeck(Deck* deck)
	{
		int i;
		for (i=0;i<deck->size;i++)
		{
			Card* card = deck->cards[i];
			printf("%c", card->face);
			printf("%c", card->suit);
			printf("%c", ',');
		}
		printf("%c", '\n');
	}
#endif