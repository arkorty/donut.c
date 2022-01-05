#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define clear_terminal() printf("\e[1;1H\e[2J"); // Clears the terminal

// Function allocates memory for the frame buffer
char **allocate_memory(int size) {
    char **array = malloc(size * sizeof(char *));

    for (int i = 0; i < size; i++)
        array[i] = malloc(size * sizeof(char));

    return array;
}

// Function gets the size of the terminal
int get_terminal_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int win_row_size = (int)w.ws_row;
    int win_col_size = (int)w.ws_col;

    if (win_row_size < win_col_size)
        return win_row_size;
    else
        return win_col_size;
}

// Function dumps the frame into the terminal
void dump_frame(char **frame, int size) {
    for (int i = 0; i < size; i++)
        printf("%s\n", frame[i]);
}

// Function builds the frame and returns the frame buffer
char **build_frame(char **frame, int frame_num, int size, int K) {
    float x = frame_num * 0.0093; // Rotational speed around the x axis
    float y = frame_num * 0.0048; // Rotational speed around the y axis

    float cos_x = cos(x), sin_x = sin(x); // Precomputing sines and cosines of x
    float cos_y = cos(y), sin_y = sin(y); // Precomputing sines and cosines of y

    float z_buffer[size][size]; // Declaring buffer for storing z coordinates

    // Initializing frame buffer and z buffer
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            frame[i][j] = ' ';
            z_buffer[i][j] = 0;
        }

    // Loop uses theta to revolve a point around the center of the circle
    // 6.283186 = 2 * Pi = 360°
    for (float theta = 0; theta < 6.283186; theta += 0.031) {
        // Precomputing sines and cosines of theta
        float cos_theta = cos(theta), sin_theta = sin(theta);

        // Loop uses phi to revolve the circle around the center of the torus
        for (float phi = 0; phi < 6.283186; phi += 0.016) {
            // Precomputing sines and cosines of phi
            float cos_phi = cos(phi), sin_phi = sin(phi);

            // Calculating the x and y coordinates of the circle before the
            // revolution
            float circle_x = 2 + 1 * cos_theta, circle_y = 1 * sin_theta;

            // Calculating the x and y coordinates after the revolution
            float x = circle_x * (cos_y * cos_phi + sin_x * sin_y * sin_phi) -
                      circle_y * cos_x * sin_y;
            float y = circle_x * (sin_y * cos_phi - sin_x * cos_y * sin_phi) +
                      circle_y * cos_x * cos_y;

            // Calculating z
            float z = 5 + cos_x * circle_x * sin_phi + circle_y * sin_x;

            // Calculating the inverse of z
            float z_inv = 1 / z;

            // Calculating x and y coordinates of the 2D projection
            int x_proj = size / 2 + K * z_inv * x;
            int y_proj = size / 2 - K * z_inv * y;

            // Calculating luminous intensity
            float lumi_int =
                cos_phi * cos_theta * sin_y - cos_x * cos_theta * sin_phi -
                sin_x * sin_theta +
                cos_y * (cos_x * sin_theta - cos_theta * sin_x * sin_phi);

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

int main() {
    // Getting the size of the terminal
    const int size = get_terminal_size();

    // Allocating memory to the frame buffer
    char **frame = allocate_memory(size);

    // Loop rotates the torus around both the axes
    for (int frame_num = 0; true; frame_num++) {

        // Building and dumping the frame into the terminal
        int konst = size * 5 * 3 / (8 * (1 + 2));
        dump_frame(build_frame(frame, frame_num, size, konst), size);

        // Clears the screen
        clear_terminal();
    }

    return 0;
}
