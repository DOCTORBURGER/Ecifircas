#include <iostream>
#include <cstdint>
#include <string>
#include <regex>
#include <vector>
#include <sstream>

#include "board.h"
#include "types.h"
#include "bitboard.h"

namespace Ecifircas 
{
    Bitboard Pieces[2][6];

    Bitboard Occupancies[3];

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

    const int CastlingRightsSquares[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
    };

    bool make_move(Move move, QuiescenceMoveFlag quieFlag)
    {
        if (quieFlag == ALL_MOVES) {
            copy_board();

            Square sourceSquare = move.from_sq();
            Square targetSquare = move.to_sq();
            Piece piece = move.get_piece();
            MoveFlags flag = move.flags();
            const Color oppositeColor = Color(SideToMove ^ 1);

            pop_bit(Pieces[SideToMove][piece], sourceSquare);
            set_bit(Pieces[SideToMove][piece], targetSquare);

            if (flag & CAPTURE) {
                for (Piece pieceType = PAWN; pieceType <= KING; pieceType++) {
                    if (get_bit(Pieces[oppositeColor][pieceType], targetSquare)) {
                        pop_bit(Pieces[oppositeColor][pieceType], targetSquare);
                        break;
                    }
                }
            }

            // Promotion
            if (0x8 & flag) {
                Piece promotedPiece = Piece(KNIGHT + (flag & 0x3));
                pop_bit(Pieces[SideToMove][piece], targetSquare);
                pop_bit(Pieces[SideToMove][promotedPiece], targetSquare);
            }

            if (flag == EP_CAPTURE) {
                pop_bit(Pieces[oppositeColor][PAWN], Square(targetSquare + ((SideToMove) ? 8 : -8)));
            }

            EnpassantSquare = NO_SQUARE;

            if (flag == DOUBLE_PAWN_PUSH) {
                EnpassantSquare = Square(targetSquare + ((SideToMove) ? 8 : -8));
            }

            if (flag == KING_CASTLE) {
                pop_bit(Pieces[SideToMove][ROOK], Square(H1 + ((SideToMove) ? 56 : 0)));
                set_bit(Pieces[SideToMove][ROOK], Square(F1 + ((SideToMove) ? 56 : 0)));
            }

            if (flag == QUEEN_CASTLE) {
                pop_bit(Pieces[SideToMove][ROOK], Square(A1 + ((SideToMove) ? 56 : 0)));
                set_bit(Pieces[SideToMove][ROOK], Square(D1 + ((SideToMove) ? 56 : 0)));
            }

            CastleRights &= CastlingRightsSquares[sourceSquare];
            CastleRights &= CastlingRightsSquares[targetSquare];

            memset(Occupancies, 0, sizeof(Occupancies));

            for (Piece piece = PAWN; piece <= KING; piece++) {
                Occupancies[WHITE] |= Pieces[WHITE][piece];
                Occupancies[BLACK] |= Pieces[BLACK][piece];
            }
            Occupancies[2] = Occupancies[WHITE] | Occupancies[BLACK];

            

            if (is_square_attacked(Square(get_ls1b_index(Pieces[SideToMove][KING])), oppositeColor)) {
                restore_from_copy();
                return false;
            }
            else {
                SideToMove = oppositeColor;
                return true;
            }
        }
        else {
            if (move.flags() == CAPTURE) {
                make_move(move, ALL_MOVES);
            }
            else {
                return false;
            }
        }
    }

    std::vector<std::string> split(const std::string& str, char delimiter) 
    {
        std::stringstream ss(str);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    bool is_valid_rank(const std::string& rank) 
    {
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

    bool is_valid_FEN(const std::string& fen) 
    {
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
                case 'K': CastleRights |= WHITE_KINGSIDE; break;
                case 'Q': CastleRights |= WHITE_QUEENSIDE; break;
                case 'k': CastleRights |= BLACK_KINGSIDE; break;
                case 'q': CastleRights |= BLACK_QUEENSIDE; break;
                default: break;
            }
        }

        EnpassantSquare = string_to_square(fenParts[3]);

        for (Piece piece = PAWN; piece <= KING; piece++) {
            Occupancies[WHITE] |= Pieces[WHITE][piece];
            Occupancies[BLACK] |= Pieces[BLACK][piece];
        }
        Occupancies[2] = Occupancies[WHITE] | Occupancies[BLACK];
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

        output += "    To move:       ";
        output += (SideToMove == WHITE) ? "white\n" : "black\n";

        output += "    Castle rights: ";
        if (CastleRights & WHITE_KINGSIDE) output += "K";
        if (CastleRights & WHITE_QUEENSIDE) output += "Q";
        if (CastleRights & BLACK_KINGSIDE) output += "k";
        if (CastleRights & BLACK_QUEENSIDE) output += "q";
        if (!(CastleRights & (WHITE_KINGSIDE | WHITE_QUEENSIDE | BLACK_KINGSIDE | BLACK_QUEENSIDE))) output += "-";
        output += "\n";

        output += "    EP square:     ";
        output += SquareToCoordinates[EnpassantSquare];

        std::cout << output << std::endl;
    }

    inline bool is_square_attacked(Square square, Color attackingSide) {
        Color oppositeSide = (attackingSide == WHITE) ? BLACK : WHITE;
        return  (pawn_attacks_bb(square, oppositeSide) & Pieces[attackingSide][PAWN])
            | (attacks_bb(square, Occupancies[BOTH], KNIGHT) & Pieces[attackingSide][KNIGHT])
            | (attacks_bb(square, Occupancies[BOTH], BISHOP) & Pieces[attackingSide][BISHOP])
            | (attacks_bb(square, Occupancies[BOTH], ROOK) & Pieces[attackingSide][ROOK])
            | (attacks_bb(square, Occupancies[BOTH], QUEEN) & Pieces[attackingSide][QUEEN])
            | (attacks_bb(square, Occupancies[BOTH], KING) & Pieces[attackingSide][KING]);
    }

    void print_attacked_squares(Color side) {
        std::string output = "\n +---+---+---+---+---+---+---+---+\n";
        std::string pieceToChar = "PNBRQKpnbrqk";

        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file <= 7; file++) {
                int square = file + (rank * 8);
                output += " | ";

                output += (is_square_attacked((Square)square, side)) ? "1" : " ";
            }

            output += " | " + std::to_string(rank + 1) + "\n +---+---+---+---+---+---+---+---+\n";
        }

        output += "   a   b   c   d   e   f   g   h\n\n";

        std::cout << output << std::endl;
    }
}


