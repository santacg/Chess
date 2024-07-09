#ifndef SQUARE_H
#define SQUARE_H

#include "piece.h"
#include "enums.h"
#include <string>

using namespace std;

class Square {
private:
  int coordinateX;
  int coordinateY;
  Piece *piece;
  Color color;

public:

  Square();

  Square(int x, int y, Color c, Piece *p);

  int getFileIndex();

  void setFileIndex(int x);

  string getFile();

  int getRankIndex();
  
  int getRank();

  void setRank(int y);

  Color getColor();

  void setColor(Color c); 

  Piece *getPiece();

  void setPiece(Piece *p);
};

#endif
