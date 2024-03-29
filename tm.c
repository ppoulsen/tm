#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tm.h"

#define MAX_TCOUNT_START (32)
#define MAX_STATECOUNT_START (32)
#define MAX_TRANSITIONS (20)

typedef struct {
	char tape[1024];
	uint8_t tapePos;
	char curState[3];
} TM_STATE;

typedef struct {
	TM_STATE *states;
	uint32_t stateCount;
	uint32_t maxStateCount;
	uint8_t transitionCount;
} TEST_STATE;

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

void expand_state_cap(TEST_STATE *ts) {
	uint32_t i;
	uint32_t newMax;
	TM_STATE *newStates;
	if (ts->maxStateCount == 0) {
		// Set to default starting max
		newMax = MAX_STATECOUNT_START;
	}
	else {
		// Set to twice old max
		newMax = ts->maxStateCount * 2;
	}
	
	// Allocate a larger chunk of memory
	newStates = malloc(sizeof(TM_STATE) * newMax);
	
	// Copy old transitions over
	for(i = 0; i < ts->stateCount; i++) {
		strncpy(newStates[i].tape, ts->states[i].tape, 1024);
		strncpy(newStates[i].curState, ts->states[i].curState, 3);
		newStates[i].tapePos = ts->states[i].tapePos;
	}
	
	// Update tm
	if (ts->states != NULL) {
		free(ts->states);
	}
	ts->states = newStates;
	ts->maxStateCount = newMax;
}

void add_state(TEST_STATE *ts, char *curState, char tape[1024], uint32_t curPos) {
	uint32_t i;

	// If we're out of space for new states, make more
	if (ts->stateCount == ts->maxStateCount) {
		expand_state_cap(ts);
	}
	
	// Add state
	i = ts->stateCount;
	strncpy(ts->states[i].tape, tape, 1024);
	ts->states[i].tapePos = curPos;
	strncpy(ts->states[i].curState, curState, 3);
	ts->stateCount++;
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

	tm->tCount++;
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

void perform_transitions(TM *tm, TEST_STATE *ts) {
	uint32_t i;
	uint32_t stateCount = ts->stateCount;
	
	// Add new states to end 
	for (i = 0; i < stateCount; i++) {
		uint32_t j;
		for (j = 0; j < tm->tCount; j++) {
			if (
					(strncmp(ts->states[i].curState, tm->transitions[j].curState, 3) == 0) &&
					(ts->states[i].tape[ts->states[i].tapePos] == tm->transitions[j].curTape)
			) {
					uint32_t tapePos = ts->states[i].tapePos;
					char oldTape = tm->transitions[j].curTape;
					uint32_t newTapePos = tm->transitions[j].dir == R ? tapePos + 1 : tapePos - 1;
					ts->states[i].tape[tapePos] = tm->transitions[j].nextTape;
					add_state(ts, tm->transitions[j].nextState, ts->states[i].tape, newTapePos); 
					ts->states[i].tape[tapePos] = oldTape;
			}
		}
	}

	// Shift states downward to delete old ones
	for (i = 0; i < stateCount && i < ts->stateCount - stateCount; i++) {
		// Add state
		strncpy(ts->states[i].tape, ts->states[i + stateCount].tape, 1024);
		ts->states[i].tapePos = ts->states[i + stateCount].tapePos;
		strncpy(ts->states[i].curState, ts->states[i + stateCount].curState, 3);
	}
	ts->stateCount -= stateCount;
}

uint8_t accepted(TEST_STATE *ts) {
	uint32_t i;
	for (i = 0; i < ts->stateCount; i++) {
		if (strncmp(ts->states[i].curState, "qa", 3) == 0) {
			return 1;
		}
	}
	return 0;
}

uint8_t rejected(TEST_STATE *ts) {
	uint32_t i;
	for (i = 0; i < ts->stateCount; i++) {
		if (strncmp(ts->states[i].curState, "qr", 3) != 0) {
			return 0;
		}
	}
	return 1;
}

TM_RESULT test_string(TM *tm, const char *testString) {
	uint32_t i = 0;
	// Initialize testState for tracking all possible tapes
	TEST_STATE testState;
	char tape[1024];
	testState.states = NULL;
	testState.stateCount = 0;
	testState.maxStateCount = 0;
	testState.transitionCount = 0;
	
	// Initialize tape
	for (i = 0; i < 1024; i++) {
		tape[i] = '_';
	}
	tape[1023] = '\0';
	i = 0;
	while(testString[i] != '#' && testString[i] != 'e') {
		tape[i] = testString[i];
		i++;
	}
	
	// Add first TM_STATE at start state
	add_state(&testState, "q0", tape, 0);
	
	while (testState.transitionCount < MAX_TRANSITIONS) {
		perform_transitions(tm, &testState);
		
		if (accepted(&testState)) {
			return ACCEPT;
		} else if (rejected(&testState)) {
			return REJECT;
		} 
		
		testState.transitionCount++;
	} 
	return UNKNOWN;
}
