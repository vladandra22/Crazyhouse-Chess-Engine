#include "Bot.h"
#include <random>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cstring>

const std::string Bot::BOT_NAME = "MyBot"; /* Edit this, escaped characters are forbidden */
extern PlaySide getEngineSide();


Bot::Bot() { /* Initialize custom fields here */
  generateChessBoard(board);
}

void Bot::generateChessBoard(ChessPiece board[8][8]) {
    // Initialize black pieces
    board[0][0] = {Piece::ROOK, PlaySide::WHITE, false, 0};
    board[0][1] = {Piece::KNIGHT, PlaySide::WHITE, false, 0};
    board[0][2] = {Piece::BISHOP, PlaySide::WHITE, false, 0};
    board[0][3] = {Piece::QUEEN, PlaySide::WHITE, false, 0};
    board[0][4] = {Piece::KING, PlaySide::WHITE, false, 0};
    board[0][5] = {Piece::BISHOP, PlaySide::WHITE, false, 0};
    board[0][6] = {Piece::KNIGHT, PlaySide::WHITE, false, 0};
    board[0][7] = {Piece::ROOK, PlaySide::WHITE, false, 0};
    for (int i = 0; i < 8; i++) {
        board[1][i] = {Piece::PAWN, PlaySide::WHITE, false, 0};
    }
    // Initialize empty squares
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = {Piece::EMPTY, PlaySide::NONE, false, 0};
        }
    }
    // Initialize white pieces
    for (int i = 0; i < 8; i++) {
        board[6][i] = {Piece::PAWN, PlaySide::BLACK, false, 0};
    }
    board[7][0] = {Piece::ROOK, PlaySide::BLACK, false, 0};
    board[7][1] = {Piece::KNIGHT, PlaySide::BLACK, false, 0};
    board[7][2] = {Piece::BISHOP, PlaySide::BLACK, false, 0};
    board[7][3] = {Piece::QUEEN, PlaySide::BLACK, false, 0};
    board[7][4] = {Piece::KING, PlaySide::BLACK, false, 0};
    board[7][5] = {Piece::BISHOP, PlaySide::BLACK, false, 0};
    board[7][6] = {Piece::KNIGHT, PlaySide::BLACK, false, 0};
    board[7][7] = {Piece::ROOK, PlaySide::BLACK, false, 0};
}

