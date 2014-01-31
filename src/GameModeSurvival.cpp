#include "GameModeSurvival.hpp"
#include "RotationSystemSRS.hpp"
#include "Globals.hpp"

GameModeSurvival::GameModeSurvival():
	pieceCurrent(NULL),
	pieceGhost(NULL),
	pieceHold(NULL),
	board(NULL)
{ }

void GameModeSurvival::start()
{
#define SAFE_DELETE(a) \
	{ \
		if (a) \
			delete(a); \
	}

	SAFE_DELETE(this->pieceCurrent);
	SAFE_DELETE(this->pieceGhost);
	SAFE_DELETE(this->pieceHold);
	SAFE_DELETE(this->board);
	this->nextPieces.clear();

	this->willQuit = false;

	this->pieceCurrent = new Piece(Piece::random(), 0, 0);

	this->board = new Board(0, 0, DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);

	this->rotationSystem = new RotationSystemSRS();

	this->nextPieces.resize(Globals::Game::next_pieces);
	for (unsigned int i = 0; i < (this->nextPieces.size()); i++)
		this->nextPieces[i] = Piece::random();
}
void GameModeSurvival::handleInput(int c)
{
	switch(c)
	{
	case 'q':
		this->willQuit = true;
		break;

	case 'a':
		movePieceIfPossible(Piece::DIR_LEFT);
		break;

	case 's':
		movePieceIfPossible(Piece::DIR_DOWN);
		break;

	case 'd':
		movePieceIfPossible(Piece::DIR_RIGHT);
		break;

	case 'w':
		this->rotationSystem->rotate(this->pieceCurrent,
		                             this->board,
		                             1);
		break;

	case ' ':
	{
		this->board->lockPiece(this->pieceCurrent);

		delete this->pieceCurrent;
		this->pieceCurrent = new Piece(this->nextPieces[0], 0, 0);

		for (unsigned int i = 0; i < (this->nextPieces.size() - 1); i++)
			this->nextPieces[i] = this->nextPieces[i + 1];

		this->nextPieces[this->nextPieces.size() - 1] = Piece::random();
		break;
	}
	}
}
void GameModeSurvival::update()
{

}
void GameModeSurvival::draw(LayoutGame* layout)
{
	layout->board->clear();

	this->board->draw(layout->board);

	this->pieceCurrent->draw(layout->board);

	layout->board->refresh();
}
bool GameModeSurvival::isOver()
{
	return (this->willQuit);
}
// Local function that moves a piece only if it's possible
void GameModeSurvival::movePieceIfPossible(Piece::PieceDirection direction)
{
	Piece tmp = *(this->pieceCurrent);
	tmp.move(direction);

	if (this->board->isPieceValid(&tmp))
		this->pieceCurrent->move(direction);
}

