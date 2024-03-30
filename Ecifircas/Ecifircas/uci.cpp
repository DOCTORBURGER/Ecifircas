#include "uci.h"

#include <iostream>
#include <string>
#include <sstream>

#include "types.h"
#include "movegen.h"
#include "board.h"
#include "perft.h"

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
            }
            else if (token == "position") {
                position(iss);
            }
            else if (token == "go") {
                Moves moves;
                generate_moves(moves);
                moves.get_move(0);
                std::string promoString = "nbrq";
                size_t index = moves.get_move(0).flags() & 0x3;
                std::string promoChar = (index < promoString.size()) ? std::string(1, promoString[index]) : std::string(1, '\0');
                std::cout << "bestmove " << SquareToCoordinates[moves.get_move(0).from_sq()] <<
                    SquareToCoordinates[moves.get_move(0).to_sq()] << std::endl;
            }
            else if (token == "perft") {
                int depth;
                if (iss >> depth) {
                    perft_test(depth);
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

            }
        }
    }

	std::string UCI::square(Square s) {

	}

	std::string UCI::move(Move m) {

	}

    Move UCI::to_move(std::string& str)
    {
        return Move();
    }
}