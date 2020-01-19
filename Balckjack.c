/*
===============================================================================
 Name        : Exercise.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#define CLUB 1
#define DIAMOND 2
#define HEART 3
#define SPADE 4
#define ACE 1
#define JACK 11
#define QUEEN 12
#define KING 13

#define MAX_CARDS 52
#define BJ_LIMIT 21
#define LINESIZE 80

typedef struct{ int suit,
	rank;
} card;
typedef struct Deck{ card *c;
struct Deck *next;
} deck;
typedef struct { deck *hand;
int min, max;
} player;
// TODO: insert other definitions and declarations here
deck* add_card(deck *d, card *c);
deck* new_deck(void);
deck *mix(deck *d);
deck* cut(deck* d);
void free_deck(deck *d);


void print_card(card *c);
void print_deck(deck *d);
void update_player(player *p);
deck* hit(player *p, deck *d);

deck* add_card(deck *d, card *c)
{
	deck *d_new;
	d_new = (deck *)malloc(sizeof(deck));

	d_new->c = c;
	d_new->next=d;

	return d_new;
}

deck* new_deck(void)
{
	int i=0,j=0;
	deck *d=NULL;
	card *c;

	for(j=CLUB;j<=SPADE;j++)
	{
		for(i=1;i<14;i++)
		{
			c = (card *)malloc(sizeof(card));

			c->suit = j;
			c->rank = i;
			d=add_card(d,c);
		}
	}

	return d;
}

deck *mix(deck *d)
{
	deck *d1,*d2,*tmp;
	int i;


	d1=d;

	for(i=0;i<25;i++)
	{
		d = d->next;

	}

	d2 = d->next;
	d->next = NULL;



	d=NULL;
	while(d1!=NULL)
	{
		tmp = d1->next;
		d1->next = d;
		d = d1;
		d1 = tmp;

		if(d2==NULL) break;

		tmp = d2->next;
		d2->next = d;
		d = d2;
		d2 = tmp;
	}


	return d;
}

deck* cut(deck* d)
{
	int r = rand() % MAX_CARDS;
	int i = 0;
	deck *d1,*d2;

	d1=d;
	for(i=1;i<r;i++)
		d=d->next;

	d2 = d->next;
	d->next = NULL;

	d = d2;
	if(d2==NULL) d = d1;
	else
	{
		while(d2->next !=NULL)
			d2 = d2->next;
		d2->next = d1;
	}

	return d;
}

deck *shuffle(deck *d)
{
	int i=0;

	for(i=0;i<10;i++)
	{
		d = cut(d);
		d = mix(d);
	}

	return d;
}

void free_deck(deck *d)
{
	deck *tmp;
	while(d!=NULL)
	{
		free(d->c);
		tmp = d->next;
		free(d);
		d = tmp;
	}
}


void print_card(card *c)
{



	if(c->rank>ACE && c->rank<JACK){
		printf("%2d",c->rank);
	}
	else
	{
		switch(c->rank)
		{
		case ACE: printf(" A"); break;
		case JACK: printf(" J"); break;
		case QUEEN: printf(" Q"); break;
		case KING: printf(" K"); break;


		}
	}
	switch(c->suit)
	{

	case CLUB: printf("c");break;
	case DIAMOND: printf("d"); break;
	case HEART: printf("h"); break;
	case SPADE: printf("s"); break;


	}

}

void print_deck(deck *d)
{


	/*Reverse printing */
	while(d!=NULL)
	{
		print_card(d->c);
		printf(" ");
		d = d->next;
	}

}

void update_player(player *p)
{
	int aces=0, i;
	int total=0;
	deck *d;



	if(p->hand==NULL)
	{
		p->min = 0;
		p->max = 0;
		return;
	}

	d = p->hand;
	while(d!=NULL)
	{
		switch(d->c->rank)
		{
		case ACE: aces++; break;
		case KING:
		case QUEEN:
		case JACK: total+=10; break;
		default: total+=d->c->rank;
		}

		d=d->next;
	}

	p->min = aces+total;
	p->max = p->min;

	i=aces;
	while(p->max <= BJ_LIMIT && i>0)
	{
		p->max +=10;
		i--;
	}

	if(p->max > BJ_LIMIT && i!=aces) p->max-=10;

}

