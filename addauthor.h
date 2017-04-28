#include <stdbool.h>

#ifndef __DBIANCHI_ADDAUTHOR__
#define __DBIANCHI_ADDAUTHOR__

/**
 * promptForStreams
 * Prompts the user to enter a list of streams
 * IN: char streamList[]
 * OUT: None
 * POST: List of streams is entered by the user
 * ERROR: If the user does not enter the list of streams correctly, an error may occur later in the program
 **/
void promptForStreams(char streamList[]);

/**
 * updateStreams
 * Based on user input, either adds or removes a user from stream(s)
 * IN: char username[], char streamList[], bool flag
 * OUT: None
 * POST: User is added or removed from stream(s)
 * ERROR: If the user does not enter the list of streams correctly, an error may occur later in the program
 **/
void updateStreams(char username[], char streamList[], bool flag);

/*bool searchForRemoveFlag(char * args[]);*/

/**
 * clearArray
 * Sets an entire array to null terminators
 * IN: char string[], int length
 * OUT: None
 * POST: Entire array is cleared (set to null terminators)
 * ERROR: If the variable length is greater than the actual string, an error will occur
 **/
void clearArray(char string[], int length);

#endif
