#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cards.h"

void renderHand(struct CardNode * fptr) {
	// Renders a hand in text graphics
	// Input is the first node in the Hand
	
	//	/------\ 48 chars
	//	|K    h| 54 with spaces
	//	|      | ((9*6)*cards)+6 newlines
	//	|      | 
	//	|h    K| 
	//	\------/ 
	
	int size = fptr->size;
	int space = (size*9*6) + 6;
	char s[space];
	
	struct CardNode * nxt;
	nxt = fptr;
	
	int i = 0;
	//line 1
	for (int z = 0; z < size; ++z)
	{
		s[i]='/'; i++; s[i]='-'; i++; s[i]='-'; i++; s[i]='-'; i++;
		s[i]='-'; i++; s[i]='-'; i++; s[i]='-'; i++; s[i]='\\';i++;
		s[i]=' '; i++;
	}
	s[i]='\n'; i++;
	//line2
	while (nxt != NULL)
	{
		s[i] = '|'; i++;
		if (nxt->card->hidden) {
			s[i] = nxt->card->style;
		} else {
			s[i] = nxt->card->face;
		}
		i++;
		s[i] = nxt->card->style; i++;
		s[i] = nxt->card->style; i++;
		s[i] = nxt->card->style; i++;
		s[i] = nxt->card->style; i++;
		if (nxt->card->hidden) {
			s[i] = nxt->card->style;
		} else {
			s[i] = nxt->card->suit;
		}
		i++;
		s[i] = '|'; i++;
		s[i] = ' '; i++;
		nxt = nxt->next;
	}
	nxt = fptr;
	s[i]='\n'; i++;
	//line 3-4
	for (int a = 0; a < 2; ++a)
	{
		while (nxt != NULL)
		{
			s[i]='|'; i++; s[i]=nxt->card->style; i++; s[i]=nxt->card->style; i++; s[i]=nxt->card->style; i++;
			s[i]=nxt->card->style; i++; s[i]=nxt->card->style; i++; s[i]=nxt->card->style; i++; s[i]='|';i++;
			s[i]=' '; i++;
			nxt = nxt->next;
		}
	s[i]='\n'; i++;
	nxt = fptr;
	}
	//line 5
	while (nxt != NULL)
	{
		s[i] = '|'; i++;
		if (nxt->card->hidden) {
			s[i] = nxt->card->style;
		} else {
			s[i] = nxt->card->suit;
		}
		i++;
		s[i] = nxt->card->style; i++;
		s[i] = nxt->card->style; i++;
		s[i] = nxt->card->style; i++;
		s[i] = nxt->card->style; i++;
		if (nxt->card->hidden) {
			s[i] = nxt->card->style;
		} else {
			s[i] = nxt->card->face;
		}
		i++;
		s[i] = '|'; i++;
		s[i] = ' '; i++;
		nxt = nxt->next;
	}
	s[i]='\n'; i++;
	//line 6
	for (int z = 0; z < size; ++z)
	{
		s[i]='\\'; i++; s[i]='-'; i++; s[i]='-'; i++; s[i]='-'; i++;
		s[i]='-'; i++; s[i]='-'; i++; s[i]='-'; i++; s[i]='/';i++;
		s[i]=' '; i++;
	}
	s[i]='\n';
	for (int i = 0; i < space; ++i)
	{
		printf("%c",s[i]);
	}
}

void renderScene(struct CardNode * dhand, struct CardNode * phand, int chips, int bet) {
	printf("--------------------------------------------------------------------------");
	printf("\nDEALER HAND:\n");
	renderHand(dhand);
	printf("TOTAL: %d\n", getHandTotal(dhand));
	printf("\nPLAYER HAND:\n");
	renderHand(phand);
	printf("TOTAL: %2d                                      CHIPS: %7d BET: %7d\n", getHandTotal(phand),chips,bet);
	printf("--------------------------------------------------------------------------\n");
}