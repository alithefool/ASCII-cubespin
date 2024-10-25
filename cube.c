#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
// Include unistd.h for Unix-based systems to use usleep function
#include <unistd.h>
#else
// For Windows, define a custom usleep function using Windows API
#include <windows.h>
void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    // Convert microseconds to 100-nanosecond intervals for Windows API
    ft.QuadPart = -(10 * usec);

    // Create a waitable timer
    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
#endif

// Global variables for rotation angles
float A, B, C;

// Settings for the cube and screen dimensions
float cubeWidth = 20;  // Cube side length
int width = 160, height = 44;  // Screen width and height
float zBuffer[160 * 44];  // Z-buffer for depth calculations
char buffer[160 * 44];  // Buffer for storing characters to display
int backgroundASCIICode = '.';  // Character used for background
int distanceFromCam = 100;  // Distance from the camera
float horizontalOffset;  // Horizontal offset for cube drawing
float K1 = 40;  // Scaling factor for projection

// Cube rotation speed
float incrementSpeed = 0.6;

// Variables for 3D to 2D projection calculations
float x, y, z;
float ooz;  // One over z (depth)
int xp, yp;  // Projected 2D screen coordinates
int idx;  // Buffer index for 2D screen position

// Function to calculate the projected X coordinate of a 3D point
float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

// Function to calculate the projected Y coordinate of a 3D point
float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

// Function to calculate the projected Z coordinate of a 3D point
float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

// Function to calculate the screen coordinates for a surface point
void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
    // Calculate 3D position after rotation
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    // Calculate the reciprocal of z for depth
    ooz = 1 / z;

    // Project 3D coordinates to 2D screen coordinates
    xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    // Calculate buffer index
    idx = xp + yp * width;

    // Update buffer if the point is within screen bounds and closer
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    // Clear the screen
    printf("\x1b[2J");

    // Main loop to continuously render frames
    while (1) {
        // Reset buffer and Z-buffer for each frame
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);

        // Draw the first cube
        cubeWidth = 20;
        horizontalOffset = -2 * cubeWidth;
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                // Calculate all six faces of the cube
                calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        // Draw the second cube
        cubeWidth = 10;
        horizontalOffset = 1 * cubeWidth;
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        // Draw the third cube
        cubeWidth = 5;
        horizontalOffset = 8 * cubeWidth;
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        // Move cursor to the top-left of the screen
        printf("\x1b[H");

        // Render the buffer to the screen
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : 10);
        }

        // Update rotation angles for animation
        A += 0.05;
        B += 0.05;
        C += 0.01;

        // Pause for a short time before rendering the next frame
        usleep(8000 * 2);
    }
    return 0;
}
