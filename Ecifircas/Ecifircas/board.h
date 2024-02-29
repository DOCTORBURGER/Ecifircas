#pragma once

#include <string>
#include <cstring> // For std::memcpy

#include "types.h"

namespace Ecifircas {
	extern Color SideToMove;
	extern Bitboard Pieces[2][6];
	extern Bitboard Occupancies[3];
	extern int CastleRights;
	extern Square EnpassantSquare;
	extern const std::string SquareToCoordinates[];

	// Copy / restore board macros
	#define copy_board()																						\
		Bitboard bitboardsCopy[2][6], occupanciesCopy[3];														\
		Color sideToMoveCopy;																					\
		Square enpassantSquareCopy;																				\
		int castleRightsCopy;																					\
		std::memcpy(bitboardsCopy, Pieces, sizeof(Pieces));														\
		std::memcpy(occupanciesCopy, Occupancies, sizeof(Occupancies));											\
		sideToMoveCopy = SideToMove, enpassantSquareCopy = EnpassantSquare, castleRightsCopy = CastleRights;	\

	#define restore_from_copy()																					\
		std::memcpy(Pieces, bitboardsCopy, sizeof(bitboardsCopy));												\
		std::memcpy(Occupancies, occupanciesCopy, sizeof(occupanciesCopy));										\
		SideToMove = sideToMoveCopy, EnpassantSquare = enpassantSquareCopy, CastleRights = castleRightsCopy;	\
	
	void set_board(std::string fen);
	void print_board();
	void print_attacked_squares(Color side);
	bool is_square_attacked(Square square, Color side);
}
