# Pong

Implementation of the game *Pong* using the `ncurses` library.

## Overview

The game can be played with either one or two players. If one player is selected, the game is played against a wall (i.e., since winning is impossible, the goal is to last as long as you can). The paddle is controlled with the W and S keys to move it up and down, respectively. In two-player mode, two players compete against each other. Player 2 is controlled with the I and K keys for up and down.

In both modes, the first player to 10 points wins.

## Requirements

* `ncurses`
* GNU Make
* GCC supporting C11 (`glibc`) with Linux headers
* Ctags (optional)

## Compilation

In the project root, run `make`. The executable will be placed in `./build/`.

To compile the prototypes, run `make proto`. The executables will be placed in `./build/proto/`.

## Project Structure

```
.
├── build           -- output for all compilation
│   ├── proto       -- output for prototype executables
│   └── src         -- output for object files
│
├── include         -- project header files
│   └── physics     -- header files relating to the game's physics
│
├── proto           -- early prototypes for various aspects of the game
│
└── src             -- project source files
    └── physics     -- source files relating to the game's physics
```

For more information about each file in the project, view the corresponding header file in `include/` for a brief description.

Note that most functions are prefixed with the translation unit they're from (e.g., `board_init()` is in `board.h`).
