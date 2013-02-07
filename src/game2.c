
#include <stdbool.h>





typedef struct block_s
{
	Colors color;

} block_s;


typedef struct piece_s
{


} piece_s;

int piece_get_block_type(int piece, int rotation, int x, int y)
{
	return pieces[piece][rotation][x][y];
}

int piece_get_initial_x(int piece, int rotation)
{
	return pieces_position[piece][rotation][0];
}

int piece_get_initial_y(int piece, int rotation)
{
	return pieces_position[piece][rotation][1];
}

////////////////////////////////////////////////////////////////////////////////

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20
#define PIECE_BLOCKS 5 /* number of horizontal and vertical blocks on a piece */

enum { POS_FREE, POS_FILLED };

typedef struct board_s
{
	block_s board[BOARD_WIDTH][BOARD_HEIGHT];

} board_s;


void board_init(board_s* b)
{
	int i, j;
	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			b->board[i][j] = POS_FREE;
}

void board_insert_piece(board_s* b, int x, int y, int piece, int rotation)
{
	int board_x, board_y, piece_x, piece_y;

	for (board_x = x, piece_x = 0; board_x < piece_x + PIECE_BLOCKS; board_x++, piece_x++)
		for (board_y = y, piece_y = 0; board_y < piece_y + PIECE_BLOCKS; board_y++, piece_y++)
			if (piece_get_block_type(piece, rotation, piece_y, piece_x) != 0)
				b->board[board_x][board_y] = POS_FILLED;
}

bool board_is_game_over(board_s* b)
{
	int i;
	for (i = 0; i < BOARD_WIDTH; i++)
		if (b->board[i][0] == POS_FILLED)
			return true;
}

void board_delete_line(board_s* b, int line_y)
{
	int i, j;
	for (j = line_y; j > 0; j--)
		for (i = 0; i < BOARD_WIDTH; i++)
			b->board[i][j] = b->board[i][j - 1];
}

void board_delete_possible_lines(board_s* b)
{
	int i, j;
	for (j = 0; j < BOARD_HEIGHT; j++)
	{
		i = 0;
		while (i < BOARD_WIDTH)
		{
			if (b->board[i][j] != POS_FILLED)
				break;
			i++;
		}
		if (i == BOARD_WIDTH)
			board_delete_line(b, j);
	}
}

bool board_is_free_block(board_s* b, int x, int y)
{
	b->board[x][y] == POS_FREE ? true : false;
}

bool board_is_possible_movement(board_s* b, int x, int y, int piece, int rotation)
{
	for (board_x = x, piece_x = 0; board_x < x + PIECE_BLOCKS; board_x++, piece_x++)
	{
		for (board_y = y, piece_y = 0; board_y < y + PIECE_BLOCKS; board_y++, piece_y++)
		{
			if ((board_x < 0) || (board_x > BOARD_WIDTH - 1) || (board_y > BOARD_HEIGHT - 1))
				if (piece_get_block_type(piece, rotation, piece_y, piece_x) != 0)
					return 0;
			if (board_y >= 0)
				if ((pieces_get_block_type(piece, rotation, piece_y, piece_x) != 0) &&
					(!block_is_free(board_x, board_y)))
					return false;
		}
	}

	return true;
}



