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
		
		// Split input line into TM definition and input string
		char *tmString = strtok(line, "##");
		char *testString = strtok(NULL, "##");
		
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
