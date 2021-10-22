#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "uni.h"


int SerNum=0;			// a counter to create unique star numbers (identifiers)
int numStars = 0;		// the actual number of stars in the universe (not the same as the maximum)
int numPairs = 0;		// the number of pairs that have been named

Star *stars[MaxStar];		// the array of stars (an array of pointers to stars)
Pair *pairs[MaxPair];		// the array of named pairs (an array of pointers to pairs)


// The purpose of this function is to find a star through its coordinates.
// The first two parameters are the coordinates of the star we are looking for.
// Since the coordinates are double (real) numbers, we cannot compare them for exact equality,
// hence the 3rd parameter to this function is a very small number (e.g. 0.00000001) called the tolerance.
// If the absolute difference between the x (or y) coordinate of a star and the given x (or y) coordinate
// is less than the tolerance, we assume that the two values are equal.
// If a star with the given x and y coordinates is found, the function should return a pointer to this star
// otherwise the function returns 0 indicating failure to find a star with such coordinates.
// 4-6 lines of code.
Star* findStarByXY  (double x , double y, double tolerance)
{
    for(int i=0; i<numStars; i++) {

        if(fabs(stars[i]->x - x)>=tolerance) {
            continue;
        }
        if(fabs(stars[i]->y - y)>=tolerance) {
            continue;
        }
        return stars[i];
    }


	return 0;
}

// The purpose of this function is to create a new universe containing stars only.
// The number of stars in this new universe is given by the parameter n_stars.
// The stars should be randomly scattered within the universe, i.e, each star should be assigned a random value
// for its x and y coordinates. x can take any random value from [0 - MaxX] inclusive,
// and y can take any random value from [0 - MaxY] inclusive.
// Although the x and y coordinates of a star are defined as double numbers, for simplicity we will only
// assign whole numbers to x and y. Every star should have its unique location, no two stars can have the same x and y coordinates.
// Hence the maximum number of stars in a completely filled universe that is 60 lights years long and 30 light years wide
// is 1891 stars (this is the value of MaxStar defined in uni.h). The minimum number of stars in a universe is 1.
// If the function fails to create the universe for any reason (including an invalid value of n_stars), it should return 0
// indicating failure, otherwise it should return 1 indicating success.
// Since this function is the start of a new universe, it should also initialise all global variables.
int bigBang (int n_stars)
{
    // Initialise global variables
    numPairs = 0;
    //SerNum = 0;
    numStars = 0;

	if(n_stars < 1 || n_stars > MaxStar) {
		return 0;
	}

	for(int i=0; i<n_stars; i++) {

        free(stars[i]);
        stars[i] = (Star *)malloc(sizeof(Star));
        stars[i]->id = i;
        stars[i]->named = 0;

        double x_rand = rand() % (MaxX+1);
        double y_rand = rand() % (MaxY+1);


        while(findStarByXY(x_rand, y_rand, 0.00001)!=0) {

            x_rand = rand() % (MaxX+1);
            y_rand = rand() % (MaxY+1);

        }

        stars[i]->x = x_rand;
        stars[i]->y = y_rand;

        numStars++;
        SerNum++;

	}

    return 1;
}


// The purpose of this function is to compute the distance between two points in a Cartesian plane.
// The coordinates of the first point are x1 , y1.
// The coordinates of the second point are x2, y2.
// The function returns the distance between the two points.
// 2 lines of code.
double pointDistance (double x1, double y1, double x2, double y2)
{
	double distance = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
	return distance;
}


// The purpose of this function is to find the closest pair of stars that have not been named yet.
// If a pair cannot be found (e.g. because all pairs have already been named) the function should return 0 (null),
// otherwise the function should return a pointer to a Pair structure initialised with h pointing to the first star in the pair
// and w pointing to the second star.
Pair* closestPair ()
{
    int found = 0;
    double shortestDistance = pointDistance(0, 0, 100, 100);
    Star* closest_star_h;
    Star* closest_star_w;

    for(int i=0; i<numStars; i++) {

        if(stars[i]->named == 1) continue;

        for(int j=0; j<numStars; j++) {

            if(stars[j]->named == 1) continue;

            // Make sure we're not checking the same star
            if(j==i) continue;

            // At this point we have 2 unnamed stars
            found = 1;
            double distance = pointDistance(stars[i]->x, stars[i]->y, stars[j]->x, stars[j]->y);
            if(distance <= shortestDistance) {
                shortestDistance = distance;
                closest_star_h = stars[i];
                closest_star_w = stars[j];
            }

        }
    }

    if(found == 0) return 0;

    if(found == 1) {
        Pair* p = (Pair *)malloc(sizeof(Pair));
        p->h = closest_star_h;
        p->w = closest_star_w;
        p->d = shortestDistance;
        return p;
    }
}

