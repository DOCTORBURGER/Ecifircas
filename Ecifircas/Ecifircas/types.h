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
		A8, B8, C8, D8, E8, F8, G8, H8, NO_SQUARE
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
		BLACK = 1,
		BOTH = 2
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

	enum Castle : int {
		WHITE_KINGSIDE = 1,
		WHITE_QUEENSIDE = 2,
		BLACK_KINGSIDE = 4,
		BLACK_QUEENSIDE = 8
	};

	enum QuiescenceMoveFlag : int
	{
		ALL_MOVES = 0,
		ONLY_CAPTURES = 1
	};

	enum MoveFlags : int
	{
		QUIET_MOVE = 0,
		DOUBLE_PAWN_PUSH = 1,
		KING_CASTLE = 2,
		QUEEN_CASTLE = 3,
		CAPTURE = 4,
		EP_CAPTURE = 5,
		KNIGHT_PROMO = 8, // Jump in size here for bits (1000), ints are "codes"
		BISHOP_PROMO = 9,
		ROOK_PROMO = 10,
		QUEEN_PROMO = 11,
		KNIGHT_PROMO_CAPTURE = 12,
		BISHOP_PROMO_CAPTURE = 13,
		ROOK_PROMO_CAPTURE = 14,
		QUEEN_PROMO_CAPTURE = 15
	};

	// 0000 0000 0000 0011 1111 source square
	// 0000 0000 1111 1100 0000 target square
	// 0000 0111 0000 0000 0000 piece type
	// 0000 1000 0000 0000 0000 color
	// 1111 0000 0000 0000 0000 flag
	// Used this flag structure: https://www.chessprogramming.org/Encoding_Moves
	// Took operator inspiration from stockfish
	class Move
	{
	public:
		Move() : data(0) {}

		Move(Square source, Square dest, Piece piece, Color color, MoveFlags flags)
		{
			data = (flags & 0xf) << 16 | (color) << 15 | (piece & 0x7) << 12
				| (dest & 0x3f) << 6 | (source & 0x3f);
		}

		constexpr Square from_sq()	const { return Square(data & 0x3f); }

		constexpr Square to_sq()	const { return Square((data >> 6) & 0x3f); }

		constexpr int to_from_sq()	const { return data & 0xfff; }

		constexpr MoveFlags flags() const { return MoveFlags((data >> 16) & 0xf); }

		constexpr Piece get_piece() const { return Piece((data >> 12) & 0x7); }

		constexpr Color get_color() const { return Color(data >> 15 & 0x1); }

		constexpr bool operator==(const Move& m) const { return data == m.data; }
		constexpr bool operator!=(const Move& m) const { return data != m.data; }

		constexpr explicit operator bool() const { return data != 0; }
	protected:
		int data;
	};
}