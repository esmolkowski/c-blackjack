#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

void runLogic() {
    Game game;
    game.chips = 1000;
    game.bet = 0;
    game.ingame = true;
    game.running = true;

    setbuf(stdout, NULL); // fixes scanf issue
	srand(time(0)); //use current time as seed for random generator

	while (game.running)
	{
		printf("CHIPS: %d\n", game.chips);
		printf("BET: ");
		scanf(" %d", &game.bet);
		//game.bet = 10;
		printf("\n");
		if (game.bet < 0 || game.chips - game.bet < 0)
			continue;
		game.chips = game.chips - game.bet;
		//CardNode *phand = initNewList();
		//CardNode *dhand = initNewList();
        game.phand = initNewList();
        game.dhand = initNewList();
		//make first dealer card hidden
		game.dhand->cptr->hidden = 1;
		game.dhand->cptr->style = 'x';
		addNode(game.phand);
		addNode(game.dhand);
		renderScene(game.dhand,game.phand,game.chips,game.bet);

		char choice = ' ';
		printf("STAND (S), HIT(H), DOUBLE DOWN (D): ");
		//choice = aiMakeDecision(game.dhand,game.phand);
		scanf(" %c", &choice);
		while ( choice != 'S' && (choice == 'H' || choice == 'D') )
		{
			_Bool redo = 0;
			switch (choice)
			{
				case 'H' :
					addNode(game.phand);
					renderScene(game.dhand,game.phand,game.chips,game.bet);
					break;
				case 'D' :
					if (game.chips - game.bet < 0) {
						redo = 1;
						break;
					}
					game.chips = game.chips - game.bet;
					game.bet = game.bet*2;
					addNode(game.phand);
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
			_Bool skipped = 1;
			renderScene(game.dhand,game.phand,game.chips,game.bet); // SHOW HIDDEN CARD
			sleepFor(2000);
			while (getTotal(game.dhand) < getTotal(game.phand)) {
				skipped = 0;
				addNode(game.dhand);
				printf("\n\n");
				renderScene(game.dhand,game.phand,game.chips,game.bet);
				sleepFor(2000);
			} // add code if dealer is tied at a low number
			//if (skipped == 1)
				//renderScene(game.dhand,game.phand,game.chips,game.bet);
			//sleep(2000);
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

        // Clear hands
        clearHand(game.phand);
        clearHand(game.dhand);

		if (game.chips > 0) {
			printf("\n\n\n--------------------------------NEW ROUND---------------------------------\n\n\n");
		} else {
			printf("\n\n\n------------------------OUT OF CHIPS | GAME OVER--------------------------\n\n\n");
			printf("EXITING IN 5 SECONDS");
			sleepFor(5000);
			break;
		}
	}
}

char aiMakeDecision(CardNode * dhand, CardNode * phand) {
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