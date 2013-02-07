#ifndef PIECE_H_DEFINED
#define PIECE_H_DEFINED

#include "block.h"

/* How many blocks exists around the piece. Check the global
 * array #pieces -- each piece has 5 x 5 blocks */
#define PIECE_BLOCKS 5

/** Enumerate all possible pieces.
 *  @note This order is important.
 *  Also, PIECE_MAX tells the biggest number for a piece
 */
typedef enum { PIECE_O = 0, PIECE_I, PIECE_J,
               PIECE_L,     PIECE_S, PIECE_Z, PIECE_T,
               PIECE_MAX} piece_e;

/** Possible directions a piece can move */
typedef enum { UP, DOWN, LEFT, RIGHT } direction_e;

/* Forward declaration of the board type -- check #board.h */
typedef struct board_s board_s;

typedef struct piece_s
{
	piece_e type;
	int     x;
	int     y;
	color_e color;
	char    theme[3]; /* the appearance of each block */
	block_s block[4]; /* the blocks */
	int     rotation; /* current rotation (0 is none) */

} piece_s;

piece_s new_piece(piece_e type);
void engine_draw_piece(piece_s* p);
void piece_rotate(piece_s* p, int rotation);
void piece_move(piece_s* p, direction_e dir);
void piece_hard_drop(piece_s* p);
piece_e piece_get_random();
bool piece_can_move(piece_s* p, board_s* b, direction_e dir);

#endif /* PIECE_H_DEFINED */
