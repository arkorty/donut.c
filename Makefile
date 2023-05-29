CC=gcc
CFLAGS=-O2 -lm
BIN=torus

$(BIN): $(BIN).c
	$(CC) $(BIN).c -o $(BIN) $(CFLAGS)

clean: $(BIN)
	rm $(BIN)

run: $(BIN)
	./$(BIN) -d