// Functie care imi face miscarea pe tabla.
void Bot::recordMove(Move* move, PlaySide sideToMove) {
    /* You might find it useful to also separately
     * record last move in another custom field */

    //e1g1, e1c1, e8g8, e8g8
    // Caz 1: Drop in (Crazyhouse)
    if(move->isDropIn()){
      last_src = "zz";
      last_dest = move->getDestination().value();
      std::string dest = move->getDestination().value();
      int dest_lit = dest[0] - 'a';
      int dest_num = dest[1] - '1';
      Piece replacement1 = move->getReplacement().value();
      // Daca realizez o miscare de drop-in, scad nr de piese de acel tip
      if(sideToMove == PlaySide::WHITE){
        isCapturedWhite[replacement1]--;
      }
      else 
        isCapturedBlack[replacement1]--;
      board[dest_num][dest_lit].culoare = sideToMove;
      board[dest_num][dest_lit].piesa = replacement1;
      board[dest_num][dest_lit].isPromotion = false;
      Move::dropIn(dest, replacement1);
    }
    // Caz 2: Miscare normala
    else if(move->isNormal()){
      last_src = move->getSource().value();
      last_dest = move->getDestination().value();
      if(move->getSource().value() == "e1" && move->getDestination().value() == "g1"){
        if(board[0][4].piesa == Piece::KING){
          board[0][5] = board[0][7];
          board[0][7].piesa = Piece::EMPTY;
          board[0][7].culoare = PlaySide::NONE;
        }
      }
      else if(move->getSource().value() == "e1" && move->getDestination().value() == "c1"){
        if(board[0][4].piesa == Piece::KING){
            board[0][3] = board[0][0];
            board[0][0].piesa = Piece::EMPTY;
            board[0][0].culoare = PlaySide::NONE;
        }
      }
      else if(move->getSource().value() == "e8" && move->getDestination().value() == "g8"){
        if(board[7][4].piesa == Piece::KING){
          board[7][5] = board[7][7];
          board[7][7].piesa = Piece::EMPTY;
          board[7][7].culoare = PlaySide::NONE;
        }
      }
      else if(move->getSource().value() == "e8" && move->getDestination().value() == "c8"){
        if(board[7][4].piesa == Piece::KING){
          board[7][3] = board[7][0];
          board[7][0].piesa = Piece::EMPTY;
          board[7][0].culoare = PlaySide::NONE;
        }
      }    
      std::string src = move->getSource().value();
      std::string dest = move->getDestination().value();
      int src_lit = src[0] - 'a';
      int src_num = src[1] - '1';
      int dest_lit = dest[0] - 'a';
      int dest_num = dest[1] - '1';
      ChessPiece piece = board[src_num][src_lit];
      // Stergere pion daca am mutare En Passant
      if(board[dest_num][dest_lit].culoare == PlaySide::NONE && board[src_num][src_lit].piesa == Piece::PAWN && src_num != dest_num && src_lit != dest_lit){
          PlaySide side = board[src_lit][src_num].culoare;
          int direction = (side == PlaySide::WHITE) ? -1 : 1;
          board[dest_num][dest_lit] = piece;
          board[src_num][src_lit].piesa = Piece::EMPTY;
          board[src_num][src_lit].culoare = PlaySide::NONE;
          board[dest_num - direction][dest_lit].piesa = Piece::EMPTY;
          board[dest_num - direction][dest_lit].culoare = PlaySide::NONE;
          board[dest_num - direction][dest_lit].isPromotion = false;
          if(side == PlaySide::WHITE){
            isCapturedBlack[Piece::PAWN]++;
          }
          else if(side == PlaySide::BLACK){
            isCapturedWhite[Piece::PAWN]++;
          }
          Move::moveTo(move->getSource(), move->getDestination());
          return;
      }
      // Daca e captura, adaug la vectorul de frecventa.
      if(board[dest_num][dest_lit].culoare == PlaySide::BLACK){
        if(board[dest_num][dest_lit].isPromotion == true){
          isCapturedWhite[Piece::PAWN]++;
        }
        else {
          int type = board[dest_num][dest_lit].piesa;
          isCapturedWhite[type]++;
        }
      }
      if(board[dest_num][dest_lit].culoare == PlaySide::WHITE){
         if(board[dest_num][dest_lit].isPromotion == true){
          isCapturedBlack[Piece::PAWN]++;
        }
        else {
          int type = board[dest_num][dest_lit].piesa;
          isCapturedBlack[type]++;
        }
      }
      // Golesc sursa
      board[src_num][src_lit].culoare = PlaySide::NONE;
      board[src_num][src_lit].piesa = Piece::EMPTY;
      board[src_num][src_lit].isPromotion = false;
      board[src_num][src_lit].moves_count = 0;
      board[dest_num][dest_lit] = piece;
      board[dest_num][dest_lit].moves_count++;
      Move::moveTo(move->getSource(), move->getDestination());
      for(int i = 7; i >= 0; i--) {       
        for(int j = 0; j < 8; j++) {          
           std::cerr << i << " " << j << " " << board[i][j].piesa << " " << board[i][j].culoare << "| ";        
           }
        std::cerr << "\n";     
        }
    }
    else if(move->isPromotion()){
      last_src = move->getSource().value();
      last_dest = move->getDestination().value();
      std::string src = move->getSource().value();
      std::string dest = move->getDestination().value();
      int src_lit = src[0] - 'a';
      int src_num = src[1] - '1';
      int dest_lit = dest[0] - 'a';
      int dest_num = dest[1] - '1';
      ChessPiece piece = board[src_num][src_lit];
      board[src_num][src_lit].culoare = PlaySide::NONE;
      board[src_num][src_lit].piesa = Piece::EMPTY;
      board[dest_num][dest_lit] = piece;
      board[dest_num][dest_lit].piesa = Piece::QUEEN;
      board[dest_num][dest_lit].isPromotion = true;
      Move::promote(src, dest, Piece::QUEEN);
    }
}

