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

	std::string Fens[7] = {
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1",
		"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
	};

	int Results[7][5] = {
		{ 20, 400, 8902, 197281, 4865609 },
		{ 48, 2039, 97862, 4085603, 193690690 },
		{ 14, 191, 2812, 43238, 674624 },
		{ 6, 264, 9467, 422333, 15833292 },
		{ 6, 264, 9467, 422333, 15833292 },
		{ 44, 1486, 62379, 2103487, 89941194 },
		{ 46, 2079, 89890, 3894594, 164075551 }
	};

	void perft_test_suite()
	{
		for (int fen = 0; fen < 7; fen++) {
			set_board(Fens[fen]);
			std::cout << "Position #" << fen + 1 << " " << Fens[fen] << std::endl;;

			for (int depth = 0; depth < 5; depth++) {
				std::string result = (perft_test(depth + 1, false) == Results[fen][depth]) ? "Passed" : "Failed";

				std::cout << "    depth " << depth + 1 << " test: " << result << std::endl;
			}
		}

        set_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	}
}

