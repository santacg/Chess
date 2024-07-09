#include "square.h"

using namespace std;

Square::Square() {}

Square::Square(int x, int y, Color c, Piece *p) {
  coordinateX = x;
  coordinateY = y;
  color = c;
  piece = p;
}

int Square::getFileIndex() { return coordinateX; }

void Square::setFileIndex(int x) { coordinateX = x; }

string Square::getFile() {
  switch (coordinateX) {
  case 0:
    return "a";
  case 1:
    return "b";
  case 2:
    return "c";
  case 3:
    return "d";
  case 4:
    return "e";
  case 5:
    return "f";
  case 6:
    return "g";
  case 7:
    return "h";
  default:
    break;
  }
  return "";
}

int Square::getRankIndex() { return coordinateY; }

int Square::getRank() { return coordinateY + 1; }

void Square::setRank(int y) { coordinateY = y; }

Color Square::getColor() { return color; }

void Square::setColor(Color c) { color = c; }

Piece *Square::getPiece() { return piece; }

void Square::setPiece(Piece *p) { piece = p; }