// Functie identica cu recordMove, pe care o realizez pentru cpyBoard. 
// Diferenta este ca nu modific numarul de frecvente din isCapturedWhite/isCapturedBlack
// (acestea sunt pentru board-ul real)
void Bot::recordMove2(ChessPiece cpyBoard[8][8], Move* move, PlaySide sideToMove) {
    /* You might find it useful to also separately
     * record last move in another custom field */
    if(move->isDropIn()){
      std::string dest = move->getDestination().value();
      int dest_lit = dest[0] - 'a';
      int dest_num = dest[1] - '1';
      Piece replacement1 = move->getReplacement().value();
      cpyBoard[dest_num][dest_lit].culoare = sideToMove;
      cpyBoard[dest_num][dest_lit].piesa = replacement1;
      cpyBoard[dest_num][dest_lit].isPromotion = false;
    }
   else if(move->isNormal()){
      std::string src = move->getSource().value();
      std::string dest = move->getDestination().value();
      int src_lit = src[0] - 'a';
      int src_num = src[1] - '1';
      int dest_lit = dest[0] - 'a';
      int dest_num = dest[1] - '1';
      ChessPiece piece = cpyBoard[src_num][src_lit];
      // Golesc sursa
      cpyBoard[src_num][src_lit].culoare = PlaySide::NONE;
      cpyBoard[src_num][src_lit].piesa = Piece::EMPTY;
      cpyBoard[dest_num][dest_lit] = piece;
    }
    else if(move->isPromotion()){
      std::string src = move->getSource().value();
      std::string dest = move->getDestination().value();
      int src_lit = src[0] - 'a';
      int src_num = src[1] - '1';
      int dest_lit = dest[0] - 'a';
      int dest_num = dest[1] - '1';
      ChessPiece piece = cpyBoard[src_num][src_lit];
      cpyBoard[src_num][src_lit].culoare = PlaySide::NONE;
      cpyBoard[src_num][src_lit].piesa = Piece::EMPTY;
      cpyBoard[dest_num][dest_lit] = piece;
      cpyBoard[dest_num][dest_lit].piesa = Piece::QUEEN;
    }
}


