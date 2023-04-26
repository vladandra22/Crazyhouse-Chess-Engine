#include "ChessBoard.h"
#include <cstdlib>
#include <algorithm>
#include <cmath>

using namespace std;
 
// Constructor piesa, cu tipul piesei si culoare
MyPiece::MyPiece(Piece m_piesa, PlaySide m_culoare) : piesa(m_piesa), culoare(m_culoare) {}
// Deconstructor piesa.
MyPiece::~MyPiece() {}
// Getteri piesa.
Piece MyPiece::getType () {return piesa; }
PlaySide MyPiece::getColor() {return culoare; }

// Generare miscari pioni 
Pawn::Pawn(PlaySide culoare) : MyPiece(Piece::PAWN, culoare) {}
bool Pawn::isLegalMove(const Board& board, Move m) {
        // Extragere coordonate din miscare.
        string src = m.getSource().value();
        string dest = m.getDestination().value();
        int src_lit = src[0] - 'a';
        int src_num = src[1] - '1';
        int dest_lit = dest[0] - 'a';
        int dest_num = dest[1] - '1';

        // Selectare culoare curenta a pionului
        int curr_color = this->culoare;
      //  MyPiece* piesa = board.getPiece(src_num, src_lit);

        // Vedem directia de mers a pionului in functie de culaoare.
        int direction = (curr_color== PlaySide::WHITE) ? 1 : -1;

        // Daca pionul se misca pe diagonala
        if (src_lit != dest_lit) {
            // Verificam daca putem lua piesa
            MyPiece* dest_piece = board.getPiece(dest_num, dest_lit);
            if (!dest_piece || dest_piece->getColor() == PlaySide::NONE)
                return false;

            // Verificam sa nu fie piesa de aceeasi culoare
            if (dest_piece->getColor() == curr_color)
                return false;

            // Verificam sa fie diagonala
            if ((src_lit + 1 == dest_lit || src_lit - 1 == dest_lit) && src_num + direction == dest_num)
                return true;

            return false;
        }
       // std::cerr << "buna! sunt mutarea " << src << " " << dest << " \n";
        
        // Daca src_lit = dest_lit, verificam sa fie neaparat casuta libera.
        if(board.getPiece(dest_num, dest_lit)->getColor() != PlaySide::NONE)
            return false;
       // std::cerr << "offf tot aici sunt \n";

        // Mutare pion alb
        if(direction == 1){
            if(dest_num == src_num + 1){
                return true;
            }
            // Putem sari doua casute la inceput doar daca avem casuta libera intre
            if(src_num == 1 && dest_num == 3  && board.getPiece(2, src_lit)->getColor() == PlaySide::NONE){
                return true;
            }
        }
        // Mutare pion negru
        else {
            if(src_num == dest_num + 1){
                return true;
            }
            if(src_num == 6 && dest_num == 4 && board.getPiece(5, src_lit)->getColor() == PlaySide::NONE){
                return true;
            }
        }
        return false;
    }

Rook::Rook(PlaySide culoare) : MyPiece(Piece::ROOK, culoare) {}

bool Rook::isLegalMove(const Board& board, Move m) {
    // Extragere coordonate din miscare.
    string src = m.getSource().value();
    string dest = m.getDestination().value();
    int src_lit = src[0] - 'a';
    int src_num = src[1] - '1';
    int dest_lit = dest[0] - 'a';
    int dest_num = dest[1] - '1';

    // Culoare piesa curenta.
    int curr_color = this->culoare;

    // Daca miscarea turei este orizontala
    if (src_num == dest_num) {
        // Daca piesa nu se misca, mutarea e falsa
        if(src_lit == dest_lit)
            return false;
        int start = min(src_lit, dest_lit);
        int end = max(src_lit, dest_lit);
        for (int i = start + 1; i < end; i++) {
            // Verificam sa nu fie piese in drum.
            if (board.getPiece(src_num, i)->getColor() != PlaySide::NONE) {
                return false;  
            }
        }
        return board.getPiece(dest_num, dest_lit)->getColor() == !curr_color|| board.getPiece(dest_num, dest_lit)->getColor() == PlaySide::NONE || !board.getPiece(dest_num, dest_lit);
    }
    // Daca miscarea este verticala
    else if (src_lit == dest_lit) {
        int start = min(src_num, dest_num);
        int end = max(src_num, dest_num);
        for (int i = start + 1; i < end; i++) {
            if (board.getPiece(i, src_lit)->getColor() != PlaySide::NONE) {
                return false;  // e o piesa in drum
            }
        }
        return board.getPiece(dest_num, dest_lit)->getColor() == !curr_color  || board.getPiece(dest_num, dest_lit)->getColor() == PlaySide::NONE  || !board.getPiece(dest_num, dest_lit);
    }
    else return false;
}

