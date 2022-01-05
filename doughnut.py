from os import name, system, get_terminal_size
from math import sin, cos, pi


# Screen dimensions
screen_width = screen_height = (
    get_terminal_size().lines
    if get_terminal_size().lines < get_terminal_size().columns
    else get_terminal_size().columns
)

# Jump values for theta and phi
theta_jump = 0.07
phi_jump = 0.02

R1 = 1
R2 = 2
K2 = 5

# Calculate K1 based on screen size: The maximum x-distance occurs roughly at
# the edge of the torus, which is at x=R1+R2, z=0.  we want that to be
# displaced 3/8ths of the width of the screen, which is 3/4th of the way from
# the center to the side of the screen. We also substract 2 from the
# screen_width to introduce a border.
# screen_width-2*K2*3/(8*(R1+R2)) = K1
K1 = (screen_width - 2) * K2 * 3 / (8 * (R1 + R2))


def render_frame(A, B):

    # Precomputing sines an cosines
    cosA = cos(A)
    sinA = sin(A)
    cosB = cos(B)
    sinB = sin(B)

    # Creating 2D arrays
    output = []
    zbuffer = []

    # Initializing 2D arrays
    for i in range(screen_height):
        output.append([" "] * (screen_width))
        zbuffer.append([0] * (screen_width))

    # Value of theta increases until it completes a rotation around the
    # cross-sectional circle of the torus
    theta = 0
    while theta < 2 * pi:

        # Precomputing sines and cosines of theta
        costheta = cos(theta)
        sintheta = sin(theta)

        # Value of phi increases until the circle completes a revolution around
        # the center of the torus
        phi = 0
        while phi < 2 * pi:

            # Precomputing sines and cosines of phi
            cosphi = cos(phi)
            sinphi = sin(phi)

            # (x, y) coordinates of the circle before revolution
            circlex = R2 + R1 * costheta
            circley = R1 * sintheta

            # Final 3D coordinates after revolution
            x = circlex * (cosB * cosphi + sinA * sinB * sinphi) - circley * cosA * sinB
            y = circlex * (sinB * cosphi - sinA * cosB * sinphi) + circley * cosA * cosB
            z = K2 + cosA * circlex * sinphi + circley * sinA

            # Inverse of z
            zinv = 1 / z

            # (x, y) coordinates of the 2D projection
            xp = int(screen_width / 2 + K1 * zinv * x)
            yp = int(screen_height / 2 - K1 * zinv * y)

            # Calculating luminance
            L = cosphi * costheta * sinB - cosA * costheta * sinphi - sinA * sintheta
            +cosB * (cosA * sintheta - costheta * sinA * sinphi)

            # L ranges from -sqrt(2) to +sqrt(2).  If it's < 0, the surface
            # is pointing away from us, so we won't bother trying to plot it.
            if L > 0 and zinv > zbuffer[xp][yp]:

                # Test against the z-buffer. Larger 1/z means that the pixel is
                # closer to the viewer than what's already plotted.

                zbuffer[xp][yp] = zinv
                luminance_index = int(L * 8)

                # Now the luminance_index is in the range 0..11 (8*sqrt(2)
                # = 11.3) now we lookup the character corresponding to the
                # luminance and plot it in our output.
                output[xp][yp] = ".,-~:;=!*#$@"[luminance_index]

            phi += phi_jump

        theta += theta_jump

    # Clearing old frame and printing new frame
    system("cls") if name is "nt" else system("clear")
    for i in range(screen_width):
        for j in range(screen_height):
            print(output[i][j], end="")
        print()


if __name__ == "__main__":
    A = 0
    B = 0

    # Calling render_frame() n times
    while True:
        render_frame(A, B)
        A += 0.07
        B += 0.03
