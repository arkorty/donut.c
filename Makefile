deb:
	@sudo apt install gcc python3

arch:
	@sudo pacman -S gcc python3

c: main.c
	@gcc -o main.out main.c -lm
	@./main.out

py: main.py
	@python3 main.py

clean: main.out
	@rm main.out
