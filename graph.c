#include <stdlib.h>
#include <stdio.h>
#include "uni.h"

char fbr[MaxRow][MaxCol];	// the frame buffer

// Clear the frame buffer, i.e. fill all positions with the space character
// 3 lines of code
void clearBuffer ()
{
    for(int i=0; i<=MaxY; i++) {
		for(int j=0; j<=MaxX; j++) {
			fbr[i][j] = ' ';
		}
	}
}

// Plot a character into the frame buffer, i.e. store the character in the frame buffer at location row, col.
// Before attempting to plot the character the function should make sure that row and col are valid values (e.g. row is not < 0)
// If the location is not valid, the character is not plotted and the function should not cause the program to crash
// 3 lines of code
void plot (int row, int col, char c)
{
    if(row >= 0 && row < MaxRow && col >= 0 && col < MaxCol) {
		fbr[row][col] = c;
	}
}

// Peek at the frame buffer's location given by row and column indices
// Before attempting to peek at a location the function should make sure that row and col are valid values (e.g. row is not < 0)
// If the location is not valid the function should return 0,
// otherwise the function returns the character stored at that location
// 3 lines of code
char peek (int row, int col)
{
	if(row < 0 || row >= MaxRow || col < 0 || col >= MaxCol) {
		return 0;
	}

	return fbr[row][col];
}

// Write a string into the frame buffer starting at location row, col.
// The string is written horizontally starting at location row,col
// If the text is too long to fit into the frame buffer (e.g. we reach the edge of the frame before all chars of the string are written),
// the function should not crash and any remaining chars in the string are ignored (clipped)
// The function should not crash if any of the parameters are invalid
// 11 lines of code
void writeAt (int row, int col, char str[])
{
    if(row < 0 || row >= MaxRow || col < 0 || col >= MaxCol) {
		return;
	}

	int stringLen = strlen(str);

	for(int i=0; i<stringLen; i++) {
		if(col+i >= MaxCol) {
			break;
		}
		fbr[row][col+i] = str[i];
	}
}

// Display (print) the whole frame buffer on the screen
// 6 lines of code
void showBuffer ()
{
    for(int i=0; i<=MaxY; i++) {
		for(int j=0; j<=MaxX; j++) {
			printf("%c", fbr[i][j]);
			if(j==MaxX) {
				printf("\n");
			}
		}
	}
}