// The purpose of this function is to assign the name of a person to a star.
// The first parameter is a pointer to the star to be named.
// The second parameter is the first name of the person.
// The last parameter is the surname of the person.
// The function should append the surname to the first name - with one single space in between - to create one single string for the full name.
// For example, if the first name is "Mr" and the surname is "Bean", the full name string will be "Mr Bean".
// If everything goes well and the star is named, the function returns 1 indicating success.
// The function also sets the named flag of the star to 1.
// If anything goes wrong (including invalid parameter values), the function should return 0 indicating failure.
int nameStar (Star *s, char* first_name, char* sur_name)
{
    if(s == NULL) return 0;
    if(first_name == NULL || sur_name == NULL) return 0;

    //if(s->named == 1) return 0;

    if(strlen(first_name)>28 || strlen(sur_name)>28) return 0;

    char name[70] = "";
    strcpy(name, first_name);
    strcat(name, " ");
    strcat(name, sur_name);

    if(strlen(name)>29) return 0;

    strcpy(s->name, name);
	s->named = 1;
	return 1;

}


// The purpose of this function is to find a pair by the name of either spouse (one of the owners of the pair).
// The parameter to this function is the full name of a person.
// If either star in a named pair have this name, the function returns a pointer to this pair,
// If no such pair is found the function returns 0
Pair* findPairByName (char* name)
{
    for(int i=0; i<numPairs; i++) {
        if(strcmp(name, (pairs[i]->h)->name) == 0 || strcmp(name, (pairs[i]->w)->name) == 0) {
            return pairs[i];
        }
    }
	return 0;
}

// The purpose of this function is to save all the data of the program to a binary file called "universe.bin".
// If the file cannot be opened, the function should return 0 indicating failure,
// If everything goes well, the function returns 1 indicating success in saving the universe
int saveUniverse ()
{
    FILE * fp;
	fp = fopen("universe.bin","wb");

	if(!fp) return 0;

    // Buffer arrays
    Star starsArr[numStars];
    Pair pairsArr[numPairs];

    // Load into buffer arrays
    for(int i=0; i<numStars; i++){
        starsArr[i].id = stars[i]->id;
        strcpy(starsArr[i].name, stars[i]->name);
        starsArr[i].named = stars[i]->named;
        starsArr[i].x = stars[i]->x;
        starsArr[i].y = stars[i]->y;
    }

    for(int i=0; i<numPairs; i++) {
        pairsArr[i].d = pairs[i]->d;
        pairsArr[i].h = pairs[i]->h;
        pairsArr[i].w = pairs[i]->w;
    }

    // Write to file
    fwrite(&numStars, sizeof(int), 1, fp);
	fwrite(&numPairs, sizeof(int), 1, fp);
	fwrite(starsArr, sizeof(Star), numStars, fp);
	fwrite(pairsArr, sizeof(Pair), numPairs, fp);


	fclose(fp);

	return 1;
}



// The purpose of this function is to load a universe from a binary file called "universe.bin".
// If the file cannot be opened, the function should return 0 indicating failure,
// If everything goes well, the function returns 1, indicating success in loading the universe
int loadUniverse ()
{
	FILE * fp;
	fp = fopen("universe.bin","rb");

	if(!fp) return 0;

	fread(&numStars, sizeof(int), 1, fp);
	fread(&numPairs, sizeof(int), 1, fp);

	// Buffer arrays
	Star starsArr[numStars];
	Pair pairsArr[numPairs];

	// Load into buffer arrays
	fread(starsArr, sizeof(Star), numStars, fp);
	fread(pairsArr, sizeof(Pair), numPairs, fp);

	// From buffer arrays into the official arrays
	for(int i=0; i<numStars; i++) {
        	stars[i]->id = starsArr[i].id;
        	strcpy(stars[i]->name, starsArr[i].name);
        	stars[i]->named = starsArr[i].named;
        	stars[i]->x = starsArr[i].x;
        	stars[i]->y = starsArr[i].y;
	}

	for(int i=0; i<numPairs; i++) {
        	pairs[i]->d = pairsArr[i].d;
        	pairs[i]->h = pairsArr[i].h;
        	pairs[i]->w = pairsArr[i].w;
	}

	fclose(fp);


	return 1;
}