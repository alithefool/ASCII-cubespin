Rotating ASCII Cube Animation

This program renders an animated 3D ASCII representation of rotating cubes in the terminal using simple 3D projection techniques and a z-buffer algorithm. The cubes appear to rotate in 3D space, giving a depth effect.
Features

    Multiple Rotating Cubes: Three cubes with different sizes and positions are animated.
    ASCII Art Representation: Uses various ASCII characters to represent different surfaces of the cubes (@, $, ~, #, ;, +).
    Real-Time Animation: Continuously updates the screen to show rotating cubes, creating a smooth animation.
    Cross-Platform Compatibility: Works on both Unix-like systems (Linux, macOS) and Windows.

How It Works

    3D to 2D Projection: The program converts 3D coordinates of cube vertices to 2D screen coordinates using a series of trigonometric transformations.
    Rotation Angles: The cubes rotate around the X, Y, and Z axes using angles A, B, and C.
    Z-Buffering: A z-buffer is used to keep track of the depth of each point on the screen, ensuring that surfaces closer to the camera overwrite those further away.
    Rendering Loop: The program runs in an infinite loop, continuously updating rotation angles and re-rendering the cubes.

Prerequisites

    A C compiler (GCC, Clang, etc.)
    Terminal that supports ANSI escape codes (most modern terminals)

How to Compile and Run
On Unix-like Systems (Linux, macOS)

    Open a terminal.
    Compile the program using gcc or any C compiler:

    bash

gcc -o rotating_cube rotating_cube.c -lm

Run the compiled program:

bash

    ./rotating_cube

On Windows

    Open Command Prompt or PowerShell.
    Compile the program using a C compiler that supports Windows, like MinGW:

gcc -o rotating_cube rotating_cube.c -lm

Run the compiled program:

bash

    rotating_cube.exe

Program Configuration

You can modify the following settings in the code to change the behavior:

    Cube Dimensions (cubeWidth): Adjust the size of the cubes.
    Screen Dimensions (width, height): Change the terminal screen size for rendering.
    Rotation Speed (incrementSpeed): Control the speed at which the cubes rotate.
    Characters for Surfaces: Customize the ASCII characters used for different cube surfaces.

Code Structure

    calculateX, calculateY, calculateZ: Functions for projecting 3D coordinates to 2D.
    calculateForSurface: Calculates the 2D screen position and updates the buffer based on the cube's rotation.
    main: Contains the animation loop, rendering multiple cubes with different sizes and positions.

Cross-Platform Sleep Handling

The program uses usleep for Unix-like systems and a custom implementation for Windows to handle frame delay.
Notes

    Make sure your terminal supports a screen size of at least 160x44 characters for optimal viewing.
    The animation may not appear smooth on all systems due to terminal rendering limitations.