Move* Bot::calculateNextMove() {
  /* Play move for the side the engine is playing (Hint: getEngineSide())
   * Make sure to record your move in custom structures before returning.
   *
   * Return move that you are willing to submit
   * Move is to be constructed via one of the factory methods declared in Move.h */
  PlaySide engineSide = getEngineSide();
  std::cerr << (engineSide == PlaySide::WHITE ? "ALB!!!!!!!" : "NEGRU!!!!!!") << "\n";

  int rocada = isCastling(engineSide);
  std::cerr << "\n\n\nROCADA MEA FRUMI " << rocada << "\n\n\n\n";
  // Rocada mica
  if(rocada == 0){
    int side = (engineSide == PlaySide::WHITE) ? 0 : 7;
    ChessPiece king = board[side][4];
    ChessPiece rook = board[side][7];

    board[side][4].culoare = PlaySide::NONE;
    board[side][4].piesa = Piece::EMPTY;
    board[side][4].isPromotion = false;
    board[side][4].moves_count = 0;
    board[side][7].culoare = PlaySide::NONE;
    board[side][7].piesa = Piece::EMPTY;
    board[side][4].isPromotion = false;
    board[side][7].moves_count = 0;

    board[side][6] = king;
    board[side][6].moves_count++;
    board[side][5] = rook;
    board[side][5].moves_count++;

    std::string src = std::string(1, (char)(4 + 'a')) + std::string(1, (char)(side + '1'));
    std::string dest = std::string(1, (char)(6 + 'a')) + std::string(1, (char)(side + '1'));
    Move* misc_king = new Move(src, dest, {});
    Move::moveTo(misc_king->getSource(), misc_king->getDestination());


    std::cerr << board[side][6].moves_count << " " << "rege" << engineSide <<"\n";
    std::cerr << board[side][5].moves_count << " " << "tura" << engineSide <<"\n";
    //recordMove(misc_king, engineSide);
    return misc_king;
  }
   // facem rocada mare
  else if (rocada == 1) {
    //std::cerr << "ROCADA MARE :D" << "\n";
    int side = (engineSide == PlaySide :: WHITE) ? 0 : 7;
    ChessPiece king =  board[side][4];
    ChessPiece rook = board[side][0];
    // eliberam patratelele turei si regelui
    board[side][4].culoare = PlaySide :: NONE;
    board[side][4].piesa = Piece :: EMPTY;
    board[side][0].culoare = PlaySide :: NONE;
    board[side][0].piesa = Piece :: EMPTY;
    // mutam regele si tura
    board[side][2] = king;
    board[side][2].moves_count++;
    board[side][3] = rook;
    board[side][3].moves_count++;

    std::string src = std::string(1, (char)(4 + 'a')) + std::string(1, (char)(side + '1'));
    std::string dest = std::string(1, (char)(2 + 'a')) + std::string(1, (char)(side + '1'));
    Move* misc_king = new Move(src, dest, {});
    Move::moveTo(misc_king->getSource(), misc_king->getDestination());

    // std::string src2 = std::string(1, (char)(0 + 'a')) + std::string(1, (char)(side + '1'));
    // std::string dest2 = std::string(1, (char)(3 + 'a')) + std::string(1, (char)(side + '1'));
    // Move* misc_rook = new Move(src2, dest2, {});
    // Move::moveTo(misc_rook->getSource(), misc_rook->getDestination());
    // recordMove(misc_rook, engineSide); //nu merge daca decomentam

    std::cerr << board[side][2].moves_count << " " << "rege"<< engineSide << "\n";
    std::cerr << board[side][3].moves_count << " " << "tura"<< engineSide << "\n";
    //recordMove(misc_king, engineSide);
    return misc_king;
  } 

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
    //recordMove(board, move, engineSide);
    //return move;
    ChessPiece cpyBoard[8][8];
    for(int i = 0; i < 8; i++)
      for(int j = 0; j < 8; j++)
        cpyBoard[i][j] = board[i][j];
    recordMove2(cpyBoard, move, engineSide);


    if(isKinginCheck(cpyBoard, engineSide))
      continue;
    recordMove(move, engineSide);
    // Afisare pentru crazyhouse

    return move;
  }
  return Move::resign();
}

