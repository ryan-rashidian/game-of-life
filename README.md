# Game of Life

Simulate emergent complexity with simple rules in a 2d sandbox grid.

Based on the classic "Conway's Game of Life," where cells on the world grid are born or survive based on the status of neighboring cells. With 8 total neighbors per cell (with the exception of cells on the grid edge), rules for the game are represented simply by how many living neighbors each cell has. The rules can be subdivided into into 2 sets of numbers between 0 and 8, where one set represents conditions for "birth" (How many neighbors are needed for a cell to be born) and "survival" (How many neighbors are needed to survive to the next generation).

Made in C using the [raylib](https://www.raylib.com/) library.

## Features

- Configurable rules and presets

