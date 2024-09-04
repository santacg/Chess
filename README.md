# Chess Engine
**Overview**
This is a simple C++ chess engine that utilizes 64-bit bitboards for fast and efficient move generation. 
It incorporates essential move calculation algorithms, including Hyperbola Quintessence for sliding pieces (covering files, diagonals, and antidiagonals), a first-rank attacks algorithm for ranks, Dumb7Fill for pawn pushes, and standard algorithms for non-sliding pieces like kings, knights, and pawn attacks. The engine also employs a LookupTable of precalculated bitboards to optimize tasks like move calculation and features a move encoding format that streamlines move generation and execution methods.
The design follows object-oriented programming (OOP) principles for clear structure and maintainability, and supports universal chess interface (UCI) for move input.

The goal of this project is to design and train a simple NNUE neural network that uses the implemented engine for 
its training, this part is currently beign developed.

**Installation**
```bash
git clone https://github.com/santacg/Chess.git
cd src

**Usage**
```bash
make
./chess
The engine will display a chessboard with ASCII pieces and prompt for a UCI move input.
