#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#define STEP_ROT_X 0.157079633
#define STEP_ROT_Y 0.078539816
#define STEP_THETA 0.130899694
#define STEP_ALPHA 0.065449847
#elif defined(__linux__)
#include <sys/ioctl.h>
#include <unistd.h>
#define STEP_ROT_X 0.012271846
#define STEP_ROT_Y 0.006135923
#define STEP_THETA 0.024543693
#define STEP_ALPHA 0.016028534
#endif

// Clears the terminal
void clear_terminal() {
#if defined(_WIN32)
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
#elif defined(__linux__)
    printf("\e[1;1H\e[2J");
#endif
}

// Function allocates memory for the frame buffer
char **allocate_memory(int size) {
    char **array = malloc(size * sizeof(char *));
    for (int i = 0; i < size; ++i)
        array[i] = malloc(size * sizeof(char));

    return array;
}

// Function gets the size of the terminal
int terminal_size() {
    int size;
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO c;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
    int row = (int)(c.srWindow.Bottom - c.srWindow.Top + 1);
    int col = (int)(c.srWindow.Right - c.srWindow.Left + 1);
    size = row < col ? row : col;
#elif defined(__linux__)
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    size = w.ws_row < w.ws_col ? (int)w.ws_row : (int)w.ws_col;
#endif
    return size;
}

// Function dumps the frame into the terminal
void dump_frame(char **frame, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            putchar(frame[i][j]);
        putchar('\n');
    }
}

// Function builds the frame and returns the frame buffer
char **build_frame(char **frame, int size, int frmno, int k) {
    float x = frmno * STEP_ROT_X; // Rotational speed around the x axis
    float y = frmno * STEP_ROT_Y; // Rotational speed around the y axis

    float cos_x = cos(x), sin_x = sin(x); // Precomputing sines and cosines of x
    float cos_y = cos(y), sin_y = sin(y); // Precomputing sines and cosines of y

    float z_buffer[size][size]; // Declaring buffer for storing z coordinates

    // Initializing frame buffer and z buffer
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            frame[i][j] = ' ';
            z_buffer[i][j] = 0;
        }

    // Loop uses theta to revolve a point around the center of the circle
    // 6.283186 = 2 * Pi = 360°
    for (float theta = 0; theta < 6.283186; theta += STEP_THETA) {
        // Precomputing sines and cosines of theta
        float cos_theta = cos(theta), sin_theta = sin(theta);

        // Loop uses alpha to revolve the circle around the center of the torus
        for (float alpha = 0; alpha < 6.283186; alpha += STEP_ALPHA) {
            // Precomputing sines and cosines of alpha
            float cos_alpha = cos(alpha), sin_alpha = sin(alpha);

            // Calculating the x and y coordinates of the circle before the
            // revolution
            float circle_x = 2 + 1 * cos_theta, circle_y = 1 * sin_theta;

            // Calculating the x and y coordinates after the revolution
            float x =
                circle_x * (cos_y * cos_alpha + sin_x * sin_y * sin_alpha) -
                circle_y * cos_x * sin_y;
            float y =
                circle_x * (sin_y * cos_alpha - sin_x * cos_y * sin_alpha) +
                circle_y * cos_x * cos_y;

            // Calculating z
            float z = 5 + cos_x * circle_x * sin_alpha + circle_y * sin_x;

            // Calculating the inverse of z
            float z_inv = 1 / z;

            // Calculating x and y coordinates of the 2D projection
            int x_proj = size / 2 + k * z_inv * x;
            int y_proj = size / 2 - k * z_inv * y;

            // Calculating luminous intensity
            float lumi_int =
                cos_alpha * cos_theta * sin_y - cos_x * cos_theta * sin_alpha -
                sin_x * sin_theta +
                cos_y * (cos_x * sin_theta - cos_theta * sin_x * sin_alpha);

            /* Checking if surface is pointing away from the point of view
             * Also checking if the point is closer than any other point
             * previously plotted
             */
            if (lumi_int > 0 && z_inv > z_buffer[x_proj][y_proj]) {
                z_buffer[x_proj][y_proj] = z_inv;

                // Bringing the value of luminance between 0 to 11
                int lumi_idx = lumi_int * 8;

                /* Storing an appropriate character that represents the correct
                 * amount of luminance
                 */
                frame[x_proj][y_proj] = ".,-~:;=!*#$@"[lumi_idx];
            }
        }
    }

    // Returning the frame buffer
    return frame;
}

int main(int argc, char **argv) {

    // Getting the size of the terminal
    int size = terminal_size();

    // Allocating memory to the frame buffer
    char **frame = allocate_memory(size);

    int k = size * 5 * 3 / (8 * (1 + 2));

    if (argc > 1 && (strcmp(argv[1], "-d") == 0 ||
                     strcmp(argv[1], "--enable-dynamic-resolution") == 0)) {
        // Loop rotates the torus around both the axes
        for (int frmno = 0; true; frmno++) {
            if (frmno % 256 == 1 && size - terminal_size() != 0) {
                // Getting the size of the terminal
                size = terminal_size();

                // Allocating memory to the frame buffer
                frame = allocate_memory(size);

                k = size * 5 * 3 / (8 * (1 + 2));
            }

            // Building and dumping the frame into the terminal
            dump_frame(build_frame(frame, size, frmno, k), size);

            // Clears the screen
            clear_terminal();
        }
    } else {
        for (int frmno = 0; true; frmno++) {
            // Building and dumping the frame into the terminal
            dump_frame(build_frame(frame, size, frmno, k), size);

            // Clears the screen
            clear_terminal();
        }
    }

    return 0;
}