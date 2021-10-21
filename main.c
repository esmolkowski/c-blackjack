#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "cards.h"

void main(int argc, char *argv[]) {
	// Seed rand()
	srand(time(0));

	// Start game loop with 1 deck and shuffle at 75%
	runLogic(1,.75);
}