deb:
	@sudo apt install gcc python3

arch:
	@sudo pacman -S gcc python3

c: doughnut.c
	@gcc -o doughnut.out doughnut.c -lm
	@./doughnut.out

py: doughnut.py
	@python3 doughnut.py

clean: doughnut.out
	@rm doughnut.out
