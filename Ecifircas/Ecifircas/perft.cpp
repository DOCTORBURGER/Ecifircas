#include <iostream>
#include <chrono>
#include <string>

#include "board.h"
#include "types.h"
#include "perft.h"
#include "movegen.h"

namespace Ecifircas
{
	long long Nodes;

	void perft_driver(int depth)
	{
		if (depth == 0) {
			Nodes++;
			return;
		}
		
		Moves moves;

		generate_moves(moves);

		for (int curMoveIndex = 0; curMoveIndex < moves.get_count(); curMoveIndex++) {
			copy_board();

			if (!make_move(moves.get_move(curMoveIndex), ALL_MOVES)) {
				continue;
			}

			perft_driver(depth - 1);

			restore_from_copy();
		}
	}

	long long perft_test(int depth, bool output)
	{
		Nodes = 0;
		if (output) {
			std::cout << "\n    Performance Test\n\n" << std::endl;
		}

		Moves moves;

		generate_moves(moves);

		std::string promoString = "nbrq";

		auto start = std::chrono::high_resolution_clock::now();

		for (int curMoveIndex = 0; curMoveIndex < moves.get_count(); curMoveIndex++) {
			copy_board();

			if (!make_move(moves.get_move(curMoveIndex), ALL_MOVES)) {
				continue;
			}

			long cummulativeNodes = Nodes;

			perft_driver(depth - 1);

			long oldNodes = Nodes - cummulativeNodes;

			restore_from_copy();
			
			if (output) {
				size_t index = moves.get_move(curMoveIndex).flags() & 0x3;
				std::string promoChar = (index < promoString.size()) ? std::string(1, promoString[index]) : std::string(1, '\0');

				std::cout << SquareToCoordinates[moves.get_move(curMoveIndex).from_sq()] <<
					SquareToCoordinates[moves.get_move(curMoveIndex).to_sq()] << 
					((moves.get_move(curMoveIndex).flags() & 0x8) ? promoChar : "")
					<< ": " << oldNodes << std::endl;
			}
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		if (output) {
			std::cout << "Nodes: " << Nodes << std::endl;
			std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;
		}

		return Nodes;
	}

	void perft_test_suite()
	{

	}
}

