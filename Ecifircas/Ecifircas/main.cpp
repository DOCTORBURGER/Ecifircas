#include <iostream>
#include <string>
#include <chrono>

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

    set_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); // "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"

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
            auto start = std::chrono::high_resolution_clock::now();

            Nodes = 0;
            perft_driver(7);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            std::cout << "Nodes: " << Nodes << std::endl;
            std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;
        }
        else {

        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