Bishop::Bishop(PlaySide culoare) : MyPiece(Piece::BISHOP, culoare) {};
bool Bishop::isLegalMove(const Board& board, Move m) {
    // Extragere coordonate din miscare
    string src = m.getSource().value();
    string dest = m.getDestination().value();
    int src_lit = src[0] - 'a';
    int src_num = src[1] - '1';
    int dest_lit = dest[0] - 'a';
    int dest_num = dest[1] - '1';

    
    // Verificam miscarea e diagonala
    int d_num = abs(src_num - dest_num);
    int d_lit = abs(src_lit - dest_lit);
    int curr_color = this->culoare;

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
        if (board.getPiece(x, y)->getColor() != PlaySide::NONE) {
            return false;
        }
        x += x_dir;
        y += y_dir;
    }

    // Daca destinatia e aceeasi culoare, returnam falds.
    if (board.getPiece(dest_num, dest_lit)->getColor() == curr_color) {
        return false;
    }

    return true;
}


Knight::Knight(PlaySide culoare) : MyPiece(Piece::KNIGHT, culoare) {};
bool Knight::isLegalMove(const Board& board, Move m) {
    // Extragere coordonate din miscare
    string src = m.getSource().value();
    string dest = m.getDestination().value();
    int src_lit = src[0] - 'a'; 
    int src_num = src[1] - '1'; 
    int dest_lit = dest[0] - 'a'; 
    int dest_num = dest[1] - '1'; 

    // Extragere culoare piesa.
    int curr_color = this->culoare;

    // Invalida mutarea pe o piesa de aceeasi culoare
    if(board.getPiece(dest_num, dest_lit)->getColor() == curr_color ){
        return false;
    }

    // Verificam miscarea de cal.
    int d_num = abs(src_num - dest_num);
    int d_lit = abs(src_lit - dest_lit);
    return (d_num == 1 && d_lit == 2) || (d_num == 2 && d_lit == 1);
}

Queen::Queen(PlaySide culoare) : MyPiece(Piece::QUEEN, culoare) {};
bool Queen::isLegalMove(const Board& board, Move m) {
    // Extragere coordonate din miscare.
    string src = m.getSource().value();
    string dest = m.getDestination().value();
    int src_lit = src[0] - 'a';
    int src_num = src[1] - '1';
    int dest_lit = dest[0] - 'a';
    int dest_num = dest[1] - '1';
        
    int d_num = abs(src_num - dest_num);
    int d_lit = abs(src_lit - dest_lit);
    int curr_color = board.getPiece(src_num, src_lit)->getColor();

    // Daca miscarea este orizontala
    if (src_num == dest_num) {
        // Daca piesa nu se misca, mutarea e falsa
        if(src_lit == dest_lit)
            return false;
        int start = min(src_lit, dest_lit);
        int end = max(src_lit, dest_lit);
        for (int i = start + 1; i < end; i++) {
            // Verificam sa nu fie piese in drum.
            if (board.getPiece(src_num, i)->getColor() != PlaySide::NONE) {
                return false;  
            }
        }
        return board.getPiece(dest_num, dest_lit)->getColor() == !curr_color|| board.getPiece(dest_num, dest_lit)->getColor() == PlaySide::NONE || !board.getPiece(dest_num, dest_lit);
    }
    else if (src_lit == dest_lit) {
        int start = min(src_num, dest_num);
        int end = max(src_num, dest_num);
        for (int i = start + 1; i < end; i++) {
            if (board.getPiece(i, src_lit)->getColor() != PlaySide::NONE) {
                return false;  // e o piesa in drum
            }
        }
        return board.getPiece(dest_num, dest_lit)->getColor() == !curr_color  || board.getPiece(dest_num, dest_lit)->getColor() == PlaySide::NONE  || !board.getPiece(dest_num, dest_lit);
    }
    else if (d_num == d_lit) {
        int x_dir = (dest_num > src_num) ? 1 : -1;
        int y_dir = (dest_lit > src_lit) ? 1 : -1;
        int x = src_num + x_dir;
        int y = src_lit + y_dir;
        while (x != dest_num && y != dest_lit) {
            if (board.getPiece(x, y)->getColor() != PlaySide::NONE) {
                return false;
            }
            x += x_dir;
            y += y_dir;
        }

        // Daca destinatia e aceeasi culoare, returnam fals.
        if (board.getPiece(dest_num, dest_lit)->getColor() == curr_color) {
            return false;
        }
        return true;
    }
    else return false;
}

