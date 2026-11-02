# Game of Life

Cellular automaton simulation based on the classic "Conway's Game of Life," where cells on the world grid are born or survive based on the status of neighboring cells. With 8 total neighbors per cell (with the exception of cells on the grid edge), rules for the game are represented simply by how many living neighbors each cell has. The rules can be subdivided into into 2 sets of numbers from 0 to 8, where one set represents conditions for "birth" (How many neighbors are needed for a cell to be born), and the other represents conditions for "survival" (How many neighbors are needed to survive to the next generation).

Made in C using the [raylib](https://www.raylib.com/) library, and 100% human written slop.

## To-Do

- Optimization: Write grid-cell colors to pixel buffer, then update and draw GPU texture once per frame.
- UI: Buttons and text are difficult to see at times.

