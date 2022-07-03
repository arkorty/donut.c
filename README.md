# Description

Puts a spinning ASCII torus (mathematical name for a doughnut-shaped object) on the terminal. Original post by Andy Sloane can be found [here](https://www.a1k0n.net/2011/07/20/donut-math.html).

# How to run

* Clone or download and extract the repository
* Open a terminal inside the project directory
* Follow the steps for your Operating System

# For Windows

## Prerequisites for Windows

* Install a compiler like [MinGW-w64](https://sourceforge.net/projects/mingw-w64/)

## How to build and run

* `gcc -Ofast -o torus.exe torus.c -lm` in the shell to compile using MinGW-w64
* `.\torus.exe` in the shell to run the program

# For Linux

## Prerequisites for Debian based distributions

* `sudo apt install gcc` to install the required compilers

## Prerequisites for Arch based distributions

* `sudo pacman -S gcc` to install the required compilers

## How to compile, run and clean

* `make build` to compile
* `make run` to run
* `make clean` to clean

# Updates

* Added support for dynamic resolution based on current terminal size
* Added Windows support
* Added support for on-the-fly dynamic resolution
* Added help option to the program

# Screenshot

![Screenshot](blob/screenshot.png)
