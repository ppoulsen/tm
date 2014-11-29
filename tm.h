#ifndef TM_H_
#define TM_H_

#include <stdint.h>

typedef enum {
	ACCEPT,
	REJECT,
	UNKNOWN
} TM_RESULT;

typedef enum {
	L,
	R
} direction;

typedef struct {
	char curState[3];
	char nextState[3];
	char curTape;
	char nextTape;
	direction dir;
} transition;

typedef struct {
	transition *transitions;
	uint8_t tCount; 
} TM;

void tm_init(TM *tm, const char *tmString);
TM_RESULT test_string(TM *tm, const char *testString);

#endif /* TM_H_ */