King::King(PlaySide culoare) : MyPiece(Piece::KING, culoare) {};
bool King::isLegalMove(const Board& board, Move m) {
        string src = m.getSource().value();
        string dest = m.getDestination().value();
        int src_lit = src[0] - 'a';
        int src_num = src[1] - '1';
        int dest_lit = dest[0] - 'a';
        int dest_num = dest[1] - '1';

        int curr_color = this->culoare;

        // Verificam daca sursa este aceeasi cu destinatia
        if(src_lit == dest_lit && src_num == dest_num){
            return false;
        }
        int d_num = abs(src_num - dest_num);
        int d_lit = abs(src_lit - dest_lit);
        // daca la destinatie ai o piesa de aceeasi culoare
        if(board.getPiece(dest_num, dest_lit)->getColor() == curr_color)
            return false;
        return(d_num <= 1 && d_lit <= 1);
}

// Orice miscare este legata pe un patratel liber
EmptySquare::EmptySquare(): MyPiece(Piece::EMPTY, PlaySide::NONE) {};
bool EmptySquare::isLegalMove(const Board& board, Move m) {
        return true;
    }

// Initializare tabla.
Board::Board() {
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(i == 0){
                    switch(j){
                        case 0: 
                        case 7: board[i][j] = new Rook(PlaySide::WHITE); break;
                        case 1:
                        case 6: board[i][j] = new Knight(PlaySide::WHITE); break;
                        case 2: 
                        case 5: board[i][j] = new Bishop(PlaySide::WHITE); break;
                        case 3: board[i][j] = new Queen(PlaySide::WHITE); break;
                        case 4: board[i][j] = new King(PlaySide::WHITE); break;
                    }
                }
                else if(i == 7){
                     switch(j){
                        case 0: 
                        case 7: board[i][j] = new Rook(PlaySide::BLACK); break;
                        case 1:
                        case 6: board[i][j] = new Knight(PlaySide::BLACK); break;
                        case 2: 
                        case 5: board[i][j] = new Bishop(PlaySide::BLACK); break;
                        case 3: board[i][j] = new Queen(PlaySide::BLACK); break;
                        case 4: board[i][j] = new King(PlaySide::BLACK); break;
                    }
                }
                else if(i == 1){
                    board[i][j] = new Pawn(PlaySide::WHITE);
                }
                else if(i == 6){
                    board[i][j] = new Pawn(PlaySide::BLACK);
                }
                else board[i][j] = new EmptySquare();
            }
        }
    }

//Eliberare tabla
Board::~Board() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                delete board[i][j];
            }
        }
}

// Returneaza o piesa de pe tabla
MyPiece* Board::getPiece(int lin, int col) const {
        return board[lin][col];
}

// Muta piesa
void Board::movePiece(Move *m){
        string src = m->getSource().value();
        string dest = m->getDestination().value();
        int src_lit = src[0] - 'a';
        int src_num = src[1] - '1';
        int dest_lit = dest[0] - 'a';
        int dest_num = dest[1] - '1';
        Move::moveTo(m->getSource(), m->getDestination()); 
        MyPiece* piece = board[src_num][src_lit];
        board[src_num][src_lit] = new EmptySquare();
        board[dest_num][dest_lit] = piece;
}

// Produce o miscare de undo
void Board::undoPiece(Move *m){
    string src = m->getSource().value();
    string dest = m->getDestination().value();
    int src_lit = src[0] - 'a';
    int src_num = src[1] - '1';
    int dest_lit = dest[0] - 'a';
    int dest_num = dest[1] - '1';
    MyPiece* piece = board[src_num][src_lit];
    board[src_num][src_lit] = board[dest_num][dest_lit];
    board[dest_num][dest_lit] = piece;
    Move::moveTo(m->getDestination(), m->getSource());
}