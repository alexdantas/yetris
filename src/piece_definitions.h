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

#ifndef PIECE_DEFINITIONS_H_DEFINED
#define PIECE_DEFINITIONS_H_DEFINED

/**
 * Global definition of all the pieces and their possible
 * rotations.
 *
 * We have 7 pieces, 4 rotations to each.
 *
 * To every rotation, there's a 5x5 char array that
 * tells how we should place the block.
 *
 * Their internal values are:
 *
 * - 0: no block
 * - 1: normal block
 * - 2: pivot block (center of rotation)
 */
extern char global_pieces[7][4][5][5];

/**
 * Global definition of the initial position to every piece
 * (how they'll show up on top).
 *
 *  We have 7 pieces, each have 4 rotations and
 *  each have a x anf y offset..
 *
 *  When we initialize the pieces,
 *  we add the first value to the x and the second to the y.
 */
extern char global_pieces_position[7][4][2];

/**
 * Piece's positions, according to the SRS (rotation system).
 *
 * - 2 piece types;
 * - 2 rotation ways (clock and counter);
 * - 4 rotations;
 * - 5 tests;
 * - 2 axes;
 *
 * @warning The tests specified on the SRS has the Y axis inverted
 *          (mine is from top to bottom,
 *          theirs is from bottom to top)
 *
 * The possible rotations are:
 *
 *     0 -> R -> 2 -> L -> 0
 *
 */
extern char srs_possible_positions[2][2][4][5][2];

#endif /* PIECE_DEFINITIONS_H_DEFINED */


