#include <iostream>
#include <string>
#include <sstream>
#include <random>

#include "types.h"
#include "board.h"
#include "bitboard.h"
#include "movegen.h"
#include "perft.h"

using namespace Ecifircas;

int main()
{
    std::string inputLine;
    bool running = true;
    initialize_bitboards();

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
            else {

            }

            if (token == "moves") {
                while (iss >> token) {
                    
                }
            }
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

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
