#include <iostream>
#include <cstdint>
#include <string>

#include "board.h"
#include "types.h"

namespace Ecifircas {
    Bitboard Pieces[2][6];

    // Starting position FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    // letters stand for pieces, numbers is blank squares, and / means a new rank
    // w or b stands for white or black to move
    // KQkq is keeping track of castling
    // - could be a square like e3, and is an en passant target square (can only be one because en passant only last for 1 turn)
    // 0 is halfmove clock, used for 50 move rule counting
    // 1 is full move clock, starts at one and is incremented after blacks move
    void set_board(std::string fen) {
        int rank = 8;
        int file = 1;

        for (char c : fen) {
            if (c == ' ') {
                break;
            }
            else if (isdigit(c)) {
                file += c - '0';
            }
            else if (c == '/') {
                rank--;
                file = 1;
            }
            else {
                // Do something with letter
                Color color;
                Piece piece;
                int square = (file - 1) + (8 * (rank - 1));
                if (c > 64 && c < 91) {     // white
                    color = WHITE;
                    c -= 65;
                }
                else if (c > 96 && c < 123) {      // black
                    color = BLACK;
                    c -= 97;
                }

                if (c == ('P' - 65)) {
                    piece = PAWN;
                }
                else if (c == 'N' - 65) {
                    piece = KNIGHT;
                }
                else if (c == 'B' - 65) {
                    piece = BISHOP;
                }
                else if (c == 'R' - 65) {
                    piece = ROOK;
                }
                else if (c == 'Q' - 65) {
                    piece = QUEEN;
                }
                else {
                    piece = KING;
                }

                Pieces[color][piece] |= (static_cast<Bitboard>(1) << square);
                file++;
            }
        }
    }



    void print_board() {
        std::string output = "\n +---+---+---+---+---+---+---+---+\n";
        std::string pieceToChar = "PNBRQKpnbrqk";

        for (int rank = 7; rank >= 0; rank--) { // FIX GROSS NESTING
            for (int file = 0; file <= 7; file++) {
                bool pieceAddedFlag = false;
                int square = file + (rank * 8);
                output += " | ";

                for (Piece piece = PAWN; piece <= KING; piece++) {
                    for (Color color = WHITE; color <= BLACK; color++) {
                        if ((Pieces[color][piece] & (static_cast<Bitboard>(1) << square)) != 0) {
                            output += pieceToChar[(color * 6) + piece];
                            pieceAddedFlag = true;
                        }
                    }
                }

                if (!pieceAddedFlag) {
                    output += " ";
                }
            }

            output += " | " + std::to_string(rank + 1) + "\n +---+---+---+---+---+---+---+---+\n";
        }
        
        output += "   a   b   c   d   e   f   g   h\n";

        std::cout << output << std::endl;
    }
}


