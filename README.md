# Rubik's Cube Solver

A C++ implementation and benchmark suite for solving a 3x3 Rubik's Cube using several classic search algorithms — DFS, BFS, Iterative Deepening DFS (IDDFS), and IDA* with pattern database heuristics.

## Overview

This project explores different search strategies for solving a scrambled Rubik's Cube, comparing their runtime and effectiveness at increasing scramble depths. The cube can be represented internally in multiple ways (e.g. bitboard), with `RubiksCubeBitboard` used as the primary representation in the benchmark driver.

The `main.cpp` driver:
- Generates a "clean" scramble (never repeating the same face twice in a row, so moves can't trivially cancel out)
- Runs each solver against a scramble of a configurable depth
- Times each solve and prints the resulting move sequence
- Safely catches memory/runtime errors so one failed solver doesn't crash the whole benchmark

## Algorithms

| Algorithm | Description |
|---|---|
| **DFS** | Depth-first search up to a fixed depth |
| **BFS** | Breadth-first search (guarantees shortest solution, but memory-intensive) |
| **IDDFS** | Iterative deepening DFS — combines BFS's optimality with DFS's memory efficiency |
| **IDA\*** | Iterative deepening A* using a corner pattern database heuristic for a large search-space reduction |

## Project Structure

```
.
├── main.cpp                     # Benchmark driver / entry point
├── Solver/
│   ├── DFSSolver.h
│   ├── BFSSolver.h
│   ├── IDDFSSolver.h
│   └── IDAstarSolver.h
├── PatternDatabases/
│   └── CornerDBMaker.h          # Generates the corner pattern database used by IDA*
└── Databases/
    └── cornerDepth5V1.txt       # Precomputed corner pattern database
```

## Requirements

- A C++17 (or later) compatible compiler
- CMake 3.x+

## Building

```bash
git clone <repo-url>
cd Rubiks_Cube_Solver
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage

Before running, make sure the pattern database path in `main.cpp` (the `fileName` variable) points to a valid corner pattern database file on your system:

```cpp
string fileName = "path/to/cornerDepth5V1.txt";
```

If the database file doesn't exist yet, generate it using `PatternDatabases/CornerDBMaker.h` first.

Then run the benchmark:

```bash
./RubiksCubeSolver
```

This will scramble a fresh cube for each algorithm, solve it, and print the shuffle moves, solution moves, and time taken for each of DFS, BFS, IDDFS, and IDA*.

### Example output

```
===================================================
 Rubik's Cube Solver - Algorithm Benchmark
===================================================

--- DFS (scramble depth 5) ---
Shuffle moves: L U R' F D2 
DFS solve moves: D2 F' R U' L' 
DFS solve time: 12.34 ms

...
```

## Configuration

Scramble depths for each algorithm can be adjusted at the top of `main()`:

```cpp
int dfsDepth = 5;
int bfsDepth = 4;
int iddfsDepth = 5;
int idaDepth = 8;
```

Note that DFS, BFS, and IDDFS scale poorly with depth due to the size of the search space, while IDA* — thanks to its pattern database heuristic — can handle much deeper scrambles efficiently.

## Notes

- BFS and uninformed DFS/IDDFS are only practical for shallow scrambles (state space grows exponentially with depth).
- IDA* is the most practical solver for real-world scrambles, relying on a precomputed corner pattern database to guide the search.

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.
