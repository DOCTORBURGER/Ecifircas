#include "types.h"
#include <iostream>
#include <cstdint>
#include <string>
#include <array>

#include "bitboard.h"

namespace Ecifircas {
    Bitboard PawnAttacks[2][64];
    Bitboard PseudoAttacks[6][64];

	int RookOccupancyBits[64] = {
		12, 11, 11, 11, 11, 11, 11, 12,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		12, 11, 11, 11, 11, 11, 11, 12
	};

	int BishopOccupancyBits[64] = {
		6, 5, 5, 5, 5, 5, 5, 6,
		5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 7, 7, 7, 7, 5, 5,
		5, 5, 7, 9, 9, 7, 5, 5,
		5, 5, 7, 9, 9, 7, 5, 5,
		5, 5, 7, 7, 7, 7, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5,
		6, 5, 5, 5, 5, 5, 5, 6
	};

    // to mask relevant squares of both lines (no outer squares)
    U64 BishopMasks[64];
    U64 RookMasks[64];

    // [square][occupancy]
    U64 BishopAttacks[64][512];
    U64 RookAttacks[64][4096];

    // magic 64-bit factor
    // Took inspiration and magics from this approach: https://github.com/maksimKorzh/chess_programming/blob/master/src/magics/magics.c
    // I may come back to generate my own magics later on
    const Bitboard BishopMagics[64] = {
        0x89a1121896040240ULL,
        0x2004844802002010ULL,
        0x2068080051921000ULL,
        0x62880a0220200808ULL,
        0x4042004000000ULL,
        0x100822020200011ULL,
        0xc00444222012000aULL,
        0x28808801216001ULL,
        0x400492088408100ULL,
        0x201c401040c0084ULL,
        0x840800910a0010ULL,
        0x82080240060ULL,
        0x2000840504006000ULL,
        0x30010c4108405004ULL,
        0x1008005410080802ULL,
        0x8144042209100900ULL,
        0x208081020014400ULL,
        0x4800201208ca00ULL,
        0xf18140408012008ULL,
        0x1004002802102001ULL,
        0x841000820080811ULL,
        0x40200200a42008ULL,
        0x800054042000ULL,
        0x88010400410c9000ULL,
        0x520040470104290ULL,
        0x1004040051500081ULL,
        0x2002081833080021ULL,
        0x400c00c010142ULL,
        0x941408200c002000ULL,
        0x658810000806011ULL,
        0x188071040440a00ULL,
        0x4800404002011c00ULL,
        0x104442040404200ULL,
        0x511080202091021ULL,
        0x4022401120400ULL,
        0x80c0040400080120ULL,
        0x8040010040820802ULL,
        0x480810700020090ULL,
        0x102008e00040242ULL,
        0x809005202050100ULL,
        0x8002024220104080ULL,
        0x431008804142000ULL,
        0x19001802081400ULL,
        0x200014208040080ULL,
        0x3308082008200100ULL,
        0x41010500040c020ULL,
        0x4012020c04210308ULL,
        0x208220a202004080ULL,
        0x111040120082000ULL,
        0x6803040141280a00ULL,
        0x2101004202410000ULL,
        0x8200000041108022ULL,
        0x21082088000ULL,
        0x2410204010040ULL,
        0x40100400809000ULL,
        0x822088220820214ULL,
        0x40808090012004ULL,
        0x910224040218c9ULL,
        0x402814422015008ULL,
        0x90014004842410ULL,
        0x1000042304105ULL,
        0x10008830412a00ULL,
        0x2520081090008908ULL,
        0x40102000a0a60140ULL,
    };

