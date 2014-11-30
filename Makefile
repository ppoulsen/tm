CC = gcc
DEPS = tm.h
OBJ = tm.o proj2.o
BIN = proj2

all: clean $(BIN)

.PHONY: clean

clean:
	rm -f $(BIN) *.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $<

$(BIN): $(OBJ)
	$(CC) -g -o $@ $^
