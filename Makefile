deb:
	@sudo apt install gcc python3

arch:
	@sudo pacman -S gcc python3

build: doughnut.c
	@gcc -o doughnut.out doughnut.c -lm

run-c: doughnut.out
	@./doughnut.out

run-cd: doughnut.out
	@./doughnut.out --enable-dynamic-resolution

run-py: doughnut.py
	@python3 doughnut.py

clean: doughnut.out
	@rm doughnut.out
