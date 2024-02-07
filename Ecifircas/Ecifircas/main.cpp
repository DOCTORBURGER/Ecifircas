#include <iostream>
#include <string>
#include "board.h"

using namespace Ecifricas;

int main()
{
    std::string inputLine;
    bool running = true;

    initializeBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

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
        else {
            
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
