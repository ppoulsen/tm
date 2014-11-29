CC = gcc
DEPS = tm.h
OBJ = tm.o proj2.o
BIN = proj2

.PHONY: clean

clean:
	rm -f $(BIN) *.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^