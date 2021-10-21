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
		// Show user current chips and take their bet
		printf("CHIPS: %d\n", game.chips);
		printf("BET: ");
		scanf(" %d", &game.bet);
		printf("\n");
		if (game.bet < 0 || game.chips - game.bet < 0)
			continue;
		game.chips = game.chips - game.bet;

		// Initialize the hands
		game.phand = initNewHand(game.deck);
		game.dhand = initNewHand(game.deck);

		// Make first dealer card hidden
		game.dhand->card->hidden = true;
		game.dhand->card->style = 'x';
		
		// Add second cards
		addCardNode(game.phand,game.deck);
		addCardNode(game.dhand,game.deck);

		// Display scene to user
		renderScene(game.dhand,game.phand,game.chips,game.bet);

		// Take user input
		char choice = ' ';
		printf("STAND (S), HIT(H), DOUBLE DOWN (D): ");
		//choice = aiMakeDecision(game.dhand,game.phand);
		scanf(" %c", &choice);
		while ( choice != 'S' && (choice == 'H' || choice == 'D') )
		{
			// While hit or double down
			bool redo = false;
			switch (choice)
			{
				case 'H' :
					addCardNode(game.phand,game.deck);
					renderScene(game.dhand,game.phand,game.chips,game.bet);
					break;
				case 'D' :
					if (game.chips - game.bet < 0) {
						// If user doesn't have enough to double down, take back to prompt
						redo = true;
						break;
					}
					game.chips = game.chips - game.bet;
					game.bet = game.bet*2;
					addCardNode(game.phand,game.deck);
					renderScene(game.dhand,game.phand,game.chips,game.bet);
					break;
			}

			// If player has score of 21+ it is dealer's turn
			if (getHandTotal(game.phand) > 20)
				break; //BLACKJACK OR BUST

			printf("STAND (S), HIT(H), DOUBLE DOWN (D): ");
			//choice = aiMakeDecision(game.dhand,game.phand);
			scanf(" %c", &choice);
		}
		printf("\n\n");

		// Player input is over. Either player busted or is standing
		// Unhide dealer's hidden card
		game.dhand->card->hidden = false;
		game.dhand->card->style = ' ';

		// Check if bust or standing
		if (getHandTotal(game.phand) > 21)
		{
			sleepFor(2000);
			printf("                        PLAYER BUST: DEALER WINS");
		} else {
			// Stand
			bool skipped = true;
			// Show hidden card
			renderScene(game.dhand,game.phand,game.chips,game.bet);
			sleepFor(2000);
			// Dealer draws until game either above player, or dealer busted
			while (getHandTotal(game.dhand) < getHandTotal(game.phand)) {
				skipped = false;
				addCardNode(game.dhand,game.deck);
				printf("\n\n");
				renderScene(game.dhand,game.phand,game.chips,game.bet);
				sleepFor(2000);
			}

			// Get totals
			int dtotal = getHandTotal(game.dhand);
			int ptotal = getHandTotal(game.phand);

			// Display winner
			printf("\n\n                        ");
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

			// Clear the deck from memory
			clearDeck(game.deck);
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

	int dt = getHandTotal(dhand);
	int pt = getHandTotal(phand);
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