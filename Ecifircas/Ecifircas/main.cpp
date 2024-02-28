#include <iostream>
#include <string>

#include "board.h"
#include "bitboard.h"
#include "movegen.h"

using namespace Ecifircas;

int main()
{
    std::string inputLine;
    bool running = true;
    initialize_bitboards();

    set_board("r3kbnr/ppp1pppp/2n5/3p1Q2/3P1q2/2N5/PPP1PPPP/R3KBNR w KQkq - 0 6"); // "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
    generate_moves();
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
        else {

        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
