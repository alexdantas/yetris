#ifndef BOARD_H_DEFINED
#define BOARD_H_DEFINED

#include <stdbool.h>
#include "block.h"

/* How many blocks we have for width and height */
#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20

/* Forward declaration of the piece type -- see #piece.h */
typedef struct piece_s piece_s;

/** The map containing all blocks */
typedef struct board_s
{
	block_s block[BOARD_WIDTH][BOARD_HEIGHT];

} board_s;

board_s new_board();
void board_save_piece(board_s* b, piece_s* p);
void board_delete_line(board_s* b, int line);
bool board_is_full(board_s* b);

#endif /* BOARD_H_DEFINED */