std::queue<Move*> Bot::generateLegalMoves(PlaySide engineSide) {
  std::queue<Move*> moves;
  // Luam pe rand fiecare patrat
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++){
      // Extragem piesa de pe acel patrat
      // Daca piesa curenta este a jucatorului curent, generam 
      // toate miscarile disponibile pentru aceasta
      if(board[i][j].culoare == engineSide) {
          if(board[i][j].piesa == Piece::PAWN){
            // ADAUGA AICI GENERARE MISCARI PAWN PROMOTION
              PlaySide curr_color = board[i][j].culoare;
              PlaySide other_color = (curr_color == PlaySide::WHITE ? PlaySide::BLACK : PlaySide :: WHITE);
              int direction = (curr_color== PlaySide::WHITE) ? 1 : -1;
              if(i + direction == 7 || i + direction == 0){
                  if(board[i + direction][j].culoare == PlaySide::NONE){
                  std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
                  std::string dest = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + direction + '1'));
                  Move *m = new Move(src, dest, Piece::QUEEN);
                  moves.push(m);
                }
                else if( (j + 1 < 8) && board[i + direction][j + 1].culoare == other_color){
                  std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
                  std::string dest = std::string(1, (char)(j + 1 + 'a')) + std::string(1, (char)(i + direction + '1'));
                    Move *m = new Move(src, dest, Piece::QUEEN);
                    moves.push(m);
                }
                else if( (j - 1 > 0) && board[i + direction][j - 1].culoare == other_color){
                  std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
                  std::string dest = std::string(1, (char)(j - 1 + 'a')) + std::string(1, (char)(i + direction + '1'));
                    Move *m = new Move(src, dest, Piece::QUEEN);
                    moves.push(m);
                }
              }
          }
         for(int x = 0; x < 8; x++)
          for(int y = 0; y < 8; y++){
            // Generare mutare
            std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
            std::string dest = std::string(1, (char)(x + 'a')) + std::string(1, (char)(y + '1'));
            Move *m= new Move(src, dest, {});
            if(isLegalMove(board, *m)){
                moves.push(m);
            }
            else delete m;
          }
      }
    }

  // Generare mutari CrazyHouse
 for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++){
        // Verificam sa gasim o destinatie nula pentru piesa noastra
        if(board[i][j].culoare == PlaySide::NONE){
          std::string dest = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
          if(engineSide == PlaySide::WHITE){
            for(int k = 0; k < 6; k++){
              if(isCapturedWhite[k] > 0){
                  Piece replacement = (Piece)k;
                  // Nu avem voie sa dam dropIn unui pion de pe ultimele randuri
                  if(replacement == Piece::PAWN && (i == 0 || i == 7)) continue;
                  Move* move2 = Move::dropIn(dest, replacement);
                  moves.push(move2);
              }
            }
          }
          else if(engineSide == PlaySide::BLACK){
            for(int k = 0; k < 6; k++){
              if(isCapturedBlack[k] > 0){
                  Piece replacement = (Piece)k;
                  // Nu avem voie sa dam dropIn unui pion de pe ultimele randuri
                  if(replacement == Piece::PAWN && (i == 0 || i == 7)) continue;
                  Move* move3 = Move::dropIn(dest, replacement);
                  moves.push(move3);
              }
            }
          }
        }
    }
      // Generare mutari En Passant
    int last_src_num = last_src[1] - '1';
    int last_dest_lit = last_dest[0] - 'a';
    int last_dest_num = last_dest[1] - '1';

     bool enPassant = false;
    if(last_src != "zz" && board[last_dest_num][last_dest_lit].piesa == Piece::PAWN && abs(last_dest_num - last_src_num) == 2)
      enPassant = true;
    
      for(int j = 0; j < 8; j++){
        if(board[3][j].piesa == Piece::PAWN && board[3][j].culoare == PlaySide::BLACK && engineSide == PlaySide::BLACK){
          if(enPassant == true)
            {
              if(j - 1 > 0 && j - 1 == last_dest_lit){
              std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(3 + '1'));
              std::string dest = std::string(1, (char)(j - 1 + 'a')) + std::string(1, (char)(2+ '1'));
              Move *move4 = new Move(src, dest, {});
              moves.push(move4);
              }
              if(j + 1 < 8 && j + 1 == last_dest_lit){
                std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(3 + '1'));
                std::string dest = std::string(1, (char)(j + 1 + 'a')) + std::string(1, (char)(2 + '1'));
                Move *move4 = new Move(src, dest, {});
                moves.push(move4);
              }
            }
        }
      }
    
      for(int j = 0; j < 8; j++){
        if(board[4][j].piesa == Piece::PAWN && board[4][j].culoare == PlaySide::WHITE && engineSide == PlaySide::WHITE){
          if(enPassant == true)
          {
            if(j - 1 > 0 && j - 1 == last_dest_lit){
              std::string src = std::string(1, (char)(j + 'a')) + std::string(4, (char)(4 + '1'));
              std::string dest = std::string(1, (char)(j - 1 + 'a')) + std::string(1, (char)(5 + '1'));
              Move *move4 = new Move(src, dest, {});
              moves.push(move4);
            }
            if(j + 1 < 8 && j + 1 == last_dest_lit){
              std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(4 + '1'));
              std::string dest = std::string(1, (char)(j + 1 + 'a')) + std::string(1, (char)(5 + '1'));
              Move *move4 = new Move(src, dest, {});
              moves.push(move4);
            }
          }
        }
      }

    // Afisare mutari generate
    std::queue<Move*> cpyMoves = moves;
    while(!cpyMoves.empty()){
      Move *m = cpyMoves.front();
      cpyMoves.pop();
      std::string dest1= m->getDestination().value();
      if(m->isDropIn()){
        Piece replacement1 = m->getReplacement().value();
        int dst_lit2 = dest1[0] - 'a';
        int dst_num2 = dest1[1] - '1';
        std::cerr << "Buna! " << dest1 << " " << replacement1 << " " << board[dst_num2][dst_lit2].piesa << "\n";
      }
      else if(m->isNormal()){
        std::string src = m->getSource().value();
        std::string dest = m->getDestination().value();
        int src_lit11 = src[0] - 'a';
        int src_num11 = src[1] - '1';
        std::cerr << "Buna! " << src << " " << dest << " " << board[src_num11][src_lit11].piesa<< "\n";
      }
    }
  return moves;
}

