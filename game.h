#ifndef GAME_H
#define GAME_H

#include "cards.h"
#include <stdbool.h>

typedef struct Game {
	int chips;
	int bet;
	// shuffle deck after a given percentage of cards have been delt.
	float shuffleAt;

	Deck* deck; // Pointer to card deck

	CardNode* phand; // Pointer to player hand
	CardNode* dhand; // Pointer to dealer hand

	bool inGame; // should the window be open
	bool running; // is there a game actively running
} Game;

void runLogic(int numDecks,float shuffleAt); // Start the main game loop

bool shouldShuffle(Game* game); // Decide whether to shuffle deck

void sleepFor(int ms); // wait ms milliseconds

// makes a player decision based on given game conditions
char aiMakeDecision(CardNode * dhand, CardNode * phand);

#endif