# Game of Life

## Student Information
- **Name:** Petar Dobrev
- **Faculty ID:** 8MI0600344
- **Class:** Software Engineering
- **Semester:** 1

## Project Overview
This project implements John Conway's Game of Life, a cellular automaton that simulates life on an infinite two-dimensional grid. Each cell on the grid can be either "alive" (@) or "dead" (-), with its state changing according to its neighbors based on set rules.

### Rules of the Game
- A live cell with fewer than two live neighbors dies.
- A live cell with two or three live neighbors stays alive.
- A live cell with more than three live neighbors dies.
- A dead cell with exactly three live neighbors becomes alive.

### Simulation of the game:
![ezgif com-video-to-gif (1)](https://github.com/peterdobrev/Game-of-Life---UP-FMI-Project/assets/78638859/a750d9e8-bf07-4c91-a526-2d17e758ee83)
<br>
Video shows randomizing the grid and then simulating life over 100 steps.


The game is played on an initially m×n grid, with the possibility of automatic expansion if necessary.

### Features
- Start a new game with an empty playing field of size 16x8.
- Load game state from a file.
- Interactive menu to step forward in the game, resize the grid, toggle cell state, clear the grid, randomize the grid, save the game, or end the current game.
- Automatic grid expansion if a cell outside the current grid boundaries needs to be revived.
- Autoplay to simulate the game so that the user can just sit back and watch.




