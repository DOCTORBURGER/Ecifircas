#include <iostream>
#include <cstdint>
#include <string>
#include <regex>
#include <vector>
#include <sstream>

#include "board.h"
#include "types.h"

namespace Ecifircas {
    Bitboard Pieces[2][6];

    Bitboard Occupancies[3];

    enum Castle : int {
        WK = 1,
        WQ = 2,
        BK = 4,
        BQ = 8
    };

    int CastleRights = 15;

    Color SideToMove = WHITE;

    Square EnpassantSquare = NO_SQUARE;

    const std::string SquareToCoordinates[] = {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "no square"
    };

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::stringstream ss(str);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    bool is_valid_rank(const std::string& rank) {
        if (rank.empty()) return false;

        int count = 0;
        for (char ch : rank) {
            if (std::isdigit(ch)) {
                count += ch - '0'; // Convert char to int and add to count
            }
            else if (std::isalpha(ch)) {
                count++; // Increment count for each piece
            }
            else {
                return false; // Invalid character
            }
        }

        return count == 8; // Each rank must have exactly 8 squares
    }

    bool is_valid_FEN(const std::string& fen) {
        std::istringstream iss(fen);
        std::vector<std::string> parts;
        std::string part;

        while (std::getline(iss, part, ' ')) {
            parts.push_back(part);
        }

        if (parts.size() != 6) return false; // FEN should have 6 parts

        std::istringstream ranks(parts[0]);
        std::string rank;
        while (std::getline(ranks, rank, '/')) {
            if (!is_valid_rank(rank)) return false;
        }

        std::regex fenRegex(
            "^([rnbqkpRNBQKP1-8]{1,8}/){7}[rnbqkpRNBQKP1-8]{1,8}\\s[bw]\\s(-|[KQkq]{1,4})\\s(-|[a-h][1-8])\\s\\d+\\s\\d+$"
        );

        return std::regex_match(fen, fenRegex);
    }

    Square string_to_square(const std::string& squareStr) {
        if (squareStr == "-") {
            return NO_SQUARE;
        }

        if (squareStr.length() == 2 && squareStr[0] >= 'a' && squareStr[0] <= 'h' && squareStr[1] >= '1' && squareStr[1] <= '8') {
            int file = squareStr[0] - 'a'; // Convert 'a'-'h' to 0-7
            int rank = squareStr[1] - '1'; // Convert '1'-'8' to 0-7
            return static_cast<Square>(8 * rank + file);
        }

        return NO_SQUARE; // Return NO_SQUARE for invalid inputs
    }

    // Starting position FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    // letters stand for pieces, numbers is blank squares, and / means a new rank
    // w or b stands for white or black to move
    // KQkq is keeping track of castling
    // - could be a square like e3, and is an en passant target square (can only be one because en passant only last for 1 turn)
    // 0 is halfmove clock, used for 50 move rule counting
    // 1 is full move clock, starts at one and is incremented after blacks move
    void set_board(std::string fen) {
        if (!is_valid_FEN(fen)) {
            std::cout << "Invalid FEN" << std::endl;
            return;
        }

        memset(Pieces, 0, sizeof(Pieces));

        std::vector<std::string> fenParts = split(fen, ' ');

        int rank = 8;
        int file = 1;

        for (char c : fenParts[0]) { // Piece Placement
            if (isdigit(c)) {
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

        SideToMove = (fenParts[1] == "w") ? WHITE : BLACK;

        CastleRights = 0;
        for (char c : fenParts[2]) {
            switch (c) {
                case 'K': CastleRights |= WK; break;
                case 'Q': CastleRights |= WQ; break;
                case 'k': CastleRights |= BK; break;
                case 'q': CastleRights |= BQ; break;
                default: break;
            }
        }

        EnpassantSquare = string_to_square(fenParts[3]);

        for (Piece piece = PAWN; piece <= KING; piece++) {
            Occupancies[0] |= Pieces[WHITE][piece];
            Occupancies[1] |= Pieces[BLACK][piece];
        }
        Occupancies[2] = Occupancies[0] | Occupancies[1];
    }



    void print_board() {
        std::string output = "\n +---+---+---+---+---+---+---+---+\n";
        std::string pieceToChar = "PNBRQKpnbrqk";

        for (int rank = 7; rank >= 0; rank--) {
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
        
        output += "   a   b   c   d   e   f   g   h\n\n";

        output += "To move: ";
        output += (SideToMove == WHITE) ? "white\n" : "black\n";

        output += "Castle rights: ";
        if (CastleRights & WK) output += "K";
        if (CastleRights & WQ) output += "Q";
        if (CastleRights & BK) output += "k";
        if (CastleRights & BQ) output += "q";
        if (!(CastleRights & (WK | WQ | BK | BQ))) output += "-";
        output += "\n";

        output += "EP square: ";
        output += SquareToCoordinates[EnpassantSquare];

        std::cout << output << std::endl;
    }
}


