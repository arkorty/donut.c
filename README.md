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

* Type `python main.py` in the shell to run the Python version

## How to compile the C version

* Type `gcc -o main.exe main.c -lm` in the shell to compile using GCC
* Type `clang -o main.exe main.c -lm` in the shell to compile using Clang

## How to run the C version

* Type `.\main.exe` in the shell to run the program

# For Linux

## Prerequisites for Debian based distros

* `sudo apt install make` to install Make
* `make deb` to install the required compilers

## Prerequisites for Arch based distros

* `sudo pacman -S make` to install Make
* `make arch` to install the required compilers

## How to run the programs

* `make c` to run the C version
* `make py` to run the Python version

## How to clean up the executable

* `make clean` to clean after execution

# Updates

* Added support for dynamic resolution based on current terminal size
* Added Windows support

# Screenshot

![Screenshot](https://github.com/arkorty/Spinning-ASCII-Torus/blob/main/blob/screenshot.png)
