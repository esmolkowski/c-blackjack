#ifndef GAME_H
#define GAME_H

#include "cards.h"
#include <stdbool.h>

typedef struct Game {
	int chips;
    int bet;

    CardNode *phand; // Pointer to player hand
    CardNode *dhand; // Pointer to dealer hand
    
    bool ingame; // should the window be open
    bool running; // is there a game actively running
} Game;

void runLogic(); // the main game loop

void sleepFor(int ms); // wait ms milliseconds

char aiMakeDecision(CardNode * dhand, CardNode * phand);
// makes a player decision based on given game conditions

#endif