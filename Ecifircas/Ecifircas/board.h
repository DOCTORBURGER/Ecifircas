#pragma once

#include <string>

#include "types.h"

namespace Ecifircas {
	extern Color SideToMove;
	extern Bitboard Pieces[2][6];
	extern Bitboard Occupancies[3];
	extern int CastleRights;
	extern Square EnpassantSquare;
	extern const std::string SquareToCoordinates[];
	
	void set_board(std::string fen);
	void print_board();
	void print_attacked_squares(Color side);
	bool is_square_attacked(Square square, Color side);
}
