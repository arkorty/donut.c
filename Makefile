CC=clang
OL=Ofast

build: torus.c
	$(CC) -$(OL) -o torus.out torus.c -lm

run: torus.out
	./torus.out

clean: torus.out
	rm torus.out
