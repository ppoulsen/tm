#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tm.h"

void main(void) {
	char *line = NULL;
	size_t lineSize = 0;

	// Iterate through each line in stdin
	while(getline(&line, &lineSize, stdin) >= 0) {
		TM tm;
		TM_RESULT result;
		uint32_t splitIndex = 0;
		
		// Split input line into TM definition and input string
		while (!(line[splitIndex] == '#' && line[splitIndex+1] == '#')) {
			splitIndex++;
		}
		char tmString[1024] = {'\0'};
		char testString[1024] = {'\0'};
		strncpy(tmString, line, splitIndex);
		strcpy(testString, line + splitIndex + 2);
		
		// Initialize TM and test testString against it
		tm_init(&tm, tmString);
		result = test_string(&tm, testString);
		
		// Print result
		switch(result) {
		case ACCEPT:
			printf("M stops and accepts w\n");
			break;
		case REJECT:
			printf("M stops and rejects w\n");
			break;
		case UNKNOWN:
			printf("Do not know\n");
			break;
		}
	} 
}
