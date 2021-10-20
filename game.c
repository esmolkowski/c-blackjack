#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "cards.h"
#include "renderer.h"
#include "game.h"

#ifdef _WIN32
	#include <windows.h>
	void sleepFor(int ms)
	{
		Sleep(ms);
	}
#else
	#include <unistd.h>

	void sleepFor(int ms)
	{
		usleep(ms*1000);
	}
#endif

void runLogic(int numDecks,float shuffleAt) {
    Game game;
    game.chips = 1000;
    game.bet = 0;
    game.inGame = true;
    game.running = true;
	game.shuffleAt = shuffleAt;

    game.deck = newDeck(numDecks); // create numDecks of 52 card decks
    shuffleDeck(game.deck); // shuffle the deck

    setbuf(stdout, NULL); // fixes scanf issue
	srand(time(0)); //use current time as seed for random generator

	while (game.running)
	{
		printf("CHIPS: %d\n", game.chips);
		printf("BET: ");
		scanf(" %d", &game.bet);
		printf("\n");
		if (game.bet < 0 || game.chips - game.bet < 0)
			continue;
		game.chips = game.chips - game.bet;

        game.phand = initNewList(game.deck);
        game.dhand = initNewList(game.deck);

		// make first dealer card hidden
		game.dhand->cptr->hidden = 1;
		game.dhand->cptr->style = 'x';
		
        // add second cards
        addNode(game.phand,game.deck);
		addNode(game.dhand,game.deck);

        // display to user
		renderScene(game.dhand,game.phand,game.chips,game.bet);

		char choice = ' ';
		printf("STAND (S), HIT(H), DOUBLE DOWN (D): ");
		//choice = aiMakeDecision(game.dhand,game.phand);
		scanf(" %c", &choice);
		while ( choice != 'S' && (choice == 'H' || choice == 'D') )
		{
			bool redo = false;
			switch (choice)
			{
				case 'H' :
					addNode(game.phand,game.deck);
					renderScene(game.dhand,game.phand,game.chips,game.bet);
					break;
				case 'D' :
					if (game.chips - game.bet < 0) {
						redo = true;
						break;
					}
					game.chips = game.chips - game.bet;
					game.bet = game.bet*2;
					addNode(game.phand,game.deck);
					renderScene(game.dhand,game.phand,game.chips,game.bet);
					break;
			}


			if (getTotal(game.phand) > 20)
				break; //BLACKJACK OR BUST

			printf("STAND (S), HIT(H), DOUBLE DOWN (D): ");
			//choice = aiMakeDecision(game.dhand,game.phand);
			scanf(" %c", &choice);
		}
		printf("\n\n");
		// Check if bust
		if (getTotal(game.phand) > 21)
		{
			sleepFor(2000);
			printf("                        PLAYER BUST: DEALER WINS");
		} else {

			// Stand
			game.dhand->cptr->hidden = 0;
			game.dhand->cptr->style = ' ';
			bool skipped = true;
			renderScene(game.dhand,game.phand,game.chips,game.bet); // Show hidden card
			sleepFor(2000);
			while (getTotal(game.dhand) < getTotal(game.phand)) {
				skipped = false;
				addNode(game.dhand,game.deck);
				printf("\n\n");
				renderScene(game.dhand,game.phand,game.chips,game.bet);
				sleepFor(2000);
			}

			printf("\n\n                        ");

			int dtotal = getTotal(game.dhand);
			int ptotal = getTotal(game.phand);

			if (ptotal > dtotal) {
				game.chips = game.chips + game.bet*2;
				printf("         YOU WIN        ");
			} else if (ptotal == dtotal) {
				game.chips = game.chips + game.bet;
				printf("           TIE          ");
			} else if (dtotal > 21) {
				printf("DEALER BUST: PLAYER WINS");
				game.chips = game.chips + game.bet*2;
			} else {
				printf("       DEALER WINS      ");
			}
		}

        // Clear hands from memory
        clearHand(game.phand);
        clearHand(game.dhand);

		if (game.chips > 0) {
			printf("\n\n\n--------------------------------NEW ROUND---------------------------------");
			// Shuffle deck if needed
			if (shouldShuffle(&game))
			{
				shuffleDeck(game.deck);
				printf("\n                              DECK SHUFFLED                               ");
			}
			printf("\n\n\n");
		} else {
			printf("\n\n\n------------------------OUT OF CHIPS | GAME OVER--------------------------\n\n\n");
			printf("EXITING IN 5 SECONDS");
			sleepFor(5000);
			break;
		}
	}
}

bool shouldShuffle(Game* game)
{
	int pos = game->deck->position;
	int maxpos = game->deck->size-1;
	float used = (float)pos/maxpos;

	return (used >= game->shuffleAt);
}

char aiMakeDecision(CardNode * dhand, CardNode * phand) {
    // Currently unused, but can make basic decisions for player
    // based on the current in play hands.

	int dt = getTotal(dhand);
	int pt = getTotal(phand);
	char choice = ' ';
	if (dt <= 6 && pt > 13) {
		choice = 'S';
	} else if (dt > 6 && pt < 17) {
		choice = 'H';
	} else if (pt < 12 && pt > 8) {
		choice = 'H';
	} else if (pt < 16) {
		choice = 'H';
	} else {
		choice = 'S';
	}
	switch (choice) {
		case 'H':
		printf("\nAI HIT\n");
		break;

		case 'S':
		printf("\nAI STAND\n");
		break;
	}
	sleepFor(2000);
	return choice;
}