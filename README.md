# Description

Prints a spinning ASCII torus (mathematical name for a doughnut-shaped object) on the terminal. Original post by Andy Sloane can be found [here](https://www.a1k0n.net/2011/07/20/donut-math.html).

# How to run

* Clone or download and extract the repository
* Open a terminal inside the project directory
* Follow the steps for your Operating System

# For Windows

## Prerequisites for Windows 10 or Windows 11

* Install a compiler like [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/) for the C version
* Install [Python](https://www.python.org/) for the Python version

## How to run the Python version

* Type `python doughnut.py` in the shell to run the Python version

## How to compile or build the C version

* Type `gcc -o doughnut.exe doughnut.c -lm` in the shell to compile using GCC
* Type `clang -o doughnut.exe doughnut.c -lm` in the shell to compile using Clang

## How to run the C version

* Type `.\doughnut.exe` in the shell to run the program
* Type `.\doughnut.exe --enable-dynamic-resolution`  in the shell to run the program with dynamic resolution enabled
* You can also double-click on the executable (a.k.a. a dot-exe file) to run it

# For Linux

## Prerequisites for Debian based distros

* `sudo apt install make` to install Make
* `make deb` to install the required compilers

## Prerequisites for Arch based distros

* `sudo pacman -S make` to install Make
* `make arch` to install the required compilers

## How to compile C version

* `make build` to compile the C version

## How to run the programs

* `make run-c` to run the C version
* `make run-cd` to run the C version with dynamic resolution enabled
* `make py` to run the Python version

## How to clean up the C executable

* `make clean` to clean up after execution

# Updates

* Added support for dynamic resolution based on current terminal size
* Added Windows support
* Added support for dynamic resolution within the same session

# Screenshot

![Screenshot](https://github.com/arkorty/Spinning-ASCII-Torus/blob/main/blob/screenshot.png)