deck* hit(player *p, deck *d)
{
	deck *tmp;



	tmp = d->next;
	d->next = p->hand;
	p->hand = d;

	update_player(p);

	return tmp;
}
int main(void) {

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	// TODO: insert code here
	FILE *fp;
	deck *d;
	player p, dealer;
	char input[LINESIZE];
	int lc = 0;
	char line[LINESIZE];

	printf("This is blackjack.\nHere are the rules\nGet as close to 21 as possible, without going over 21.\nAce can act as 1 or 11\n");
	fp = fopen("blackjack.txt", "w");
	fprintf( fp,"Rules\n");
	fclose(fp);
	srand(521);

	do
	{


		p.hand=NULL;
		p.min=0;
		p.max=0;
		dealer.hand=NULL;
		dealer.min=0;
		dealer.max=0;

		printf("\nShuffling the deck\n\n");
		d=new_deck();
		d=shuffle(d);
		fp = fopen("blackjack.txt", "a");
		fprintf( fp,"Shuffle\n");
		fclose(fp);

		d=hit(&p,d);
		d=hit(&dealer,d);
		d=hit(&p,d);
		d=hit(&dealer,d);
		fp = fopen("blackjack.txt", "a");
		fprintf( fp,"Cards dealt\n");
		fclose(fp);

		printf("Dealer: ");
		print_deck(dealer.hand);
		printf("\nYou: ");
		print_deck(p.hand);

		input[0]='\0';
		while(input[0]!='s')
		{

			printf("\nH for hit or S for stand\n");
			scanf("%s",input);

			input[0]=tolower(input[0]);

			if(input[0]!='h'&& input[0]!='s')
			{
				printf("No good,try again\n");
			}

			if(input[0]=='h')
			{
				d = hit(&p,d);
				printf("You: ");
				print_deck(p.hand);
				printf("\n");
				fp = fopen("blackjack.txt", "a");
				fprintf( fp,"Hit\n");
				fclose(fp);
				if(p.min>BJ_LIMIT) break;
			}
			if(input[0]=='s'){
				fp = fopen("blackjack.txt", "a");
				fprintf( fp,"Stand\n");
				fclose(fp);
			}
		}

		if(p.min>BJ_LIMIT) {
			printf("\nI win!!!\n");
			fp = fopen("blackjack.txt", "a");
			fprintf( fp,"Win\n");
			fclose(fp);
		}
		else
		{

			while(dealer.max<17)
			{
				d = hit(&dealer,d);
				printf("\nDealer: ");
				print_deck(dealer.hand);
				if(dealer.min>BJ_LIMIT)
					break;
			}

			if(dealer.min>BJ_LIMIT){
				printf("\nI lose.\n");
				fp = fopen("blackjack.txt", "a");
				fprintf( fp,"Loss\n");
				fclose(fp);
			}
			else
			{
				if(dealer.max>p.max){
					printf("\nI win.\n");
					fp = fopen("blackjack.txt", "a");
					fprintf( fp,"Win\n");
					fclose(fp);
				}
				else{
					printf("\nI lose.\n");
					fp = fopen("blackjack.txt", "a");
					fprintf( fp,"Loss\n");
					fclose(fp);
				}
			}
		}
		printf("Dealer: ");
		print_deck(dealer.hand);
		free_deck(p.hand);
		free_deck(dealer.hand);
		free_deck(d);

		printf("\nPlay again?Enter a string with a letter (n) first to end.The game will continue with everything else\n");
		scanf("%s",input);

		input[0]=tolower(input[0]);
		if(input[0]=='n'){
			fp = fopen("blackjack.txt", "a");
			fprintf( fp,"GAME END\n");
			fclose(fp);
		}
		else{
			fp = fopen("blackjack.txt", "a");
			fprintf( fp,"NEW GAME\n");
			fclose(fp);
		}

	} while(input[0]!='n');

	fp = fopen("blackjack.txt", "r");
	if(fp == NULL) {
		printf("Unable to open the file");
	}
	else {
		printf("GAMELOG\n");
		while(!feof(fp)) {
			if(fgets(line, LINESIZE, fp) != NULL) {
				lc++;
				printf("%2d: %s", lc, line);
			}
		}
		fclose(fp);
	}
	// Force the counter to be placed into memory
	volatile static int z = 0 ;
	// Enter an infinite loop, just incrementing a counter
	while(1) {
		z++ ;
	}
	return 0 ;
}
