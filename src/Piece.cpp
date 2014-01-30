#include "Piece.hpp"
#include "Globals.hpp"
#include "Window.hpp"
#include "PieceDefinitions.hpp"

Piece::Piece(Piece::PieceType type, int x, int y):
	type(type),
	x(x),
	y(y),
	rotation(0)
{
	switch(type)
	{
	case PIECE_S: this->block = Globals::Theme::piece_S; break;
	case PIECE_Z: this->block = Globals::Theme::piece_Z; break;
	case PIECE_O: this->block = Globals::Theme::piece_O; break;
	case PIECE_I: this->block = Globals::Theme::piece_I; break;
	case PIECE_L: this->block = Globals::Theme::piece_L; break;
	case PIECE_J: this->block = Globals::Theme::piece_J; break;
	case PIECE_T: this->block = Globals::Theme::piece_T; break;

	default:
		this->block = Globals::Theme::piece_colorless;
		break;
	}

	/* The starting positions are derived from the SRS */
	// p.x = BOARD_WIDTH/2 + global_pieces_position[p.type][p.rotation][0];
	// p.y = global_pieces_position[p.type][p.rotation][1];

}
void Piece::draw(Window* win)
{
	if (!this->block)
		return;

	// Looking up the global table of
	// possible piece positions and rotations
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (global_pieces[this->type][this->rotation][j][i] != 0)
				this->block->draw(win,
				                  (this->x + i) * 2,
				                  this->y + j);
}

