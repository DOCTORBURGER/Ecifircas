#include <iostream>
#include <string>

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

    set_board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"); // "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"

    while (running) {
        if (!std::getline(std::cin, inputLine)) {
            break; 
        }

        if (inputLine == "uci") {
            std::cout << "id name Ecifircas" << std::endl;
            std::cout << "id author Jonah Bogusch" << std::endl;

            std::cout << "uciok" << std::endl;
        }
        else if (inputLine == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (inputLine == "quit") {
            running = false;
        }
        else if (inputLine == "d") {
            print_board();
        }
        else if (inputLine == "perft") {
            perft_test(4);
        }
        else {

        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
