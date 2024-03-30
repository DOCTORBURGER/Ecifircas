#include "search.h"

#include "types.h"
#include "movegen.h"

namespace Ecifircas {
	Move search_placeholder() {
		Moves moves;

		generate_moves(moves);
		
		return moves.get_move(0);
	}
}