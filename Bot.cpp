#include "Bot.h"
#include <random>
#include <vector> 
const std::string Bot::BOT_NAME = "MyBot"; /* Edit this, escaped characters are forbidden */


Bot::Bot(Board* m_board, PlaySide m_currColor) : board(m_board), currColor(m_currColor) {};
Bot::Bot(
) { /* Initialize custom fields here */
  board = new Board();
}


void Bot::recordMove(Move* move, PlaySide sideToMove) {
    /* You might find it useful to also separately
     * record last move in another custom field */
    board->movePiece(move);
}

Move* Bot::calculateNextMove() {
  /* Play move for the side the engine is playing (Hint: Main.getEngineSide())
   * Make sure to record your move in custom structures before returning.
   *
   * Return move that you are willing to submit
   * Move is to be constructed via one of the factory methods declared in Move.h */
  //PlaySide engineSide = Main.getEngineSide();
  PlaySide engineSide = PlaySide::BLACK;
  std::vector<Move*> legalMovesRand;
  std::queue<Move*> legalMoves= generateLegalMoves();
  while(!legalMoves.empty()){
    Move* m = legalMoves.front();
    legalMovesRand.push_back(m);
    legalMoves.pop();
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(legalMovesRand.begin(), legalMovesRand.end(), g);
  for(Move *move : legalMovesRand){
      recordMove(move, engineSide);
      bool willKingBeInCheck = isKinginCheck();
      board->undoPiece(move);
      if(willKingBeInCheck)
        continue;
      recordMove(move, engineSide);
      return move;
  }
  return Move::resign();
}


std::string Bot::getBotName() { return Bot::BOT_NAME; }

std::queue<Move*> Bot::generateLegalMoves() {
  std::queue<Move*> moves;
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++){
      MyPiece* piesa = board->getPiece(i, j);
      //PlaySide engineSide = Main.getEngineSide();
      PlaySide engineSide = PlaySide::BLACK;
      if(piesa->getType() != EMPTY && piesa->getColor() == engineSide) {
         for(int x = 0; x < 8; x++)
          for(int y = 7; y >= 0; y--){
            std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
            std::string dest = std::string(1, (char)(x + 'a')) + std::string(1, (char)(y + '1'));
            Move *m = new Move(src, dest, Piece::EMPTY);
            if(piesa->isLegalMove(*board, *m)){
                moves.push(m);
            }
            else delete m;
          }
      }
    }
  std::queue<Move*> cpyMoves = moves;
  while(!cpyMoves.empty()){
    Move *m = cpyMoves.front();
    cpyMoves.pop();
    string src = m->getSource().value();
    string dest = m->getDestination().value();
    int src_lit = src[0] - 'a';
    int src_num = src[1] - '1';
    std::cerr << "Buna! " << src << " " << dest << " " << board->getPiece(src_num, src_lit)->getType() << "\n";
  }
  return moves;
}

bool Bot::isKinginCheck(){
  int king_num = -1, king_col = -1;
  PlaySide engineSide = PlaySide::BLACK;
  // PlaySide engineSide = Main.getEngineSide();
    for(int i = 0; i < 8; i++)
      for(int j= 0; j < 8; j++)
        if(board->getPiece(i, j)->getType() == Piece::KING && board->getPiece(i,j)->getColor() == engineSide){
            king_num = i;
            king_col = j;
            break;
        }
    PlaySide opponentSide = (engineSide == PlaySide::WHITE) ? PlaySide::BLACK : PlaySide::WHITE;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        MyPiece* piesa = board->getPiece(i, j);
        if (piesa->getType() != Piece::EMPTY && piesa->getColor() == opponentSide) {
          std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
          std::string dest = std::string(1, (char)(king_col + 'a')) + std::string(1, (char)(king_num + '1'));
          Move move(src, dest, Piece::EMPTY);
          if (piesa->isLegalMove(*board, move)) {
                std::cerr << piesa->getType() << " " << piesa->getColor() << " might capture king \n";
                return true;
              }
          }
      }
  }
  return false;
}