std::string Bot::getBotName() { return Bot::BOT_NAME; }

bool Bot::isLegalMove(ChessPiece board[8][8], Move m){
        std::string src = m.getSource().value();
        std::string dest = m.getDestination().value();
        int src_lit = src[0] - 'a';
        int src_num = src[1] - '1';
        int dest_lit = dest[0] - 'a';
        int dest_num = dest[1] - '1';

        PlaySide curr_color = board[src_num][src_lit].culoare;
        // TRATARE PIESA PION
        if(board[src_num][src_lit].piesa == Piece::PAWN){
             int direction = (curr_color== PlaySide::WHITE) ? 1 : -1;

              // Daca pionul se misca pe diagonala
              if (src_lit != dest_lit) {
                  // Verificam daca putem lua piesa
                  ChessPiece dest_piece = board[dest_num][dest_lit];
                  if (dest_piece.culoare == PlaySide::NONE)
                      return false;

                  // Verificam sa nu fie piesa de aceeasi culoare
                  if (dest_piece.culoare == curr_color)
                      return false;

                  // Verificam sa fie diagonala
                  if ((src_lit + 1 == dest_lit || src_lit - 1 == dest_lit) && src_num + direction == dest_num){
                        return true;
                  }

                  return false;
              }
            // std::cerr << "buna! sunt mutarea " << src << " " << dest << " \n";
              
              // Daca src_lit = dest_lit, verificam sa fie neaparat casuta libera.
              if(board[dest_num][dest_lit].culoare != PlaySide::NONE)
                  return false;
            // std::cerr << "offf tot aici sunt \n";

              // Mutare pion alb
              if(direction == 1){
                  if(dest_num == src_num + 1){
                      return true;
                   }
                  // Putem sari doua casute la inceput doar daca avem casuta libera intre
                  if(src_num == 1 && dest_num == 3  && board[2][src_lit].culoare == PlaySide::NONE){
                      return true;
                  }
              }
              // Mutare pion negru
              else {
                  if(src_num == dest_num + 1){
                      return true;
                  }
                  if(src_num == 6 && dest_num == 4 && board[5][src_lit].culoare == PlaySide::NONE){
                      return true;
                  }
              }
              return false;
          }
          // CAZ TURA
          else if(board[src_num][src_lit].piesa == Piece::ROOK){
            // Daca miscarea turei este orizontala
              if (src_num == dest_num) {
                  // Daca piesa nu se misca, mutarea e falsa
                  if(src_lit == dest_lit)
                      return false;
                  int start = std::min(src_lit, dest_lit);
                  int end = std::max(src_lit, dest_lit);
                  for (int i = start + 1; i < end; i++) {
                      // Verificam sa nu fie piese in drum.
                      if (board[src_num][i].culoare != PlaySide::NONE) {
                          return false;  
                      }
                  }
                  return board[dest_num][dest_lit].culoare == !curr_color|| board[dest_num][dest_lit].culoare == PlaySide::NONE;
              }
              // Daca miscarea este verticala
              else if (src_lit == dest_lit) {
                  int start = std::min(src_num, dest_num);
                  int end = std::max(src_num, dest_num);
                  for (int i = start + 1; i < end; i++) {
                      if (board[i][src_lit].culoare != PlaySide::NONE) {
                          return false;  // e o piesa in drum
                      }
                  }
                  return board[dest_num][dest_lit].culoare == !curr_color  || board[dest_num][dest_lit].culoare == PlaySide::NONE;
              }
              else return false;
          }
          else if(board[src_num][src_lit].piesa == Piece::KNIGHT){
             if(board[dest_num][dest_lit].culoare == curr_color ){
                  return false;
              }

              // Verificam miscarea de cal.
              int d_num = abs(src_num - dest_num);
              int d_lit = abs(src_lit - dest_lit);
              return (d_num == 1 && d_lit == 2) || (d_num == 2 && d_lit == 1);
          }
          else if(board[src_num][src_lit].piesa == Piece::BISHOP){
            int d_num = abs(src_num - dest_num);
            int d_lit = abs(src_lit - dest_lit);

            // Mutare invalida
            if (src_lit == dest_lit && src_num == dest_num) {
                return false;
            }

            // Daca mutarea nu e diagonala, returnam fals
            if (d_num != d_lit) {
                return false;
            }

            // Verificam existenta pieselor in drum
            int x_dir = (dest_num > src_num) ? 1 : -1;
            int y_dir = (dest_lit > src_lit) ? 1 : -1;
            int x = src_num + x_dir;
            int y = src_lit + y_dir;
            while (x != dest_num && y != dest_lit) {
                if (board[x][y].culoare != PlaySide::NONE) {
                    return false;
                }
                x += x_dir;
                y += y_dir;
            }

            // Daca destinatia e aceeasi culoare, returnam fals.
            if (board[dest_num][dest_lit].culoare == curr_color) {
                return false;
            }

            return true;
          }
          else if(board[src_num][src_lit].piesa == Piece::QUEEN){
            int d_num = abs(src_num - dest_num);
            int d_lit = abs(src_lit - dest_lit);

            // Daca miscarea este orizontala
            if (src_num == dest_num) {
                // Daca piesa nu se misca, mutarea e falsa
                if(src_lit == dest_lit)
                    return false;
                int start = std::min(src_lit, dest_lit);
                int end = std::max(src_lit, dest_lit);
                for (int i = start + 1; i < end; i++) {
                    // Verificam sa nu fie piese in drum.
                    if (board[src_num][i].culoare != PlaySide::NONE) {
                        return false;  
                    }
                }
                return board[dest_num][dest_lit].culoare== !curr_color|| board[dest_num][dest_lit].culoare == PlaySide::NONE;
            }
            else if (src_lit == dest_lit) {
                int start = std::min(src_num, dest_num);
                int end = std::max(src_num, dest_num);
                for (int i = start + 1; i < end; i++) {
                    if (board[i][src_lit].culoare != PlaySide::NONE) {
                        return false;  // e o piesa in drum
                    }
                }
                return board[dest_num][dest_lit].culoare == !curr_color  || board[dest_num][dest_lit].culoare== PlaySide::NONE;
            }
            else if (d_num == d_lit) {
                int x_dir = (dest_num > src_num) ? 1 : -1;
                int y_dir = (dest_lit > src_lit) ? 1 : -1;
                int x = src_num + x_dir;
                int y = src_lit + y_dir;
                while (x != dest_num && y != dest_lit) {
                    if (board[x][y].culoare != PlaySide::NONE) {
                        return false;
                    }
                    x += x_dir;
                    y += y_dir;
                }

                // Daca destinatia e aceeasi culoare, returnam fals.
                if (board[dest_num][dest_lit].culoare == curr_color) {
                    return false;
                }
                return true;
            }
            else return false;
          }
          else if(board[src_num][src_lit].piesa == Piece::KING){
            if(src_lit == dest_lit && src_num == dest_num){
                return false;
            }
            int d_num = abs(src_num - dest_num);
            int d_lit = abs(src_lit - dest_lit);
            // daca la destinatie ai o piesa de aceeasi culoare
            if(board[dest_num][dest_lit].culoare == curr_color)
                return false;
            return(d_num <= 1 && d_lit <= 1);
          }
          else if(board[src_num][src_lit].piesa == Piece::EMPTY){
            return true;
          }
          else return false;
}

