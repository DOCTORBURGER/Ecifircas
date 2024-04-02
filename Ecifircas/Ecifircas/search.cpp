#include "search.h"

#include "types.h"
#include "movegen.h"
#include "board.h"

namespace Ecifircas {
	Move search_placeholder() {
		Moves moves;
		Move selectedMove;

		generate_moves(moves);

		for (int i = 0; i < moves.get_count(); i++) {
			selectedMove = moves.get_move(i);

			if (make_move(selectedMove, ALL_MOVES)) {
				break;
			}
		}
		
		return selectedMove;
	}
}