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

  // Jucator curent
  PlaySide engineSide = getEngineSide();

  // Generam toate miscarile intr-o coada si dupa aceea le amestecam.
  std::vector<Move*> legalMovesRand;
  std::queue<Move*> legalMoves = generateLegalMoves(engineSide);
  while (!legalMoves.empty()) {
    Move* m = legalMoves.front();
    legalMovesRand.push_back(m);
    legalMoves.pop();
  }

  // Shuffle mutari
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(legalMovesRand.begin(), legalMovesRand.end(), g);

  // Rationament: facem mutarea, verificam daca regele este in sah si ii dam undo.
  // Daca regele e in sah din cauza acelei mutari, continuam sa generam altele.
  // Altfel, facem pe bune mutarea si o si returnam.
  for (Move* move : legalMovesRand) {
    // std::cerr<< "Buna! " <<move->getSource().value() << " " << move->getDestination().value() << "\n";
    recordMove(move, engineSide);
    bool willKingBeInCheck = isKinginCheck(engineSide);
    board->undoPiece(move);
    if (willKingBeInCheck)
      continue;
    recordMove(move, engineSide);
    return move;
  }
  // Daca nu gaseste nicio mutare buna, jucatorul curent da resign.
  return Move::resign();
}



std::string Bot::getBotName() { return Bot::BOT_NAME; }

// Generarea miscarilor legale
std::queue<Move*> Bot::generateLegalMoves(PlaySide engineSide) {
  std::queue<Move*> moves;
  // Luam pe rand fiecare patrat
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++){
      // Extragem piesa de pe acel patrat
      MyPiece* piesa = board->getPiece(i, j);
      // Daca piesa curenta este a jucatorului curent, generam 
      // toate miscarile disponibile pentru aceasta
      if(piesa->getColor() == engineSide) {
         for(int x = 0; x < 8; x++)
          for(int y = 0; y < 8; y++){
            // Generare mutare
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
  
  // Cod auxiliar pentru afisare mutari facute
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

bool Bot::isKinginCheck(PlaySide engineSide){
  // Cautare rege
  int king_num = -1, king_col = -1;
    for(int i = 0; i < 8; i++)
      for(int j= 0; j < 8; j++)
        if(board->getPiece(i, j)->getType() == Piece::KING && board->getPiece(i,j)->getColor() == engineSide){
            king_num = i;
            king_col = j;
            break;
        }
    
    // Culoare oponent
    PlaySide opponentSide = (engineSide == PlaySide::WHITE) ? PlaySide::BLACK : PlaySide::WHITE;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        MyPiece* piesa = board->getPiece(i, j);
        // Cautam piese de culoarea oponentului
        if (piesa->getColor() == opponentSide) {
          // Extragem mutari tip piesa_oponent->rege
          std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
          std::string dest = std::string(1, (char)(king_col + 'a')) + std::string(1, (char)(king_num + '1'));
          // std::cerr << "miscare " << src << " " << dest << " " << opponentSide << "\n";
          Move move(src, dest, Piece::EMPTY);
          if (piesa->isLegalMove(*board, move)) {
                // std::cerr << piesa->getType() << " " << piesa->getColor() << " might capture king \n";
                return true;
              }
          }
      }
  } 
  // Daca nicio piesa a oponentului nu genereaza sah, atunci returnam fals.
  return false;
}