    const U64 RookMagics[64] = {
        0xa8002c000108020ULL,
        0x6c00049b0002001ULL,
        0x100200010090040ULL,
        0x2480041000800801ULL,
        0x280028004000800ULL,
        0x900410008040022ULL,
        0x280020001001080ULL,
        0x2880002041000080ULL,
        0xa000800080400034ULL,
        0x4808020004000ULL,
        0x2290802004801000ULL,
        0x411000d00100020ULL,
        0x402800800040080ULL,
        0xb000401004208ULL,
        0x2409000100040200ULL,
        0x1002100004082ULL,
        0x22878001e24000ULL,
        0x1090810021004010ULL,
        0x801030040200012ULL,
        0x500808008001000ULL,
        0xa08018014000880ULL,
        0x8000808004000200ULL,
        0x201008080010200ULL,
        0x801020000441091ULL,
        0x800080204005ULL,
        0x1040200040100048ULL,
        0x120200402082ULL,
        0xd14880480100080ULL,
        0x12040280080080ULL,
        0x100040080020080ULL,
        0x9020010080800200ULL,
        0x813241200148449ULL,
        0x491604001800080ULL,
        0x100401000402001ULL,
        0x4820010021001040ULL,
        0x400402202000812ULL,
        0x209009005000802ULL,
        0x810800601800400ULL,
        0x4301083214000150ULL,
        0x204026458e001401ULL,
        0x40204000808000ULL,
        0x8001008040010020ULL,
        0x8410820820420010ULL,
        0x1003001000090020ULL,
        0x804040008008080ULL,
        0x12000810020004ULL,
        0x1000100200040208ULL,
        0x430000a044020001ULL,
        0x280009023410300ULL,
        0xe0100040002240ULL,
        0x200100401700ULL,
        0x2244100408008080ULL,
        0x8000400801980ULL,
        0x2000810040200ULL,
        0x8010100228810400ULL,
        0x2000009044210200ULL,
        0x4080008040102101ULL,
        0x40002080411d01ULL,
        0x2005524060000901ULL,
        0x502001008400422ULL,
        0x489a000810200402ULL,
        0x1004400080a13ULL,
        0x4000011008020084ULL,
        0x26002114058042ULL,
    };

	void print_bitboard(Bitboard bb) 
    {
		std::string output = "\n +---+---+---+---+---+---+---+---+\n";

		for (int rank = 7; rank >= 0; rank--) {
			for (int file = 0; file <= 7; file++) {
				output += " | ";
				int square = file + (rank * 8);

				output += get_bit(bb, (Square)(square)) ? "1" : " ";
			}

			output += " | " + std::to_string(rank + 1) + "\n +---+---+---+---+---+---+---+---+\n";
		}

		output += "   a   b   c   d   e   f   g   h\n";

		std::cout << output << std::endl;
	}

	Bitboard knight_shift(Bitboard bb, int shift) 
    {
		return	shift == 17		? (bb & ~FileH) << 17
			:	shift == 15		? (bb & ~FileA) << 15
			:	shift == 10		? (bb & ~FileH & ~FileG) << 10
			:	shift == 6		? (bb & ~FileA & ~FileB) << 6
			:	shift == -6		? (bb & ~FileH & ~FileG) >> 6
			:	shift == -10	? (bb & ~FileA & ~FileB) >> 10
			:	shift == -15	? (bb & ~FileH) >> 15
			:	shift == -17	? (bb & ~FileA) >> 17
								: 0;
	}

    Bitboard sliding_attacks_on_the_fly(Square square, Piece pieceType, Bitboard block) 
    {
        if (pieceType != ROOK && pieceType != BISHOP) { return 0ULL; }

        Bitboard squareBitboard = get_square_bb(square);
        Bitboard attacks = 0ULL;

        // Define direction arrays for each piece
        const Direction bishopDirections[] = { NORTH_WEST, NORTH_EAST, SOUTH_WEST, SOUTH_EAST };
        const Direction rookDirections[] = { NORTH, SOUTH, EAST, WEST };

        // Use a pointer to refer to the correct array
        const Direction* directions = (pieceType == BISHOP) ? bishopDirections : rookDirections;

        for (int i = 0; i < 4; i++) {
            Direction direction = directions[i];
            for (Bitboard nextSquare = shift_bit(squareBitboard, direction); nextSquare != 0; nextSquare = shift_bit(nextSquare, direction)) {
                attacks |= nextSquare;
                if (block & nextSquare) break;
            }
        }

        return attacks;
    }