bool Bot::isKinginCheck(ChessPiece cpyBoard[8][8], PlaySide engineSide){
  // Cautare rege
  int king_num = -1, king_col = -1;
    for(int i = 0; i < 8; i++)
      for(int j= 0; j < 8; j++)
        if(cpyBoard[i][j].piesa == Piece::KING && cpyBoard[i][j].culoare == engineSide){
            king_num = i;
            king_col = j;
            break;
        }
    
    // Culoare oponent
    PlaySide opponentSide = (engineSide == PlaySide::WHITE) ? PlaySide::BLACK : PlaySide::WHITE;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        // Cautam piese de culoarea oponentului
        if (cpyBoard[i][j].culoare == opponentSide) {
          // Extragem mutari tip piesa_oponent->rege
          std::string src = std::string(1, (char)(j + 'a')) + std::string(1, (char)(i + '1'));
          std::string dest = std::string(1, (char)(king_col + 'a')) + std::string(1, (char)(king_num + '1'));
          Move move(src, dest, {});
          if (isLegalMove(cpyBoard, move)) {
                return true;
              }
          }
      }
  } 
  // Daca nicio piesa a oponentului nu genereaza sah, atunci returnam fals.
  return false;
}

bool Bot::isSquareSafe(int row, int col, PlaySide engineSide){
  std::queue<Move*> moves2;
  PlaySide playside = (engineSide == PlaySide::WHITE) ? PlaySide::BLACK : PlaySide::WHITE;
  
  moves2 = generateLegalMoves(playside);
  while(!moves2.empty()){
    Move *m2 = moves2.front();
    moves2.pop();
    std::string dest = m2->getDestination().value();
    delete m2;
    int dest_lit = dest[0] - 'a';
    int dest_num = dest[1] - '1';
    if(dest_lit == col && dest_num == row){
      return false;
    }
  }
    return true;
}

