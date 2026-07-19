#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Solver/DFSSolver.h"
#include "Solver/BFSSolver.h"
#include "Solver/IDDFSSolver.h"
#include "Solver/IDAstarSolver.h"
#include "PatternDatabases/CornerDBMaker.h"

using namespace std;

string fileName = "E:\\Downloads\\Rubiks_Cube_Solver-main\\Rubiks_Cube_Solver-main\\Databases\\cornerDepth5V1.txt";

// Generates a scramble that never repeats the same face twice in a row,
// so moves like L followed by L' can't cancel each other out.
// Faces are ordered L, R, U, D, F, B (matches the MOVE enum grouping of 3 each).
vector<RubiksCube::MOVE> generateCleanScramble(RubiksCubeBitboard &cube, int length) {
    vector<RubiksCube::MOVE> moves;
    int lastFace = -1;

    for (int i = 0; i < length; i++) {
        int face;
        do {
            face = rand() % 6;
        } while (face == lastFace);

        int variant = rand() % 3; // 0 = normal, 1 = prime, 2 = double
        int moveIndex = face * 3 + variant;

        RubiksCube::MOVE move = RubiksCube::MOVE(moveIndex);
        cube.move(move);
        moves.push_back(move);
        lastFace = face;
    }
    return moves;
}

// Runs a solver safely, catching bad_alloc / other errors instead of crashing.
// Times in milliseconds using steady_clock for better precision on fast solves.
template <typename SolverFunc>
void runSafely(const string &label, SolverFunc solverFunc) {
    try {
        auto start = chrono::steady_clock::now();
        solverFunc();
        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration<double, milli>(end - start);
        cout << label << " solve time: " << elapsed.count() << " ms\n\n";
    }
    catch (const bad_alloc &) {
        cout << label << " FAILED: ran out of memory (std::bad_alloc) at this scramble depth.\n\n";
    }
    catch (const exception &e) {
        cout << label << " FAILED with error: " << e.what() << "\n\n";
    }
    catch (...) {
        cout << label << " FAILED with an unknown error.\n\n";
    }
}

int main() {
    srand(time(0));

    int dfsDepth = 5;
    int bfsDepth = 4;
    int iddfsDepth = 5;
    int idaDepth = 8;

    cout << "===================================================\n";
    cout << " Rubik's Cube Solver - Algorithm Benchmark\n";
    cout << "===================================================\n\n";

    // ---------------- DFS ----------------
    {
        RubiksCubeBitboard cube;
        auto shuffleMoves = generateCleanScramble(cube, dfsDepth);
        cout << "--- DFS (scramble depth " << dfsDepth << ") ---\n";
        cout << "Shuffle moves: ";
        for (auto move : shuffleMoves) cout << cube.getMove(move) << " ";
        cout << "\n";

        runSafely("DFS", [&]() {
            DFSSolver<RubiksCubeBitboard, HashBitboard> dfsSolver(cube, dfsDepth);
            auto moves = dfsSolver.solve();
            cout << "DFS solve moves: ";
            for (auto move : moves) cout << cube.getMove(move) << " ";
            cout << "\n";
        });
    }

    // ---------------- BFS ----------------
    {
        RubiksCubeBitboard cube;
        auto shuffleMoves = generateCleanScramble(cube, bfsDepth);
        cout << "--- BFS (scramble depth " << bfsDepth << ") ---\n";
        cout << "Shuffle moves: ";
        for (auto move : shuffleMoves) cout << cube.getMove(move) << " ";
        cout << "\n";

        runSafely("BFS", [&]() {
            BFSSolver<RubiksCubeBitboard, HashBitboard> bfsSolver(cube);
            auto moves = bfsSolver.solve();
            cout << "BFS solve moves: ";
            for (auto move : moves) cout << cube.getMove(move) << " ";
            cout << "\n";
        });
    }

    // ---------------- IDDFS ----------------
    {
        RubiksCubeBitboard cube;
        auto shuffleMoves = generateCleanScramble(cube, iddfsDepth);
        cout << "--- IDDFS (scramble depth " << iddfsDepth << ") ---\n";
        cout << "Shuffle moves: ";
        for (auto move : shuffleMoves) cout << cube.getMove(move) << " ";
        cout << "\n";

        runSafely("IDDFS", [&]() {
            IDDFSSolver<RubiksCubeBitboard, HashBitboard> iddfsSolver(cube, iddfsDepth);
            auto moves = iddfsSolver.solve();
            cout << "IDDFS solve moves: ";
            for (auto move : moves) cout << cube.getMove(move) << " ";
            cout << "\n";
        });
    }

    // ---------------- IDA* ----------------
    {
        RubiksCubeBitboard cube;
        auto shuffleMoves = generateCleanScramble(cube, idaDepth);
        cout << "--- IDA* (scramble depth " << idaDepth << ") ---\n";
        cout << "Shuffle moves: ";
        for (auto move : shuffleMoves) cout << cube.getMove(move) << " ";
        cout << "\n";

        runSafely("IDA*", [&]() {
            IDAstarSolver<RubiksCubeBitboard, HashBitboard> idaStarSolver(cube, fileName);
            auto moves = idaStarSolver.solve();
            cout << "IDA* solve moves: ";
            for (auto move : moves) cout << cube.getMove(move) << " ";
            cout << "\n";
        });
    }

    cout << "===================================================\n";
    cout << " Benchmark complete\n";
    cout << "===================================================\n";

    return 0;
}
