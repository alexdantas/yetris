
#include "engine.h"
#include "piece.h"
#include "piece_definitions.h"
#include "board.h"
#include "utils.h"

piece_s new_piece(piece_type_e type)
{
//	if ((type < 0) || (type >= PIECE_MAX))
//		exit(0); /* Isn't that a bit rough? */

	piece_s p;

	/* Deciding the appearance of the blocks */
	p.type = type;

	switch(type)
	{
	case PIECE_S: p.theme = &(global.theme_piece_S);  break;
	case PIECE_Z: p.theme = &(global.theme_piece_Z);  break;
	case PIECE_O: p.theme = &(global.theme_piece_O);  break;
	case PIECE_I: p.theme = &(global.theme_piece_I);  break;
	case PIECE_L: p.theme = &(global.theme_piece_L);  break;
	case PIECE_J: p.theme = &(global.theme_piece_J);  break;
	case PIECE_T: p.theme = &(global.theme_piece_T);  break;
	default:      p.theme = &(global.theme_piece_colorless); break;
	}

	p.rotation = 0;

	/* If we're creating a dummy piece, stop.
	 * It won't be printed anyway */
	if (!piece_is_valid(&p))
		return p;

	/* The starting positions are derived from the SRS */
	p.x = BOARD_WIDTH/2 + global_pieces_position[p.type][p.rotation][0];
	p.y = global_pieces_position[p.type][p.rotation][1];

	return p;
}

void piece_rotate(piece_s* p, int rotation)
{
	/* rotating to the negative side (counter-clockwise) */
	if (rotation < 0)
		rotation += 4;

	/* keep it under 4 */
	p->rotation = (p->rotation + rotation) % 4;
}

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

	/* The SRS depends on several informations about the piece.
	 * First, it's type */
	piece_type_e type;
	int rot_num;

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

	/* Then if the player wants to rotate it
	 * clockwise or counter-clockwise */
	int rot_way;
	if (rotation > 0)
		rot_way = 0;
	else
		rot_way = 1;

	/* This will make 5 tests, trying to move the piece around.
	 * If any of them pass, the piece is immediately moved.
	 * If none of them pass, well...
	 * Lets say we're out of luck.
	 */
	int i;
	for (i = 0; i < 5; i++)
	{
		char dx = srs_possible_positions[type][rot_way][rot_num][i][0];
		char dy = srs_possible_positions[type][rot_way][rot_num][i][1];

		/* Remember, the piece #tmp has already been rotated.
		 * Here we just try to move it,
		 * making it preform wall/floor kicks */
		tmp.x += dx;
		tmp.y += dy;

		/* alright, if it worked for #tmp, it will work for #p */
		if (piece_is_on_valid_position(&tmp, b))
		{
			piece_rotate(p, rotation);
			p->x += dx;
			p->y += dy;
			return true;
		}

		/* well, it didnt work, so lets return #tmp
		 * to its initial position */
		tmp.x -= dx;
		tmp.y -= dy;
	}
	return false;
}

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
}

bool piece_move_if_possible(piece_s* p, board_s* b, direction_e dir)
{
	if (piece_can_move(p, b, dir))
	{
		piece_move(p, dir);
		return true;
	}
	return false;
}

void piece_hard_drop(piece_s* p, board_s* b)
{
	while (piece_can_move(p, b, DIR_DOWN))
		piece_move(p, DIR_DOWN);
}

piece_type_e piece_random_type()
{
	/* Default 'smart' algorithm
	 *
	 * The logic is to keep a bag with
	 * one of each piece (7 pieces total).
	 *
	 * Then we take one after another in a random order.
	 * As soon as we took them all,
	 * place them back on the bag and redo it.
	 *
	 * This way, we avoid long sequences
	 * of the same piece and guarantee a certain
	 * degree of piece rotativity ("I WANT TEH LINES!11!!").
	 */
	if (global.game_random_algorithm == 1)
	{
		int i,j;
		int size = PIECE_MAX;

		static int bag[PIECE_MAX];
		static int piece = PIECE_MAX;

		if (piece >= PIECE_MAX)
		{
			piece = 0;

			bag[0] = random_int_between(0, size - 1);
			for (i = 1; i < size; i++)
			{
				bag[i] = random_int_between(0, size - 1);
				j = 0;
				do {				///for (j = 0; j < i; j++)

					if (bag[j] == bag[i])
					{
						bag[i] = random_int_between(0, size - 1);
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
//	else if (global.game_random_algorithm == 2)
	/* dummy random algorithm */
	else
	{
		return random_int_between(0, (PIECE_MAX - 1));
	}
}

bool piece_can_move(piece_s* p, board_s* b, direction_e dir)
{
	piece_s tmp = (*p);

	piece_move(&tmp, dir);
	return piece_is_on_valid_position(&tmp, b);
}

bool piece_is_on_valid_position(piece_s* p, board_s* b)
{
	/* We're simply looking up on the global "table"
	 * of piece positions and rotations.
	 */
	int i, j;

	for (i = 0; i < PIECE_BLOCKS; i++)
	{
		for (j = 0; j < PIECE_BLOCKS; j++)
		{
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				/* WHAT THE FUCK */
		/* /\* Here we don't confuse our 'dummy' blocks */
		/*  * with real ones *\/ */
		/* p->block[k].is_visible = false; */

				int block_x = i + p->x;
				int block_y = j + p->y;

				/* Off-limits check */
				if ((block_x >= BOARD_WIDTH) ||
				    (block_y >= BOARD_HEIGHT) ||
				    (block_x < 0))
					return false;

				/* If the piece is still out of the board,
				 * we don't check collision */
				if (block_y < 0) continue;

				/* Fellow blocks check */
				if (b->block[block_x][block_y])
					return false;
			}
		}
	}
	/* Pheew! */
	return true;
}

bool piece_is_valid(piece_s* p)
{
	/* p is always positive */
	if (p->type >= PIECE_MAX)
		return false;
	else
		return true;
}

