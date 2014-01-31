#include "GameModeSurvival.hpp"
#include "RotationSystemSRS.hpp"
#include "Globals.hpp"

GameModeSurvival::GameModeSurvival(LayoutGame* layout):
	GameMode(layout),
	gameOver(false),
	pieceCurrent(NULL),
	pieceGhost(NULL),
	pieceHold(NULL),
	board(NULL),
	movedPieceDown(NULL)
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
	this->gameOver = false;

	this->pieceCurrent = new Piece(Piece::random(), 0, 0);

	this->board = new Board(0, 0, DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);

	this->rotationSystem = new RotationSystemSRS();

	this->nextPieces.resize(Globals::Game::next_pieces);
	for (unsigned int i = 0; i < (this->nextPieces.size()); i++)
		this->nextPieces[i] = Piece::random();

	this->pieceTimer.start();

	this->stats = Statistics();
}
void GameModeSurvival::handleInput(int c)
{
	if (c == Globals::Input::quit)
	{
		this->willQuit = true;
	}
	else if (c == Globals::Input::left)
	{
		movePieceIfPossible(Piece::DIR_LEFT);
		this->movedPieceDown = true;
	}
	else if (c == Globals::Input::down)
	{
		movePieceIfPossible(Piece::DIR_DOWN);
		this->movedPieceDown = true;
	}
	else if (c == Globals::Input::right)
	{
		movePieceIfPossible(Piece::DIR_RIGHT);
		this->movedPieceDown = true;
	}
	else if (c == Globals::Input::rotate_clockwise)
	{
		this->rotationSystem->rotate(this->pieceCurrent,
		                             this->board,
		                             1);
	}
	else if (c == Globals::Input::rotate_counterclockwise)
	{
		this->rotationSystem->rotate(this->pieceCurrent,
		                             this->board,
		                             -1);
	}
	else if (c == Globals::Input::drop)
	{
		// Drop piece
	}
}
void GameModeSurvival::update()
{
	// Droping piece if enough time has passed
	this->pieceTimer.pause();
	if (this->pieceTimer.delta_ms() >= 500)
	{
		if (! this->movedPieceDown)
		{
			Piece tmp = *(this->pieceCurrent);
			tmp.move(Piece::DIR_DOWN);

			if (this->board->isPieceValid(&tmp))
				this->pieceCurrent->move(Piece::DIR_DOWN);
			else
				this->lockCurrentPiece();
		}
		else
			this->movedPieceDown = false;

		this->pieceTimer.start();
	}
	else
		this->pieceTimer.unpause();

	int lines = this->board->clearFullLines(this->layout->board);

	// Statistics
	this->stats.lines += lines;
	switch(lines)
	{
	case 1: this->stats.singles++; break;
	case 2: this->stats.doubles++; break;
	case 3: this->stats.triples++; break;
	case 4: this->stats.tetris++;  break;
	}

	// Score

	// Checking if game over
	if (this->board->isFull())
		this->gameOver = true;
}
void GameModeSurvival::draw()
{
	if (! this->layout)
		return;

	this->layout->board->clear();

	this->board->draw(this->layout->board);

	this->pieceCurrent->draw(this->layout->board);

	this->layout->board->refresh();
}
bool GameModeSurvival::isOver()
{
	return (this->willQuit);
}
void GameModeSurvival::movePieceIfPossible(Piece::PieceDirection direction)
{
	Piece tmp = *(this->pieceCurrent);
	tmp.move(direction);

	if (this->board->isPieceValid(&tmp))
		this->pieceCurrent->move(direction);
}
void GameModeSurvival::lockCurrentPiece()
{
	// Statistics
	this->stats.pieces++;

	switch(this->pieceCurrent->getType())
	{
	case Piece::PIECE_O: this->stats.O++; break;
	case Piece::PIECE_I: this->stats.I++; break;
	case Piece::PIECE_L: this->stats.L++; break;
	case Piece::PIECE_J: this->stats.J++; break;
	case Piece::PIECE_Z: this->stats.Z++; break;
	case Piece::PIECE_S: this->stats.S++; break;
	case Piece::PIECE_T: this->stats.T++; break;
	default: break;
	}

	// Actually locking the current piece
	this->board->lockPiece(this->pieceCurrent);

	// Getting next piece
	delete this->pieceCurrent;
	this->pieceCurrent = new Piece(this->nextPieces[0], 0, 0);

	// Adjusting the text pieces array
	for (unsigned int i = 0; i < (this->nextPieces.size() - 1); i++)
		this->nextPieces[i] = this->nextPieces[i + 1];

	this->nextPieces[this->nextPieces.size() - 1] = Piece::random();
}

