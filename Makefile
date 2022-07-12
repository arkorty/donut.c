CC=gcc
CFLAGS=-Ofast -lm
BIN=torus

$(BIN): $(BIN).c
	$(CC) $(CFLAGS) $(BIN).c -o $(BIN)

clean: $(BIN)
	rm $(BIN)
