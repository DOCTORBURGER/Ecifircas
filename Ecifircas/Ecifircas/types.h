#pragma once

#include <cstdint>

namespace Ecifircas {
	using U64 = uint64_t;
	using Bitboard = uint64_t;

	enum Square : int {
		A1, B1, C1, D1, E1, F1, G1, H1,
		A2, B2, C2, D2, E2, F2, G2, H2,
		A3, B3, C3, D3, E3, F3, G3, H3,
		A4, B4, C4, D4, E4, F4, G4, H4,
		A5, B5, C5, D5, E5, F5, G5, H5,
		A6, B6, C6, D6, E6, F6, G6, H6,
		A7, B7, C7, D7, E7, F7, G7, H7,
		A8, B8, C8, D8, E8, F8, G8, H8
	};

	inline Square& operator++(Square& square) {
		square = static_cast<Square>(static_cast<int>(square) + 1);
		return square;
	}

	inline Square operator++(Square& square, int) {
		Square temp = square;
		++square;
		return temp;
	}

	enum Direction : int {
		NORTH = 8,
		EAST = 1,
		SOUTH = -NORTH,
		WEST = -EAST,

		NORTH_EAST = NORTH + EAST,
		SOUTH_EAST = SOUTH + EAST,
		SOUTH_WEST = SOUTH + WEST,
		NORTH_WEST = NORTH + WEST
	};

	enum Piece : int {
		PAWN, 
		KNIGHT, 
		BISHOP, 
		ROOK, 
		QUEEN, 
		KING,
		EMPTY = 6
	};

	inline Piece& operator++(Piece& piece) {
		piece = static_cast<Piece>(static_cast<int>(piece) + 1);
		return piece;
	}

	inline Piece operator++(Piece& piece, int) {
		Piece temp = piece;
		++piece; 
		return temp;
	}

	enum Color : int {
		WHITE = 0,
		BLACK = 1
	};

	inline Color& operator++(Color& color) {
		color = static_cast<Color>(static_cast<int>(color) + 1);
		return color;
	}

	inline Color operator++(Color& color, int) {
		Color temp = color;
		++color; 
		return temp;
	}

	// We may need to change this later, either to a short for efficiency, or add more fields for promotion
	typedef struct {
		int start;
		int dest;
	} move;
}