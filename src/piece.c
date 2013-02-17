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
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:   alex.dantas92@gmail.com
 */

#include "engine.h"
#include "piece.h"
#include "pieces_define.h"
#include "block.h"
#include "board.h"

piece_s new_piece(piece_e type)
{
//	if ((type < 0) || (type >= PIECE_MAX))
//		exit(0); /* Isn't that a bit rough? */

	piece_s p;
	color_e color;

	/* user specifies if the piece has colors */
	if (global.theme_piece_has_colors)
		color = piece_get_color(type);
	else
		color = engine_get_color(WHITE_BLACK, false);
	p.color = color;

	/* user specifies the piece appearance */
	if (global.theme_piece[0] != '\0')
	{
		p.theme[0] = global.theme_piece[0];
		p.theme[1] = global.theme_piece[1];
	}
	else
	{
		p.theme[0] = ' ';
		p.theme[1] = ' ';
	}
	p.theme[2] = '\0';

	p.rotation = 0;
	p.type     = type;

	/* If we're creating a dummy piece, stop. It won't be printed anyway */
	if (!piece_is_valid(&p))
		return p;

	/* The starting positions are derived from the SRS */
	p.x = BOARD_WIDTH/2 + global_pieces_position[p.type][p.rotation][0];
	p.y = global_pieces_position[p.type][p.rotation][1];

	/* This seems complicated, but its just initializing each
	 * block of the piece according to it's X and Y on the board.
	 * Remember that each block has its X and Y relative to the start
	 * of the board.
	 */
	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p.type][p.rotation][j][i] != 0)
			{
				int block_x  = p.x + i;
				int block_y  = p.y + j;

				p.block[k] = new_block(block_x, block_y, p.theme, p.color);

				if (global.theme_show_pivot_block)
					if (global_pieces[p.type][p.rotation][j][i] == 2)
						p.block[k] = new_block(block_x, block_y, p.theme, engine_get_color(WHITE_WHITE, true));
				k++;
			}
	return p;
}

/** Rotate piece #p by #rotation times. Negative number rotates backwards
 *  Doesnt check if the piece will be valid later.
 */
void piece_rotate(piece_s* p, int rotation)
{
	/* rotating to the negative side (counter-clockwise) */
	if (rotation < 0)
		rotation += 4;

	/* keep it under 4 */
	p->rotation = (p->rotation + rotation) % 4;

	/* refreshing the piece's blocks */
	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				int block_x  = p->x + i;
				int block_y  = p->y + j;

				p->block[k] = new_block(block_x, block_y, p->theme, p->color);

				if (global.theme_show_pivot_block)
					if (global_pieces[p->type][p->rotation][j][i] == 2)
						p->block[k] = new_block(block_x, block_y, p->theme, engine_get_color(WHITE_WHITE, true));
				k++;
			}
}


/** Tries to rotate the piece, doing nothing if cant.
 *   @return true if succeeded, false if failed
 *
 *   This function acts according to the SRS (super rotation system).
 *   It's described on http://tetrisconcept.net/wiki/SRS
 */
bool piece_rotate_if_possible(piece_s* p, board_s* b, int rotation)
{
	piece_s tmp = *p;

	/* Here's the simplest case - the piece rotates just fine */
	piece_rotate(&tmp, rotation);
	if (piece_is_on_valid_position(&tmp, b))
	{
		piece_rotate(p, rotation);
		return true;
	}

	/* the SRS depends on several informations about the piece */
	/* first, the type */
	int type; int rot_num;
	if (p->type == PIECE_I)
	{
		type    = 1;
		rot_num = p->rotation;
	}
	else if (p->type == PIECE_O)
	{
		return true;
	}
	else
	{
		type = 0;
		rot_num = tmp.rotation;
	}

	/* then if the player wants to rotate it clockwise or counter */
	int rot_way;
	if (rotation > 0)
		rot_way = 0;
	else
		rot_way = 1;

	/* This will make 5 tests, trying to move the piece around.
	 * If any of them pass, the piece is immediately moved.
	 * If none of them pass, well... Lets say we're out of luck.
	 */
	int i;
	for (i = 0; i < 5; i++)
	{
		char dx = srs_possible_positions[type][rot_way][rot_num][i][0];
		char dy = srs_possible_positions[type][rot_way][rot_num][i][1];
		int  k;

		/* Remember, the piece #tmp has already been rotated.
		 * Here we just try to move it, making it preform wall/floor kicks */
		tmp.x += dx;
		tmp.y += dy;
		for (k = 0; k < 4; k++)
		{
			tmp.block[k].x += dx;
			tmp.block[k].y += dy;
		}

		/* alright, if it worked for #tmp, it will work for #p */
		if (piece_is_on_valid_position(&tmp, b))
		{
			piece_rotate(p, rotation);
			p->x += dx;
			p->y += dy;
			for (k = 0; k < 4; k++)
			{
				p->block[k].x += dx;
				p->block[k].y += dy;
			}
			return true;
		}

		/* well, it didnt work, so lets return #tmp to its initial position */
		tmp.x -= dx;
		tmp.y -= dy;
		for (k = 0; k < 4; k++)
		{
			tmp.block[k].x -= dx;
			tmp.block[k].y -= dy;
		}
	}
	return false;
}

