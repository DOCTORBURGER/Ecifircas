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

    set_board("r6r/p1ppkpb1/b1n1pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQ - 0 2"); // "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
    print_board();

    Moves moves;
    generate_moves(moves);
    int legalMoves = 0;
    for (int i = 0; i < moves.get_count(); i++) {
        Move move = moves.get_move(i);

        copy_board();

        if (!make_move(move, ALL_MOVES)) {
            continue;
        }
        legalMoves++;
        
        if (move.flags() == KING_CASTLE || move.flags() == QUEEN_CASTLE || move.get_piece() == ROOK) {
            print_board();
        }


        restore_from_copy();
    }
    std::cout << "Legal moves: " << legalMoves << std::endl;

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
