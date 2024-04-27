#include "uci.h"

#include <iostream>
#include <string>
#include <sstream>

#include "types.h"
#include "movegen.h"
#include "board.h"
#include "perft.h"
#include "search.h"
#include "eval.h"

namespace Ecifircas {

	void UCI::loop() {
		std::string inputLine;
		bool running = true;

        set_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        while (running) {
            if (!std::getline(std::cin, inputLine)) {
                break;
            }

            std::istringstream iss(inputLine);
            std::string token, fen;
            iss >> token;

            if (token == "uci") {
                std::cout << "id name Ecifircas" << std::endl;
                std::cout << "id author Jonah Bogusch" << std::endl;

                std::cout << "uciok" << std::endl;
            }
            else if (token == "isready") {
                std::cout << "readyok" << std::endl;
            }
            else if (token == "quit" || token == "q") {
                running = false;
            }
            else if (token == "d") {
                print_board();
                std::cout << evaluate() << std::endl;
            }
            else if (token == "position") {
                position(iss);
            }
            else if (token == "go") {
                std::cout << "bestmove " << move(search()) << std::endl;
            }
            else if (token == "perft") {
                int depth;
                if (iss >> depth) {
                    perft_test(depth, true);
                }
                else {
                    std::cout << "Error: 'perft' command requires a depth argument." << std::endl;
                }
            }
            else {

            }
        }
	}

    void UCI::go(std::istringstream& iss) {

    }

    void UCI::position(std::istringstream& iss) {
        Move        m;
        std::string token, fen;

        iss >> token;

        if (token == "startpos") {
            set_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            iss >> token;
        }
        else if (token == "fen") {
            while (iss >> token && token != "moves")
                fen += token + " ";

            size_t endpos = fen.find_last_not_of(" \t");
            if (std::string::npos != endpos) {
                fen = fen.substr(0, endpos + 1);
            }

            set_board(fen);
        }

        if (token == "moves") {
            while (iss >> token) {
                std::string t = token;
                Move move = to_move(token);
                if (!(move.from_sq() == A1 && move.to_sq() == A1)) {
                    make_move(move, ALL_MOVES);
                }
            }
        }
    }

	std::string UCI::move(Move m) {
        std::string move = SquareToCoordinates[m.from_sq()] + SquareToCoordinates[m.to_sq()];

        MoveFlags flag = m.flags();

        if (0x8 & flag) { // Promotion Check
            Piece promotedPiece = Piece(KNIGHT + (flag & 0x3));
            move += "pnbrq"[promotedPiece];
        }

        return move;
	}

    Move UCI::to_move(std::string& str)
    {
        Moves moves;
        generate_moves(moves);
        
        for (int i = 0; i < moves.get_count(); i++) {
            std::string moveStr = move(moves.get_move(i));
            if (str == moveStr) {
                return moves.get_move(i);
            }
        }

        return Move(NO_SQUARE, NO_SQUARE, PAWN, WHITE, QUIET_MOVE);
    }
}