#include <stdlib.h>
#include "cards.h"

int getTotal(CardNode * nxt) {
	CardNode * f;
	f = nxt;
	//loop through all cards in hand
	//cnode *nxt;
	//nxt = first.next;
	int total = 0;
	while (nxt != NULL) {
		if (nxt->cptr->hidden == 0) {
			total = total + cardToInt(nxt->cptr);
		}
		nxt = nxt->next;
	}
	//if total is over 21 check for aces
	if (total > 21) {
		nxt = f;
		while (nxt != NULL) {
			if (nxt->cptr->face == 'a') {
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

Card * randomCard() {
    // Fetches new random card
	char faces[13] = {'2','3','4','5','6','7','8','9','t','j','q','k','a'};
	char suits[4] = {'d','c','h','s'};
	Card *cptr = malloc(sizeof(Card));
	cptr->face = faces[rand()%13];
	cptr->suit = suits[rand()%4];
	cptr->hidden = 0;
	cptr->style = ' ';
	return cptr;
}

void addNode(CardNode * fptr) {
    // Adds a new Card to a hand

	CardNode *hptr = fptr->head; // Fetches curren head in list
	CardNode *nptr = malloc(sizeof(CardNode)); // Allocates space for new node
	nptr->cptr = randomCard();
	nptr->next = NULL;
	hptr->next = nptr; // Sets next node on the old head to the new node
	fptr->head = nptr; // Update head on first node
	fptr->size++; // Update size on first node
}

CardNode * initNewList() {
	// Initalizes new hand and returns first Node

	CardNode *first = malloc(sizeof(CardNode));
	first->cptr = randomCard();
	first->next = NULL; // No next card yet
	first->size = 1;
	first->first = 1; // Define this Node as the first.
	first->head = first; // Sets head of list to this node
	return first;
	//cnode *hptr;
	//hptr = &first;
}

void clearHand(CardNode * nxt) {
	CardNode * f; // Temporary pointer
	while (nxt != NULL) {
		// Clear the card
		free(nxt->cptr);

		// Set temporary var
		f = nxt;
		// Set nxt for next iteration
		nxt = nxt->next;

		// Clear current node
		free(f);
	}
}