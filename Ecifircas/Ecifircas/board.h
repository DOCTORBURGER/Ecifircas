#pragma once

#include <string>

#include "types.h"

namespace Ecifircas {
	void set_board(std::string fen);
	void print_board();
	bool is_square_attacked(Square square, Color side);
}
