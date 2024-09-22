#include <math.h>
#include <signal.h>
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

volatile sig_atomic_t keep_running = 1;

void sigint_handler(int sig) { keep_running = 0; }

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

// Frees up the heap
void cleanup(char **buffer, size_t term_size) {
    if (buffer) {
        for (size_t i = 0; i < term_size; ++i) {
            free(buffer[i]);
        }
        free(buffer);
    }
    clear_terminal();
}

// Function allocates memory for the frame buffer
char **allocate_memory(size_t term_size) {
    char **array = malloc(term_size * sizeof(char *));
    for (size_t i = 0; i < term_size; ++i)
        array[i] = malloc(term_size * sizeof(char));

    return array;
}

// Function gets the size of the terminal
size_t terminal_size() {
    size_t term_size;
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO c;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
    size_t row = (size_t)(c.srWindow.Bottom - c.srWindow.Top + 1);
    size_t col = (size_t)(c.srWindow.Right - c.srWindow.Left + 1);
    term_size = row < col ? row : col;
#elif defined(__linux__)
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    term_size = w.ws_row < w.ws_col ? (size_t)w.ws_row : (size_t)w.ws_col;
#endif
    return term_size;
}

// Function dumps the frame into the terminal
void dump_frame(char **buffer, size_t term_size) {
    for (size_t i = 0; i < term_size; ++i) {
        for (size_t j = 0; j < term_size; ++j)
            putchar(buffer[i][j]);
        putchar('\n');
    }
}

// Function builds the frame and returns the frame buffer
char **build_frame(char **buffer, size_t term_size, size_t i, size_t scrn_dist) {
    float x = i * STEP_ROT_X; // Rotational speed around the x axis
    float y = i * STEP_ROT_Y; // Rotational speed around the y axis

    float cos_x = cos(x), sin_x = sin(x); // Precomputing sines and cosines of x
    float cos_y = cos(y), sin_y = sin(y); // Precomputing sines and cosines of y

    float z_buffer[term_size][term_size]; // Declaring buffer for storing z coordinates

    // Initializing frame buffer and z buffer
    for (size_t i = 0; i < term_size; ++i)
        for (size_t j = 0; j < term_size; ++j) {
            buffer[i][j] = ' ';
            z_buffer[i][j] = 0;
        }

    // Loop uses theta to revolve a posize_t around the center of the circle
    // 6.283186 = 2 * Pi = 360°
    for (float theta = 0; theta < 6.283186; theta += STEP_THETA) {
        // Precomputing sines and cosines of theta
        float cos_theta = cos(theta), sin_theta = sin(theta);

        // Loop uses alpha to revolve the circle around the center of the donut
        for (float alpha = 0; alpha < 6.283186; alpha += STEP_ALPHA) {
            // Precomputing sines and cosines of alpha
            float cos_alpha = cos(alpha), sin_alpha = sin(alpha);

            // Calculating the x and y coordinates of the circle before the
            // revolution
            float circle_x = 2 + 1 * cos_theta, circle_y = 1 * sin_theta;

            // Calculating the x and y coordinates after the revolution
            float x = circle_x * (cos_y * cos_alpha + sin_x * sin_y * sin_alpha) - circle_y * cos_x * sin_y;
            float y = circle_x * (sin_y * cos_alpha - sin_x * cos_y * sin_alpha) + circle_y * cos_x * cos_y;

            // Calculating z
            float z = 5 + cos_x * circle_x * sin_alpha + circle_y * sin_x;

            // Calculating the inverse of z
            float z_inv = 1 / z;

            // Calculating x and y coordinates of the 2D projection
            size_t x_proj = (float)term_size / 2 + scrn_dist * z_inv * x;
            size_t y_proj = (float)term_size / 2 - scrn_dist * z_inv * y;

            // Calculating luminous intensity
            float lumi_size_t = cos_alpha * cos_theta * sin_y - cos_x * cos_theta * sin_alpha - sin_x * sin_theta +
                                cos_y * (cos_x * sin_theta - cos_theta * sin_x * sin_alpha);

            /* Checking if surface is pointing away from the posize_t of view
             * Also checking if the posize_t is closer than any other point
             * previously plotted
             */
            if (lumi_size_t > 0 && z_inv > z_buffer[x_proj][y_proj]) {
                z_buffer[x_proj][y_proj] = z_inv;

                // Bringing the value of luminance between 0 to 11
                size_t lumi_idx = lumi_size_t * 8;

                /* Storing an appropriate character that represents the correct
                 * amount of luminance
                 */
                buffer[x_proj][y_proj] = ".,-~:;=!*#$@"[lumi_idx];
            }
        }
    }

    // Returning the frame buffer
    return buffer;
}

void help(char *prog_name) {
    printf("Usage: %s [OPTION]...\n\
Puts a spinning ASCII donut on the terminal.\n\
\n\
Options:\n\
  -f, --frames          number of frames to be rendered\n\
  -d, --dynamic         enables dynamic resolution\n\
  -h, --help            displays this help screen and exits\n\
\n\
Examples:\n\
  %s --help             displays this help screen and exits\n\
  %s -d -f 256          renders 256 frames with dynamic resolution enabled\n",
           prog_name, prog_name, prog_name);
}

void runner(bool dynamic, bool limit, size_t frames) {
    // Set up the signal handler
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Allocating memory to the frame buffer according to terminal size
    size_t term_size = terminal_size();
    size_t scrn_dist = term_size * 5 * 3 / (8 * (1 + 2));
    char **buffer = allocate_memory(term_size);

    // Loop rotates the donut around both the axes
    for (size_t i = 0; (keep_running) && (!limit || i < frames); ++i) {
        if (dynamic && i % 32 == 0 && term_size - terminal_size() != 0) {
            // Frees the old frame buffer
            cleanup(buffer, term_size);
            // Reallocates the frame buffer as per new terminal size
            term_size = terminal_size();
            buffer = allocate_memory(term_size);
            // Calculates screen distance based on terminal size
            scrn_dist = term_size * 5 * 3 / (8 * (1 + 2));
        }
        // Building the frame
        buffer = build_frame(buffer, term_size, i, scrn_dist);
        // Dumping the frame into the terminal
        dump_frame(buffer, term_size);
        // Clearing the frame from the terminal
        clear_terminal();
    }

    // Clean up the heap
    cleanup(buffer, term_size);
}

int main(int argc, char **argv) {
    bool limit = false;
    bool dynamic = false;
    size_t frames = 0;
    if (argc > 1) {
        for (size_t i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
                help(argv[0]);
                return EXIT_SUCCESS;
            } else if ((!strcmp(argv[i], "-f") || !strcmp(argv[i], "--frames")) && i + 1 < argc) {
                limit = true;
                frames = atoi(argv[++i]);
            } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dynamic")) {
                dynamic = true;
            } else {
                printf("Invalid argument passed\n");
                return EXIT_FAILURE;
            }
        }
    }

    // Calls the runner function
    runner(dynamic, limit, frames);

    return EXIT_SUCCESS;
}
