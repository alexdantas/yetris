#include <Game/GameModeSurvival.hpp>
#include <Game/RotationSystemSRS.hpp>
#include <Game/PieceDefinitions.hpp>
#include <Config/Globals.hpp>

GameModeSurvival::GameModeSurvival(LayoutGame* layout):
	GameMode(layout),
	gameOver(false),
	pieceCurrent(NULL),
	pieceGhost(NULL),
	pieceHold(NULL),
	board(NULL),
	movedPieceDown(NULL),
	canHold(true)
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

	this->userAskedToQuit = false;
	this->gameOver = false;

	this->board = new Board(0, 0, DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);

	// Populating all the next pieces
	this->nextPieces.resize(Globals::Game::next_pieces);
	for (unsigned int i = 0; i < (this->nextPieces.size()); i++)
		this->nextPieces[i] = Piece::random();

	// And the first piece
	this->pieceCurrent = this->getNextPiece();

	this->pieceGhost = new PieceGhost();
	this->pieceHold = NULL;

	this->rotationSystem = new RotationSystemSRS();

	this->pieceTimer.start();

	this->stats = Statistics();
}
void GameModeSurvival::handleInput(int c)
{
	if (c == Globals::Input::quit)
	{
		this->userAskedToQuit = true;
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
		this->board->hardDrop(this->pieceCurrent);
		this->lockCurrentPiece();
	}
	else if (c == Globals::Input::hold)
	{
		this->holdCurrentPiece();
	}
}
void GameModeSurvival::update()
{
	// Dropping piece if enough time has passed
	this->pieceTimer.pause();
	if (this->pieceTimer.delta_ms() >= 800)
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

	this->pieceGhost->update(this->pieceCurrent,
	                         this->board);

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

	this->pieceGhost->draw(this->layout->board);
	this->pieceCurrent->draw(this->layout->board);

	this->layout->board->refresh();

	if (this->pieceHold)
	{
		this->layout->hold->clear();
		this->pieceHold->draw(this->layout->hold);
		this->layout->hold->refresh();
	}
}
bool GameModeSurvival::isOver()
{
	return (this->gameOver);
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
	case Piece::O: this->stats.O++; break;
	case Piece::I: this->stats.I++; break;
	case Piece::L: this->stats.L++; break;
	case Piece::J: this->stats.J++; break;
	case Piece::Z: this->stats.Z++; break;
	case Piece::S: this->stats.S++; break;
	case Piece::T: this->stats.T++; break;
	default: break;
	}

	// Actually locking the current piece
	this->board->lockPiece(this->pieceCurrent);

	// Getting next piece
	delete this->pieceCurrent;

	this->pieceCurrent = this->getNextPiece();

	// Since we've dropped the piece, we can hold now
	this->canHold = true;

	this->pieceTimer.start();
}
Piece* GameModeSurvival::getNextPiece()
{
	Piece::PieceType new_type = this->nextPieces[0];

	int x = (this->board->getW()/2) + global_pieces_position[new_type][0][0];
	int y = global_pieces_position[new_type][0][1] + 1;

	Piece* next = new Piece(new_type, x, y);

	// Adjusting the text pieces array
	for (unsigned int i = 0; i < (this->nextPieces.size() - 1); i++)
		this->nextPieces[i] = this->nextPieces[i + 1];

	this->nextPieces[this->nextPieces.size() - 1] = Piece::random();

	return next;
}
void GameModeSurvival::holdCurrentPiece()
{
	if ((! Globals::Game::can_hold) ||
	    (! this->canHold))
		return;

	this->canHold = false;

	Piece* tmp = this->pieceHold;

	this->pieceHold = new Piece(this->pieceCurrent->getType(),
	                            0, 0);

	if (! tmp)
	{
		this->pieceCurrent = this->getNextPiece();
	}
	else
	{
		this->pieceCurrent = tmp;

		Piece::PieceType new_type = tmp->getType();

		int x = (this->board->getW()/2) + global_pieces_position[new_type][0][0];
		int y = global_pieces_position[new_type][0][1];

		this->pieceCurrent->moveTo(x, y);
	}
}
bool GameModeSurvival::willQuit()
{
	return this->userAskedToQuit;
}

