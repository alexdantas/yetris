#ifndef PIECE_H_DEFINED
#define PIECE_H_DEFINED

#include "block.h"

/* How many blocks exists around the piece. Check the global
 * array #pieces -- each piece has 5 x 5 blocks */
#define PIECE_BLOCKS 5

/* How many blocks we have for width and height */
#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20


/** Enumerate all possible pieces.
 *  @note This order is important.
 *  Also, PIECE_MAX tells the biggest number for a piece
 */
typedef enum piece_e { PIECE_O = 0, PIECE_I, PIECE_J,
                       PIECE_L,     PIECE_S, PIECE_Z, PIECE_T,
                       PIECE_MAX} piece_e;

typedef struct piece_s
{
	piece_e type;
	int     x;
	int     y;
	color_e color;
	char    theme[3]; /* the appearance of each block of this piece */
	block_s block[4];
	int     rotation;

} piece_s;

piece_s new_piece(piece_e type);
void engine_draw_piece(piece_s* p);
void piece_rotate(piece_s* p, int rotation);
void piece_soft_drop(piece_s* p);
void piece_move(piece_s* p, int direction);
void piece_hard_drop(piece_s* p);
piece_e piece_get_random();

#endif /* PIECE_H_DEFINED */
