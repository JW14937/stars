#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "uni.h"


// Print the details of a star
void printStar (Star* s)
{
	printf("star %i\n", s->id);
	if(s->named == 1) {
            printf("name: %s\n", s->name);
        }
	printf("coords: (%.3f,%.3f)\n", s->x, s->y);

}

// Display a list of all stars in the universe
void listStars ()
{
	for(int i=0; i<numStars; i++) {
        printStar(stars[i]);
	}
}

// List all named pairs
void listPairs ()
{
	for(int i=0; i<numPairs; i++) {
        printf("Pair %i:\n", i);
        printf("distance: %f\n", pairs[i]->d);
        printf("star %i\n", (pairs[i]->h)->id);
        printf("name: %s\n", (pairs[i]->h)->name);
        printf("coords: (%.3f,%.3f)\n", (pairs[i]->h)->x, (pairs[i]->h)->y);

        printf("star %i\n", (pairs[i]->w)->id);
        printf("name: %s\n", (pairs[i]->w)->name);
        printf("coords: (%.3f,%.3f)\n", (pairs[i]->w)->x, (pairs[i]->w)->y);

        printf("**************************\n");
	}
}

// Draw the entire universe
void drawUniverse ()
{
    clearBuffer();

    for(int i=0; i<numStars; i++) {

        char character;

        if(stars[i]->named == 1) character='*';
        else if(stars[i]->named == 0) character='.';

        plot(stars[i]->y, stars[i]->x, character);
    }
}

// Tag a named pair of stars by writing a string comprising the names of the couple that own the pair
// underneath this pair in the frame buffer (see assignment brief)
// The names of the 2 owners should be concatenated into one string with an & sign in between.
// The name of the h star should be first followed by the name of the w star (by the way, h stands for husband and w stands for wife).
// For example, if the the h star name is "Mr Bean" and the w star name is "Irma Gobb", the string will be "Mr Bean & Irma Gobb"
// with exactly one space before and after the &.
// The string should be positioned one row below the h star's row, with the middle of the string aligned with the h star's column
// Calling this function will NOT cause the frame buffer to be redisplayed on the screen, if you need to show the frame buffer, you must
// call showBuffer (but NOT inside this function)

void tagPair (Pair* p)
{

    char hName[70];
    strcpy(hName, (p->h)->name);
    char wName[30];
    strcpy(wName, (p->w)->name);

    strcat(hName, " & ");
    strcat(hName, wName);

    int hRow = (p->h)->y;
    int hCol = (p->h)->x;

    int halfName = strlen(hName)/2;
    writeAt(hRow+1, hCol-halfName, hName);
}

#ifndef TEST
int  main (int argc, char* argv[])
{
	char userCommand[500];

	while(strcmp(userCommand, "quit") != 0) {

        printf(">>>");
		gets(userCommand);

		// Bang command
		if(strstr(userCommand, "bang") == &(userCommand[0])) {

            clearBuffer();

			//Find n_stars
			char* addressOfSpace = strchr(userCommand, ' ');
			char n_starsChar[4] = {'A', 'A', 'A', 'A'};
			int n = 0;

			char* currentlyReadChar = addressOfSpace+1;

			while(*currentlyReadChar != '\0' && n < 4) {
				n_starsChar[n] = *currentlyReadChar;
				n++;
				currentlyReadChar++;
			}

			int n_stars = 0;

			for(int i=0; i<4; i++) {
                if(n_starsChar[i] != 'A') {
                    n_stars = 10*n_stars + (n_starsChar[i] - '0');
                }
			}

			drawUniverse();

            if(bigBang(n_stars) == 1) {
                printf("The Big Bang has occurred!\n");
            }

            else {
                printf("Sorry, unable to bang!\n");
            }
		}

		// List command
		else if(strcmp(userCommand, "list") == 0) {
            listStars();
		}

		// Name command
		else if(strcmp(userCommand, "name") == 0) {

            if(closestPair() == 0) {
                printf("Sorry no pairs were found!\nWish you a better luck in the next universe\n");
            }

            // closestPair() will return a pointer to a Pair structure initialised with h pointing to the first star in
            // the pair and w pointing to the second star.


            else {

                Pair* pair = closestPair();
                printf("The closest pair of stars are no. %i and %i\n", (pair->h)->id, (pair->w)->id);
                printf("They are %.3f light years apart\n", pair->d);
                printf("Would you like to name this pair (y/n)?");
                char uInput = getchar();
                getchar();

                if(uInput == 'y') {
                    printf("Enter your full name:");
                    char theirFName[30];
                    char theirSName[30];
                    scanf("%s %s", theirFName, theirSName);

                    while(nameStar(pair->h, theirFName, theirSName) == 0){
                        scanf("%s %s", theirFName, theirSName);
                    }

                    printf("Enter your spouse full name:");
                    char spouseFName[30];
                    char spouseSName[30];
                    scanf("%s %s", spouseFName, spouseSName);

                    while(nameStar(pair->w, spouseFName, spouseSName) == 0){
                        scanf("%s %s", theirFName, theirSName);

                    }

                    // Initialise a pair structure
                    pairs[numPairs] = (Pair *)malloc(sizeof(Pair));
                    pairs[numPairs]->h = pair->h;
                    pairs[numPairs]->w = pair->w;
                    pairs[numPairs]->d = pair->d;


                    plot((pair->h)->y, (pair->h)->x, '*');
                    plot((pair->w)->y, (pair->w)->x, '*');
                    numPairs++;
                    
                    getchar();
                    printf("Congratulations a pair of stars has been named after you and your spouse\n");

                }
                else if (uInput == 'n') {
                    // No need to do anything
                }

            }
		}

		// Pairs command
		else if(strcmp(userCommand, "pairs") == 0) {
            listPairs();
		}

		// Draw command
		else if(strcmp(userCommand, "draw") == 0) {
            drawUniverse();
            showBuffer();
		}

		// Show command
		else if(strcmp(userCommand, "show") == 0) {

            char name[100];
            printf("Enter your full name:");
            gets(name);

            if(findPairByName(name)==0) {
                // No need to do anything
            }
            else {
                Pair* p = findPairByName(name);
                tagPair(p);
                showBuffer();
                drawUniverse();
            }
		}

		// Save command
		else if(strcmp(userCommand, "save") == 0) {
            if(saveUniverse()==1)
            printf("Thanks, you have saved the universe!\n");
		}

		// Load command
		else if(strcmp(userCommand, "load") == 0) {
            if(loadUniverse() ==1)
            printf("Congratulations, your saved universe was restored!\n");
		}

		else if(strcmp(userCommand, "quit") != 0){
            printf("No such command!\n");
		}

	}
}
#endif