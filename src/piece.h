/* yetris - Tetris(tm) on the console.
 * Copyright (C) 2013 Alexandre Dantas (kure)
 *
 * This file is part of yetris.
 *
 * yetris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://yetris.alexdantas.net
 * mailto:   eu@alexdantas.net
 */

#ifndef PIECE_H_DEFINED
#define PIECE_H_DEFINED

/**
 * @file piece.h
 * Everything related to a single piece (tetramino).
 */

/* How many blocks exists around the piece. Check the global
 * array #pieces -- each piece has 5 x 5 blocks */
#define PIECE_BLOCKS 5

/**
 * Enumerate all possible pieces.
 * @note This order is important.
 * Also, PIECE_MAX tells the biggest number for a visible piece.
 * The others are flags for special pieces.
 */
typedef enum
{
	PIECE_O = 0,
	PIECE_I,
	PIECE_L,
	PIECE_J,
	PIECE_Z,
	PIECE_S,
	PIECE_T,
	PIECE_MAX,
	PIECE_DUMMY

} piece_type_e;

/** Possible directions a piece can move */
typedef enum
{
	DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT

} direction_e;

/* Forward declaration of the board type -- check #board.h */
#ifndef BOARD_S_DEFINED
#define BOARD_S_DEFINED
typedef struct board_s board_s;
#endif

/**
 * A single piece (tetramino) on the game.
 *
 * Instead of each piece having it's blocks inside,
 * we have a global definition of all possible
 * pieces and rotations.
 * @see piece_definitions.h
 *
 * To get a block set, we need to know a piece
 * #type and #rotation.
 *
 * With that in hands, we can show the piece on the
 * screen, applying the piece's #theme to those
 * blocks, based on the piece's #x and #y.
 *
 */
struct piece_s
{
	piece_type_e type;
	block_theme_s* theme; /**< Appearance of the inner blocks */

	int x; /**< Top-left y (@see #piece_definitions.h) */
	int y; /**< Top-left y (@see #piece_definitions.h) */

	short rotation; /* current rotation number (0 is none) */
};

#ifndef PIECE_S_DEFINED
#define PIECE_S_DEFINED
typedef struct piece_s piece_s;
#endif

/**
 * Creates a new piece of #type.
 */
piece_s new_piece(piece_type_e type);

/**
 * Rotate piece #p by #rotation times.
 * Negative number rotates backwards
 *
 * @note After rotating, the piece may be on
 *       an invalid position (overlapping others, and such)!
 */
void piece_rotate(piece_s* p, int rotation);

/**
 * Tries to rotate the piece, doing nothing if cant.
 *
 * @return false (0) if couldn't rotate, true (1) if it could.
 *
 * This function acts according to the SRS (super rotation system).
 * It's described on <http://tetrisconcept.net/wiki/SRS>
 */
bool piece_rotate_if_possible(piece_s* p, board_s* b, int rotation);

/**
 * Moves piece according to #direction.
 */
void piece_move(piece_s* p, direction_e dir);

/**
 * Tries to move the piece, doing nothing if cant.
 *
 * @return false (0) if something weird happened, else true (1).
 */
bool piece_move_if_possible(piece_s* p, board_s* b, direction_e dir);

/**
 * Drops piece as far as possible (to the bottom).
 */
void piece_hard_drop(piece_s* p, board_s* b);

/**
 * Returns a random piece type.
 *
 * This algorithm does way more than simply `srand()`.
 * Check it out.
 */
piece_type_e piece_random_type();


/**
 * Tells if the piece #p can move to direction #dir.
 */
bool piece_can_move(piece_s* p, board_s* b, direction_e dir);

/**
 * Tells if the piece #p is colliding with something
 * or if it's off-limits on board #b.
 */
bool piece_is_on_valid_position(piece_s* p, board_s* b);

/**
 * Tells if #p is a "regular" piece.
 * Well, IDK. What if it gets corrupted or whatnot.
 */
bool piece_is_valid(piece_s* p);

#endif /* PIECE_H_DEFINED */

