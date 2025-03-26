# Tetris Game

## Table of Contents

- [Game Overview](#game-overview)
- [How to Play](#how-to-play)
- [Controls](#controls)
- [Game Rules](#game-rules)
- [Game Logic](#game-logic)
- [Data Structure Analysis](#data-structure-analysis)
- [OS Compatibility](#os-compatibility)
- [Contributing](#contributing)

## Game Overview

This is a **console-based Tetris game** written in **C++**, utilizing **Windows API** for cursor manipulation and colored output. The game features real-time user input handling, shape rotations, line clearing, and level progression.

### Features

- **Classic Tetris Mechanics**: Move, rotate, and drop tetrominoes to clear lines.
- **Real-Time Input Handling**: Uses `_kbhit()` and `_getch()` for smooth control.
- **Scoring System**: Earn points based on the number of lines cleared at once.
- **Leveling System**: Increase level and speed after clearing a set number of lines.
- **Pause and Resume**: Press 'P' to pause or resume the game.
- **Game Over Detection**: The game ends when no more pieces can be placed.
- **Colorful Display**: Uses `SetConsoleTextAttribute()` for block colors.

### Prerequisites

- **Windows OS** (Game uses Windows-specific functions)
- **C++ Compiler (MinGW/GCC or MSVC)**

### Compilation & Execution

1. Clone the repository:

   ```bash
   git clone https://github.com/Yash-P88/IT206-P1-TetrisGame.git
   ```

2. Navigate into the project folder:

   ```bash
   cd IT206-P1-TetrisGame
   ```

3. Compile the code using a C++ compiler:

   ```bash
   g++ Tetris.cpp -o tetris.exe
   ```

4. Run the game:

   ```bash
   ./tetris.exe
   ```

## How to Play

1. Once you start the game, tetrominoes will begin falling from the top.
2. Move and rotate the pieces to form complete horizontal lines.
3. Completed lines will disappear, granting points.
4. The game speeds up as you progress, increasing the challenge.
5. The game ends when new pieces can no longer be placed.

## Controls

| Key         | Action       |
| ----------- | ------------ |
| `←` (Left)  | Move left    |
| `→` (Right) | Move right   |
| `↑` (Up)    | Rotate piece |
| `↓` (Down)  | Soft drop    |
| `SPACE`     | Hard drop    |
| `P`         | Pause/Resume |
| `ESC`       | Quit game    |

## Game Rules

- The game board consists of a **10x20 grid**.
- Tetrominoes spawn at the top and fall down automatically.
- The goal is to **clear lines** by filling rows with blocks.
- The game ends when the **stack reaches the top** of the board.

## Game Logic

- **Tetromino Class**: Represents different Tetris pieces and their rotations.
- **Board Class**: Manages the grid, line clearing, and collision detection.
- **Game Controller**: Handles input, scoring, and level progression.

## Data Structure Analysis

### 1. **2D Array for the Game Board**

- Stores the current state of the grid (empty cells and placed blocks).
- Used for checking collisions and clearing full rows.

### 2. **Tetromino Representation**

- Each tetromino is stored as a **2D matrix**.
- Rotations are implemented by transforming this matrix.

### 3. **Queue for Upcoming Pieces**

- Implements a **Next Piece Preview** feature.
- Helps in planning moves ahead.

## OS Compatibility

### Windows

This game uses the `windows.h` header for:

- `Sleep()` for controlling the game speed.
- `SetConsoleTextAttribute()` for colored output.
- `system("cls")` for clearing the console.
- `_kbhit()` and `_getch()` for real-time input handling.

### Linux/macOS Compatibility

- **To run on Linux/macOS**, modifications are needed:
  - Replace `windows.h` functions with **ncurses**.
  - Use `usleep()` instead of `Sleep()`.
  - Replace `system("cls")` with `system("clear")`.
  - Implement `termios` for real-time key detection.

## Contributing

Contributions are welcome! Whether it's bug fixes, optimizations, or adding new features, your help is appreciated.

### How to Contribute

1. **Fork the repository** on GitHub.
2. **Create a new branch**:

   ```bash
   git checkout -b feature-name
   ```

3. **Make your changes** and test the game.
4. **Push your branch**:

   ```bash
   git push origin feature-name
   ```

5. **Create a pull request** to merge your changes.

Enjoy the game! Have fun clearing lines and setting high scores!
