# Description
These programs print a spinning ASCII torus (mathematical name for a doughnut-shaped object) on the terminal. Original post by Andy Sloane can be found [here](https://www.a1k0n.net/2011/07/20/donut-math.html).

# Updates
* Now supports dynamic resolution based on terminal size

# For Linux
## Prerequisites for Debian based distros
* `sudo apt install make` to install Make
* `make deb` to install the required compilers
## Prerequisites for Arch based distros
* `sudo pacman -S make` to install Make
* `make arch` to install the required compilers

## How to run?
* Clone or download the repository
* Open a terminal inside the project directory
* Complete the prerequisites for your OS
* `make c` to run the C version
* `make py` to run the Python version

## How to clean up the executable?
* `make clean` to delete the executable

# Screenshot
![screenshot.png](https://github.com/arkorty/spinning-ascii-torus/blob/main/blob/screenshot.png)
