#include <stdlib.h>
#include <string.h>

#include "tm.h"

#define MAX_TCOUNT_START (32)

void expand_transition_cap(TM *tm) {
	uint32_t i;
	uint32_t newMax;
	transition *newTransitions;
	if (tm->maxTCount == 0) {
		// Set to default starting max
		newMax = MAX_TCOUNT_START;
	}
	else {
		// Set to twice old max
		newMax = tm->maxTCount * 2;
	}
	
	// Allocate a larger chunk of memory
	newTransitions = malloc(sizeof(transition) * newMax);
	
	// Copy old transitions over
	for(i = 0; i < tm->tCount; i++) {
		newTransitions[i].curState[0] = tm->transitions[i].curState[0];
		newTransitions[i].curState[1] = tm->transitions[i].curState[1];
		newTransitions[i].curState[2] = tm->transitions[i].curState[2];
		newTransitions[i].nextState[0] = tm->transitions[i].nextState[0];
		newTransitions[i].nextState[1] = tm->transitions[i].nextState[1];
		newTransitions[i].nextState[2] = tm->transitions[i].nextState[2];
		newTransitions[i].curTape = tm->transitions[i].curTape;
		newTransitions[i].nextTape = tm->transitions[i].nextTape;
		newTransitions[i].dir = tm->transitions[i].dir;
	}
	
	// Update tm
	if (tm->transitions != NULL) {
		free(tm->transitions);
	}
	tm->transitions = newTransitions;
	tm->maxTCount = newMax;
}

void add_transition(TM *tm, const char *tString) {
	uint32_t i;

	// If we're out of space for new transitions, make more
	if (tm->tCount == tm->maxTCount) {
		expand_transition_cap(tm);
	}
	
	// Add transition
	i = tm->tCount;
	tm->transitions[i].curState[0] = tString[0];
	tm->transitions[i].curState[1] = tString[1];
	tm->transitions[i].curState[2] = '\0';
	tm->transitions[i].nextState[0] = tString[6];
	tm->transitions[i].nextState[1] = tString[7];
	tm->transitions[i].nextState[2] = '\0';
	tm->transitions[i].curTape = tString[3];
	tm->transitions[i].nextTape = tString[9];
	tm->transitions[i].dir = tString[11] == 'R' ? R : L;
}

void tm_init(TM *tm, char *tmString) {
	char *tString;
	
	// Initialize to 0
	tm->transitions = NULL;
	tm->maxTCount = 0;
	tm->tCount = 0;
	
	// For each '#' delimited string in tmString, add a transition
	tString = strtok(tmString, "#");
	add_transition(tm, tString);
	
	while((tString = strtok(NULL, "#")) != NULL) {
		add_transition(tm, tString);
	}
}

TM_RESULT test_string(TM *tm, const char *testString) {
	return ACCEPT;
}
