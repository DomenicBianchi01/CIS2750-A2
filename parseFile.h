#include "converter.h"
#include <stdbool.h>

#ifndef __DBIANCHI_PARSEFILE__
#define __DBIANCHI_PARSEFILE__

/**
 * parseDataFile
 * Loads keywords into a linked list
 * IN: FILE * dataFile, Node * lsit
 * OUT: Node * list
 * POST: A linked list containing all of the keywords from the C++ file is created
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
Node * parseDataFile(FILE * dataFile, Node * list);

/**
 * saveToFile
 * Save linked list of keywords to a .c file. Also, format the keywords
 * IN: Node * list, char fileName[]
 * OUT: NONE
 * POST: A new .c file is created
 * ERROR: If this program did not convert the code correctly, incorrect output will be produced
 **/
void saveToFile(Node * list, char fileName[]);

/**
 * addIndenting
 * Insert indenting (4 spaces) in the .c file where appropriate
 * IN: bool * indent, int counter, FILE * dataFile
 * OUT: NONE
 * POST: Indenting (4 spaces) added to .c file
 * ERROR: If this program did not convert the code correctly, indents might be added in incorrect placeds
 **/
void addIndenting(bool * indent, int counter, FILE * dataFile);

/**
 * clearArray
 * Clears all the elements of a char array so the array can be re-used for other purposes elsewhere in the program
 * IN: char string[], int length
 * OUT: NONE
 * POST: Empties character array
 * ERROR: If the length variable (int length) is shorter than the actually length of the char array, not all elements of the array will be cleared which will result in errors when the array is re-used else where in the program
 **/
void clearArray(char string[], int length);

#endif