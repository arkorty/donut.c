CC=gcc
CFLAGS=-Ofast -lm
BIN=torus

$(BIN): $(BIN).c
	$(CC) $(BIN).c -o $(BIN) $(CFLAGS)

clean: $(BIN)
	rm $(BIN)
