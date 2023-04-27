#ifndef BOT_H
#define BOT_H
#include <string>
#include <queue>

#include "Move.h"
#include "PlaySide.h"

struct ChessPiece {
    Piece piesa;
    PlaySide culoare;
};


class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */
    ChessPiece board[8][8];
  /* Declare custom fields above */
  Bot();

  /**
   * Record move (either by enemy in normal mode, or by either side
   * in force mode) in custom structures
   * @param move received move
   * @param sideToMode side to move
   */
  void recordMove(Move* move, PlaySide sideToMove);

  /**
   * Calculates and return the bot's next move
   * @return your move
   */
  Move* calculateNextMove();

  static std::string getBotName();
  void generateChessBoard(ChessPiece board[8][8]);
  std::queue<Move*> generateLegalMoves(PlaySide engineSide);
  bool isLegalMove(ChessPiece board[8][8], Move m);
  bool isKinginCheck(ChessPiece cpyBoard[8][8], PlaySide engineSide);
};
#endif
