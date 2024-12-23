# Chess Engine

**Overview**

This is a simple C++ chess engine that utilizes 64-bit bitboards for fast and efficient move generation. 
It incorporates essential move calculation algorithms, including Hyperbola Quintessence for sliding pieces (covering files, diagonals, and antidiagonals), a first-rank attacks algorithm for ranks, Dumb7Fill for pawn pushes, and standard algorithms for non-sliding pieces like kings, knights, and pawn attacks. The engine also employs a LookupTable of precalculated bitboards to optimize tasks like move calculation and features a move encoding format that streamlines move generation and execution methods.
The design follows object-oriented programming (OOP) principles for clear structure and maintainability, and supports universal chess interface (UCI) for move input.

The goal of this project is to design and train a simple NNUE neural network that uses the implemented engine for 
its training, this part is currently beign developed.

**Important**

Perf_t function wich gives number of moves for a given depth does not output the correct number of moves. This means that the make move or generate moves functions have bugs.

**Installation**

```bash
git clone https://github.com/santacg/Chess.git
```
**Usage**

Compiling and executing.
```bash
make
./chess
```
The engine will display a chessboard with ASCII pieces and prompt for a UCI move input, as shown below.
```bash
[santacg@archlinux Chess]$ ./chess 

8 ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖ 
7 ♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙ 
6 . . . . . . . . 
5 . . . . . . . . 
4 . . . . . . . . 
3 . . . . . . . . 
2 ♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟ 
1 ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜ 
  a b c d e f g h 

Side to move: white

Castling rights: KQkq

En passant square: -

Insert UCI format string (exit to end): e2e4

8 ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖ 
7 ♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙ 
6 . . . . . . . . 
5 . . . . . . . . 
4 . . . . ♟ . . . 
3 . . . . . . . . 
2 ♟ ♟ ♟ ♟ . ♟ ♟ ♟ 
1 ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜ 
  a b c d e f g h 

Side to move: black

Castling rights: KQkq

En passant square: e3

Insert UCI format string (exit to end): e7e5

8 ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖ 
7 ♙ ♙ ♙ ♙ . ♙ ♙ ♙ 
6 . . . . . . . . 
5 . . . . ♙ . . . 
4 . . . . ♟ . . . 
3 . . . . . . . . 
2 ♟ ♟ ♟ ♟ . ♟ ♟ ♟ 
1 ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜ 
  a b c d e f g h 

Side to move: white

Castling rights: KQkq

En passant square: e6
```