int Bot::isCastling(PlaySide engineSide){
  int side = (engineSide == PlaySide::WHITE) ? 0 : 7;
  int ok = 1;
  ChessPiece king, rook_s, rook_l;
  king = board[side][4];
  rook_s = board[side][7]; // rocada mica
  rook_l = board[side][0]; // rocada mare

  // Daca pe pozitia regelui nu e regele
  if(king.piesa != Piece::KING){
    return -1;
  }
  
  // Daca pe pozitia turei e ROOK 
  if(rook_l.piesa == Piece::ROOK){
    // Verificam daca piesele au mai fost mutate sau nu
    if(rook_l.moves_count != 0 || king.moves_count != 0){
        ok = 0;
    }
    // Verificam daca avem spatii goale intre piese
    for(int i = 1; i <= 3; i++){
      if(board[side][i].piesa != Piece::EMPTY){
        ok = 0;
      }
    }
    int side2 = (engineSide == PlaySide::WHITE) ? 1 : 6;
    int oppSide = (engineSide == PlaySide::WHITE) ? PlaySide::BLACK : PlaySide::WHITE;
    for(int i = 1; i <= 5; i++)
      if(board[side2][i].piesa == Piece::PAWN && board[side2][i].culoare == oppSide)
        ok = 0;
    if(ok == 1)
      if(isSquareSafe(side, 2, engineSide) && isSquareSafe(side, 3, engineSide) && isSquareSafe(side, 4, engineSide)){
      return 1; // 1 = facem rocada mare
    }
  }
  ok = 1;
  if(rook_s.piesa == Piece::ROOK){
    if(king.moves_count != 0 || rook_s.moves_count != 0){
      ok = 0;
    }
    for(int i = 5; i <= 6; i++){
      if(board[side][i].piesa != Piece::EMPTY){
        ok = 0;
      }
    }
    int side2 = (engineSide == PlaySide::WHITE) ? 1 : 6;
    int oppSide = (engineSide == PlaySide::WHITE) ? PlaySide::BLACK : PlaySide::WHITE;
    for(int i = 3; i <= 7; i++)
      if(board[side2][i].piesa == Piece::PAWN && board[side2][i].culoare == oppSide)
        ok = 0;
    if(ok == 1)
      if(isSquareSafe(side, 5, engineSide) && isSquareSafe(side, 4, engineSide) && isSquareSafe(side, 6, engineSide)){
        return 0; // 0 = facem rocada mica
      }
  }
  return -1;
}