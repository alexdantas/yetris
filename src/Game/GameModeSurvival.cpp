#include <Game/GameModeSurvival.hpp>
#include <Game/RotationSystemSRS.hpp>
#include <Game/PieceDefinitions.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>
#include <Interface/LayoutGameModeSurvival.hpp>

#include <stdlib.h>

GameModeSurvival::GameModeSurvival():
	GameMode(),
	layout(NULL),
	gameOver(false),
	pieceCurrent(NULL),
	pieceGhost(NULL),
	pieceHold(NULL),
	board(NULL),
	movedPieceDown(NULL),
	canHold(true),
	willClearLines(true),
	isInvisible(false),
	score(nullptr)
{ }

void GameModeSurvival::start()
{
	SAFE_DELETE(this->pieceCurrent);
	SAFE_DELETE(this->pieceGhost);
	SAFE_DELETE(this->pieceHold);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->score);
	this->nextPieces.clear();

	this->userAskedToQuit = false;
	this->gameOver = false;

	this->layout = new LayoutGameModeSurvival(this, 80, 24);

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

	this->score = new Score();

	this->timerPiece.start();
	this->stats = Statistics();

	this->timerInvisible.start();
	this->timer.start();
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
	else if (c == 'o')
	{
		this->board->turnInvisible(false);
	}
}
void GameModeSurvival::update()
{
	// Dropping piece if enough time has passed
	this->timerPiece.pause();
	if (this->timerPiece.delta_ms() >= 800)
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

		this->timerPiece.start();
	}
	else
		this->timerPiece.unpause();

	this->pieceGhost->update(this->pieceCurrent,
	                         this->board);

	// Clearing lines previously marked as full
	if (this->willClearLines)
	{
		this->board->clearFullLines();
		this->willClearLines = false;
	}

	// How many lines will be cleared on the next frame.
	int lines = this->board->markFullLines();

	if (lines != 0)
	{
		this->willClearLines = true;

		// Statistics
		this->stats.lines += lines;
		this->score->lines += lines;
		switch(lines)
		{
		case 1: this->stats.singles++; break;
		case 2: this->stats.doubles++; break;
		case 3: this->stats.triples++; break;
		case 4: this->stats.tetris++;  break;
		}

		// Applying score (kinda complex)

		// Score according to how many lines were cleared now
		int line_score = 0;
		switch (lines)
		{
		case 1:  line_score = 100; break;
		case 2:  line_score = 300; break;
		case 3:  line_score = 500; break;
		case 4:  line_score = 800; break;
		default: line_score = -1;  break; // someone's cheating...
		}

		this->score->points += line_score;
	}



	// Checking if game over
	if (this->board->isFull())
		this->gameOver = true;

	// If on invisible mode, will flash the pieces
	// once in a while
	if (Globals::Game::invisible)
	{
		this->timerInvisible.pause();
		if (this->isInvisible)
		{
			if (this->timerInvisible.delta_s() > 3)
			{
				this->board->turnInvisible(false);
				this->timerInvisible.start();
				this->isInvisible = false;
			}
			else
				this->timerInvisible.unpause();
		}
		else
		{

			if (this->timerInvisible.delta_ms() > 500)
			{
				this->board->turnInvisible(true);
				this->timerInvisible.start();
				this->isInvisible = true;
			}
			else
				this->timerInvisible.unpause();
		}
	}
}
void GameModeSurvival::draw()
{
	this->layout->draw();
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

	// Invisible game mode! Yay!
	if (Globals::Game::invisible)
		this->pieceCurrent->block = Globals::Theme::invisible;

	// Actually locking the current piece
	this->board->lockPiece(this->pieceCurrent);

	// Free score
	this->score->points += 10;

	// Sliding left/right based on options
	if (Globals::Game::slide_right)
		this->board->pushRight();

	if (Globals::Game::slide_left)
		this->board->pushLeft();

	// Getting next piece
	delete this->pieceCurrent;
	this->pieceCurrent = this->getNextPiece();

	// Since we've dropped the piece, we can hold now
	this->canHold = true;

	this->timerPiece.start();
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

