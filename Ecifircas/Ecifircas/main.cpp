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

    set_board("r3kbnr/ppp2ppp/2n1b3/3pp1q1/3PP1Q1/2N1B3/PPP2PPP/R3KBNR w KQkq - 4 6"); // "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
    print_board();

    Moves moves;
    generate_moves(moves);

    moves.print_moves();

    for (int i = 0; i < moves.get_count(); i++) {
        Move move = moves.get_move(i);
        if (move.flags() == QUEEN_CASTLE) {
            make_move(moves.get_move(i), ALL_MOVES);
            print_board();
        }
    }

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