    Bitboard set_occupancy(int index, int bits, Bitboard mask) 
    {
        Bitboard occupancy = 0ULL;

        for (int count = 0; count < bits; count++) {
            Square square = (Square)get_ls1b_index(mask);

            pop_bit(mask, square);

            if (index & (1 << count))
                occupancy |= (1ULL << square);
        }

        return occupancy;
    }

    Bitboard get_bishop_attacks(Square square, Bitboard occupancy) 
    {
        occupancy &= BishopMasks[square];
        occupancy *= BishopMagics[square];
        occupancy >>= 64 - BishopOccupancyBits[square];

        return BishopAttacks[square][occupancy];
    }

    Bitboard get_rook_attacks(Square square, Bitboard occupancy) 
    {
        occupancy &= RookMasks[square];
        occupancy *= RookMagics[square];
        occupancy >>= 64 - RookOccupancyBits[square];

        return RookAttacks[square][occupancy];
    }

	void initialize_bitboards() 
    {
		for (Square square = A1; square <= H8; square++) {
            Bitboard squareBitboard = get_square_bb(square);

			PawnAttacks[WHITE][square] = pawn_attacks_bb(squareBitboard, WHITE);
			PawnAttacks[BLACK][square] = pawn_attacks_bb(squareBitboard, BLACK);

			for (int moveOffset : { (int)NORTH, (int)SOUTH, (int)EAST, (int)WEST, (int)NORTH_WEST, (int)NORTH_EAST, (int)SOUTH_WEST, (int)SOUTH_EAST }) 
                PseudoAttacks[KING][square] |= shift_bit(squareBitboard, (Direction)moveOffset);

			for (int shift : { 17, 15, 10, 6, -6, -10, -15, -17 })
                PseudoAttacks[KNIGHT][square] |= knight_shift(squareBitboard, shift);

            PseudoAttacks[BISHOP][square] = sliding_attacks_on_the_fly(square, BISHOP, 0ULL);
            PseudoAttacks[ROOK][square] = sliding_attacks_on_the_fly(square, ROOK, 0ULL);

            PseudoAttacks[QUEEN][square] = (PseudoAttacks[ROOK][square] | PseudoAttacks[BISHOP][square]);

            Bitboard occupancyEdgeMask = 0ULL;

            if (!get_bit(FileA, square)) { occupancyEdgeMask |= FileA; }
            if (!get_bit(FileH, square)) { occupancyEdgeMask |= FileH; }
            if (!get_bit(Rank1, square)) { occupancyEdgeMask |= Rank1; }
            if (!get_bit(Rank8, square)) { occupancyEdgeMask |= Rank8; }
            
            BishopMasks[square] = PseudoAttacks[BISHOP][square] & ~occupancyEdgeMask;
            RookMasks[square] = PseudoAttacks[ROOK][square] & ~occupancyEdgeMask;

            Bitboard bishopMask = BishopMasks[square];
            Bitboard rookMask = RookMasks[square];
             
            int bishopMaskCount = count_bits(bishopMask);
            int rookMaskCount = count_bits(rookMask);

            int bishopOccupancyVariations = 1 << bishopMaskCount; // This is basically saying 2^count
            int rookOccupancyVariations = 1 << rookMaskCount;

            for (int count = 0; count < bishopOccupancyVariations; count++) {
                Bitboard occupancy = set_occupancy(count, bishopMaskCount, bishopMask);
                Bitboard magicIndex = occupancy * BishopMagics[square] >> (64 - BishopOccupancyBits[square]);
                BishopAttacks[square][magicIndex] = sliding_attacks_on_the_fly(square, BISHOP, occupancy);
            }

            for (int count = 0; count < rookOccupancyVariations; count++) {
                Bitboard occupancy = set_occupancy(count, rookMaskCount, rookMask);
                Bitboard magicIndex = occupancy * RookMagics[square] >> (64 - RookOccupancyBits[square]);
                RookAttacks[square][magicIndex] = sliding_attacks_on_the_fly(square, ROOK, occupancy);
            }
		}
	}
}