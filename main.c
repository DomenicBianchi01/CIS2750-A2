/**********************
 Assignment #1
 Domenic Bianchi
 0921557
 January 28, 2017
 This program reads in a C++ file and converts the code to a C file.
 ************************/

#include <stdio.h>
#include <stdlib.h>
#include "parseFile.h"
#include "converter.h"

int main(int argc, char * argv[]) {

	Node * listOfWords = NULL;
	FILE * dataFile = fopen(argv[1], "r");

	if (argc != 2) {

		printf("Invalid number of parameters\n");
		return 1;
	}

	if (dataFile == NULL) {

		printf("File could not be opened\n");
		return 1;
	}

	/*Break up c++ code into keywords*/
	listOfWords = parseDataFile(dataFile, listOfWords);

	replaceClassWithStruct(listOfWords);

	saveToFile(listOfWords, argv[1]);

	destroyList(listOfWords);

	return 0;
}