/** Tries to move the piece, doing nothing if cant.
 *  @return true if succeeded, false if failed
 */
bool piece_move_if_possible(piece_s* p, board_s* b, direction_e dir)
{
	if (piece_can_move(p, b, dir))
	{
		piece_move(p, dir);
		return true;
	}
	return false;
}

/** Moves piece according to #direction. 0 is right and 1 is left */
void piece_move(piece_s* p, direction_e dir)
{
	int delta_x = 0;
	int delta_y = 0;

	switch (dir)
	{
	case DIR_LEFT:  delta_x = -1; break;
	case DIR_RIGHT: delta_x =  1; break;
	case DIR_DOWN:  delta_y =  1; break;
	case DIR_UP:    delta_y = -1; break;
	default: return;
	}

	p->x += delta_x;
	p->y += delta_y;

	int k;
	for (k = 0; k < 4; k++)
	{
		p->block[k].x += delta_x;
		p->block[k].y += delta_y;
	}
}

/** Drops piece as far as possible */
void piece_hard_drop(piece_s* p, board_s* b)
{
	while (piece_can_move(p, b, DIR_DOWN))
		piece_move(p, DIR_DOWN);
}

/** Returns a random integer between #min and #max */
int random_number_between(int min, int max)
{
	if (min > max) { int tmp = min; min = max; max = tmp; }

	return rand() % (max - min + 1) + min;
}

/** Returns a random piece type.
 *
 *  The logic is to keep a bag with one of each piece (7 pieces total).
 *  Then we take one after another in a random order.
 *  As soon as we took them all, place them back on the bag and redo it.
 *  This way, we avoid long sequences of the same piece and guarantee
 *  a certain degree of piece rotativity (I WANT TEH LINES!11!!).
 */
piece_e piece_get_random()
{
 	int i,j;
 	int size = PIECE_MAX;

 	static int bag[PIECE_MAX];
	static int piece = PIECE_MAX;

	if (piece >= PIECE_MAX)
	{
		piece = 0;

		bag[0] = random_number_between(0, size - 1);
		for (i = 1; i < size; i++)
		{
			bag[i] = random_number_between(0, size - 1);
			j = 0;
			do {				///for (j = 0; j < i; j++)

				if (bag[j] == bag[i])
				{
					bag[i] = random_number_between(0, size - 1);
					j = 0;
				}
				else
					j++;
			} while (j < i);
		}
	}
	piece++;
	return bag[piece - 1];
}

/** Checks if the piece #p can move to direction #dir.
 *  The thing here is we create a 'dummy' piece, move it and then
 *  tests if it's on a right spot.
 */
bool piece_can_move(piece_s* p, board_s* b, direction_e dir)
{
	piece_s tmp = *p;

	piece_move(&tmp, dir);
	return piece_is_on_valid_position(&tmp, b);
}

/** Returns if the piece is colliding with something or if it's off-limits */
bool piece_is_on_valid_position(piece_s* p, board_s* b)
{
	/* Going through the board only where the piece's at */
	int k;
	for (k = 0; k < 4; k++)
	{
		/* Here we don't confuse our 'dummy' blocks with real ones */
		p->block[k].type = EMPTY;

		/* block's x and y are not relative to the piece -- they're global */
		int block_x = p->block[k].x;
		int block_y = p->block[k].y;

		/* Off-limits check */
		if ((block_x >= BOARD_WIDTH) || (block_y >= BOARD_HEIGHT) ||
			(block_x < 0))
			return false;

		/* If the piece is still out of the board, we don't check collision */
		if (block_y < 0) continue;

		/* Fellow blocks check */
		if (b->block[block_x][block_y].type != EMPTY)
			return false;
	}
	return true;
}

/** Tells if a piece is a basic one (one of the 7 original ones) */
bool piece_is_valid(piece_s* p)
{
	/* p is always positive */
	if (p->type >= PIECE_MAX)
		return false;
	else
		return true;
}

/** Returns the color according to the piece type */
int piece_get_color(piece_e type)
{
	int color;
	switch(type)
	{
	case PIECE_S: color = engine_get_color(WHITE_GREEN,   false); break;
	case PIECE_Z: color = engine_get_color(WHITE_RED,     false); break;
	case PIECE_O: color = engine_get_color(WHITE_YELLOW,  true);  break;
	case PIECE_I: color = engine_get_color(WHITE_CYAN,    true);  break;
	case PIECE_L: color = engine_get_color(WHITE_YELLOW,  false); break;
	case PIECE_J: color = engine_get_color(WHITE_BLUE,    false); break;
	case PIECE_T: color = engine_get_color(WHITE_MAGENTA, false); break;
	default:      color = engine_get_color(WHITE_BLACK,   false); break;
	}
	return color;
}

