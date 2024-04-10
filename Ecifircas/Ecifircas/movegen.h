#pragma once

#include <iostream>
#include <string>

#include "types.h"
#include "board.h"

namespace Ecifircas {
	class Moves
	{
	public:
		Moves()
		{
			count = 0;
		}

		inline void add_move(Move move)
		{
			moves[count] = move;
			count++;
		}

		inline int get_count()
		{
			return count;
		}

		inline Move get_move(int index)
		{
			return moves[index];
		}

		inline void swap(int i, int j)
		{
			Move temp = moves[i];
			moves[i] = moves[j];
			moves[j] = temp;
		}

	protected:
		Move moves[256];
		int count;
	};

	void generate_moves(Moves& moveList);
}
