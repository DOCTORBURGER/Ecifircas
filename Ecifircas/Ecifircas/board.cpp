#include <iostream>
#include <cstdint>
#include <string>
#include "board.h"

unsigned long long whitePawns;

unsigned long long blackPawns;

unsigned long long whiteKnights;

unsigned long long blackKnights;

unsigned long long whiteBishops;

unsigned long long blackBishops;

unsigned long long whiteRooks;

unsigned long long blackRooks;

unsigned long long whiteQueens;

unsigned long long blackQueens;

unsigned long long whiteKing;

unsigned long long blackKing;

bool isBitSet(unsigned long long board, int position) {
    return (board & (1ULL << (63 - position))) != 0;
}

void initializeBoard() {
    whitePawns = 0x000000000000FF00ULL; 
    blackPawns = 0x00FF000000000000ULL;

    whiteKnights = 0x0000000000000042ULL;
    blackKnights = 0x4200000000000000ULL;

    whiteBishops = 0x0000000000000024ULL;
    blackBishops = 0x2400000000000000ULL;

    whiteRooks = 0x0000000000000081ULL;
    blackRooks = 0x8100000000000000ULL;

    whiteQueens = 0x0000000000000010ULL;
    blackQueens = 0x1000000000000000ULL;

    whiteKing = 0x0000000000000008ULL;
    blackKing = 0x0800000000000000ULL;
}

void printBoard() {
    std::string board = "";

    for (int rank = 0; rank < 8; rank++) {
        board += std::to_string(rank + 1) + " ";

        for (int file = 0; file < 8; file++) {
            int square = (rank * 8) + file;

            if (isBitSet(whitePawns, square)) {
                board += " P";
            }
            else if (isBitSet(blackPawns, square)) {
                board += " p";
            }
            else if (isBitSet(whiteKnights, square)) {
                board += " N";
            }
            else if (isBitSet(blackKnights, square)) {
                board += " n";
            }
            else if (isBitSet(whiteBishops, square)) {
                board += " B";
            }
            else if (isBitSet(blackBishops, square)) {
                board += " b";
            }
            else if (isBitSet(whiteRooks, square)) {
                board += " R";
            }
            else if (isBitSet(blackRooks, square)) {
                board += " r";
            }
            else if (isBitSet(whiteQueens, square)) {
                board += " Q";
            }
            else if (isBitSet(blackQueens, square)) {
                board += " q";
            }
            else if (isBitSet(whiteKing, square)) {
                board += " K";
            }
            else if (isBitSet(blackKing, square)) {
                board += " k";
            }
            else {
                board += " .";
            }
            
        }
        board += "\n";
    }

    board += "\n   A B C D E F G H\n\n";
    
    std::cout << board;
}
