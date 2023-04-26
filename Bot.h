#ifndef BOT_H
#define BOT_H

#include <string>
#include <queue>

#include "ChessBoard.h"
#include "Move.h"
#include "PlaySide.h" 

extern enum PlaySide getEngineSide();
class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */
    Board* board = nullptr;
    PlaySide currColor;
  /* Declare custom fields above */
  Bot();
  Bot(Board* m_board, PlaySide m_currColor);

  /**
   * Record move (either by enemy in normal mode, or by either side
   * in force mode) in custom structures
   * @param move received move
   * @param sideToMode side to move
   */
  void recordMove(Move* move, PlaySide sideToMove);

  /**
   * Calculates next move, in response to enemyMove
   * @param enemyMove the enemy's last move
   *                  null if this is the opening move, or previous
   *                  move has been recorded in force mode
   * @return your move
   */
  Move* calculateNextMove();

  static std::string getBotName();
  std::queue<Move*> generateLegalMoves(PlaySide engineSide);
  bool isKinginCheck(PlaySide engineSide);
};
#endif
