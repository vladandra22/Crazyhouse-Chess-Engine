#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "PlaySide.h"
#include "Move.h"
#include <string>
#include <vector>

using namespace std;

extern enum PlaySide getEngineSide();

class Board; 

// Clasa abstracta myPieces
class MyPiece {
public:
    MyPiece(Piece m_piesa, PlaySide m_culoare);
    virtual ~MyPiece();
    Piece getType ();
    PlaySide getColor();
    virtual bool isLegalMove(const Board& board, Move m) = 0;
    Piece piesa;
    PlaySide culoare;
};

class Pawn : public MyPiece{
public: 
    Pawn(PlaySide culoare);
    bool isLegalMove(const Board& board, Move m) override;
};

class Rook : public MyPiece{
public:
    Rook(PlaySide culoare);
    bool isLegalMove(const Board& board, Move m) override;
};

class Bishop : public MyPiece{
public:
    Bishop(PlaySide culoare);
    bool isLegalMove(const Board& board, Move m) override;
};

class Knight : public MyPiece{
public:
    Knight(PlaySide culoare);
    bool isLegalMove(const Board& board, Move m) override;
};

class Queen : public MyPiece{
public:
    Queen(PlaySide culoare);
    bool isLegalMove(const Board& board, Move m) override;
};

class King : public MyPiece{
public:
    King(PlaySide culoare);
    bool isLegalMove(const Board& board, Move m) override;
};

class EmptySquare : public MyPiece{
    public:
    EmptySquare();
    bool isLegalMove(const Board& board, Move m) override;
};

class Board {
public:
    MyPiece* board[8][8];
    vector<MyPiece*> isCapturedWhite;
    vector<MyPiece*> isCapturedBlack;
    Board();
    ~Board();
    MyPiece* getPiece(int lin, int col) const;
    void movePiece(Move *m);
    void undoPiece(Move *m);
};

#endif