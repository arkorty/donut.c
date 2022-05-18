# Description

Puts a spinning ASCII torus (mathematical name for a doughnut-shaped object) on the terminal. Original post by Andy Sloane can be found [here](https://www.a1k0n.net/2011/07/20/donut-math.html).

# How to run

* Clone or download and extract the repository
* Open a terminal inside the project directory
* Follow the steps for your Operating System

# For Windows

## Prerequisites for Windows

* Install a compiler like [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/) for the C version
* Install [Python](https://www.python.org/) for the Python version

## How to run the Python version

* Type `python torus.py` in the shell to run the Python version

## How to build and run the C version

* Type `gcc -o torus torus.c -lm` in the shell to compile using GCC
* Type `clang -o torus torus.c -lm` in the shell to compile using Clang
* Type `.\torus` in the shell to run the program

# For Linux

## Prerequisites for Debian based distributions

* `sudo apt install python3 gcc` to install the required compilers

## Prerequisites for Arch based distributions

* `sudo pacman -S python3 gcc` to install the required compilers

## How to compile and run C version

* `gcc -o torus torus.c -lm` to compile the C version
* `./torus` to run the C version

## How to run the Python version

* `python3 torus.py` in the shell to run the Python version

# Updates  to the C version

* Added support for dynamic resolution based on current terminal size
* Added Windows support
* Added support for on-the-fly dynamic resolution
* Added help option to the program

# Screenshot

![Screenshot](https://github.com/arkorty/Spinning-ASCII-Torus/blob/main/blob/screenshot.png)
