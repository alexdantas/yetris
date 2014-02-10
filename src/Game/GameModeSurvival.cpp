#include <Game/GameModeSurvival.hpp>
#include <Game/RotationSystemSRS.hpp>
#include <Game/PieceDefinitions.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>
#include <Interface/LayoutGameModeSurvival.hpp>

#include <stdlib.h>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	RESUME, QUIT_MENU, QUIT_GAME
};

GameModeSurvival::GameModeSurvival():
	GameMode(),
	layout(nullptr),
	gameOver(false),
	pieceCurrent(nullptr),
	pieceGhost(nullptr),
	pieceHold(nullptr),
	board(nullptr),
	rotationSystem(nullptr),
	movedPieceDown(nullptr),
	canHold(true),
	willClearLines(true),
	isInvisible(false),
	score(nullptr),
	isPaused(false),
	showPauseMenu(false),
	showHelp(false),
	pauseMenu(nullptr)
{ }

void GameModeSurvival::start()
{
	SAFE_DELETE(this->pieceCurrent);
	SAFE_DELETE(this->pieceGhost);
	SAFE_DELETE(this->pieceHold);
	SAFE_DELETE(this->rotationSystem);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->score);
	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->pieceGhost);
	SAFE_DELETE(this->pauseMenu);

	this->nextPieces.clear();

	this->userAskedToQuit = false;
	this->gameOver = false;

	this->layout = new LayoutGameModeSurvival(this, 80, 24);

	// Creating the board and adding noise.
	this->board = new Board(0, 0, DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);

	if (Globals::Game::initial_noise != 0)
		this->board->addNoise(Globals::Game::initial_noise);

	// Populating all the next pieces
	this->nextPieces.resize(Globals::Game::next_pieces);
	for (unsigned int i = 0; i < (this->nextPieces.size()); i++)
		this->nextPieces[i] = Piece::random();

	// And the first piece
	this->pieceCurrent = this->getNextPiece();

	if (Globals::Game::has_ghost)
		this->pieceGhost = new PieceGhost();

	this->pieceHold = nullptr;

	this->rotationSystem = new RotationSystemSRS();

	this->score = new Score();
	this->score->level = Globals::Game::starting_level;

	// Creating the menu and adding each item
	this->pauseMenu = new Menu(1,
	                           1,
	                           this->layout->pause->getW() - 2,
	                           this->layout->pause->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Resume", RESUME);
	this->pauseMenu->add(item);

	this->pauseMenu->addBlank();

	item = new MenuItem("Quit to Main Menu", QUIT_MENU);
	this->pauseMenu->add(item);

	item = new MenuItem("Quit Game", QUIT_GAME);
	this->pauseMenu->add(item);

	// Starting timers
	this->timerPiece.start();
	this->stats = Statistics();

	this->timerInvisible.start();
	this->timer.start();
}
void GameModeSurvival::handleInput(int c)
{
	// The only two absolute inputs are to quit and pause.
	// Others depend if the game is paused or not.

	if (c == Globals::Input::quit)
	{
		this->userAskedToQuit = true;
	}
	else if (c == Globals::Input::pause)
	{
		(this->isPaused) ?
			this->pause(false) :
			this->pause(true);

		return;
	}
	else if ((c == '\n') || (c == KEY_ENTER))
	{
		if (! this->isPaused)
		{
			this->pause(true);
			return;
			// This needs to be here otherwise
			// ENTER goes to the menu and immediately
			// unpauses the game.
		}
	}
	else if (c == 'h' || c == 'H')
	{
		// Toggling Pause and Help window
		if (this->isPaused)
		{
			this->isPaused = false;
			this->showHelp = false;
			this->timer.unpause();
			this->timerPiece.unpause();
		}
		else
		{
			this->isPaused = true;
			this->showHelp = true;
			this->timer.pause();
			this->timerPiece.pause();
		}
	}

	// Other keys are not used when paused.
	if (this->isPaused)
	{
		this->pauseMenu->handleInput(c);
		return;
	}

	if (c == Globals::Input::left)
	{
		movePieceIfPossible(Piece::DIR_LEFT);
		this->movedPieceDown = true;
	}
	else if (c == Globals::Input::down)
	{
		if (! movePieceIfPossible(Piece::DIR_DOWN))
			this->lockCurrentPiece();

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
	// If we're paused, only handle the menu.
	if (this->isPaused)
	{
		if (this->pauseMenu->willQuit())
		{
			int option = this->pauseMenu->getSelectedValue();

			switch(option)
			{
			case RESUME:
				this->pause(false);
				break;

			case QUIT_GAME:
				this->userAskedToQuit = true;
				break;
			}
			this->pauseMenu->reset();
		}

		return;
	}

	// Dropping piece if enough time has passed
	// (time based on current level which is based on how
	//  many lines were cleared)
	this->timerPiece.pause();
	int delta = this->getDelay(this->score->level);

	if (this->timerPiece.delta_ms() >= delta)
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

	if (Globals::Game::has_ghost)
	{
		this->pieceGhost->update(this->pieceCurrent,
		                         this->board);
	}

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

	// Updating level based on total lines cleared.
	//
	// Will only update the level if it's greater than
	// what currently is.
	// It allows you to set a high current level even
	// without clearing enough lines to get there.
	unsigned int new_level = this->getLevel(this->score->lines);

	if (new_level > this->score->level)
		this->score->level = new_level;

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
	this->layout->draw(this->pauseMenu);
}
bool GameModeSurvival::isOver()
{
	return (this->gameOver);
}
bool GameModeSurvival::movePieceIfPossible(Piece::PieceDirection direction)
{
	Piece tmp = *(this->pieceCurrent);
	tmp.move(direction);

	if (this->board->isPieceValid(&tmp))
	{
		this->pieceCurrent->move(direction);
		return true;
	}
	return false;
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

	// To draw it nicely (HACK)
	if (this->pieceHold->getType() == Piece::O)
		this->pieceHold->moveBy(-1, 1);

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
int GameModeSurvival::getLevel(int lines)
{
	// this is getting too long - need to create a math function

	if (lines < 5)   return 1;
	if (lines < 10)  return 2;
	if (lines < 15)  return 3;
	if (lines < 20)  return 4;
	if (lines < 25)  return 5;
	if (lines < 30)  return 6;
	if (lines < 40)  return 7;
	if (lines < 50)  return 8;
	if (lines < 60)  return 9;
	if (lines < 70)  return 10;
	if (lines < 100) return 11;
	if (lines < 120) return 12;
	if (lines < 140) return 13;
	if (lines < 160) return 14;
	if (lines < 180) return 15;
	if (lines < 210) return 16;
	if (lines < 240) return 17;

	return 0;
}
int GameModeSurvival::getDelay(int level)
{
	// returning delay in milliseconds
	if (level < 2)  return 1000;
	if (level < 3)  return 900;
	if (level < 4)  return 850;
	if (level < 5)  return 800;
	if (level < 6)  return 750;
	if (level < 7)  return 700;
	if (level < 8)  return 650;
	if (level < 9)  return 600;
	if (level < 10)  return 550;
	if (level < 11) return 500;
	if (level < 12) return 450;
	if (level < 13) return 400;
	if (level < 14) return 350;
	if (level < 15) return 300;
	if (level < 16) return 250;
	if (level < 17) return 200;
	if (level < 18) return 150;
	if (level < 19) return 100;

	return 0;
}
void GameModeSurvival::pause(bool option)
{
	if (option)
	{
		if (this->isPaused)
			return;

		this->isPaused = true;
		this->showPauseMenu = true;
		this->timer.pause();
		this->timerPiece.pause();
	}
	else
	{
		if (! this->isPaused)
			return;

		this->isPaused = false;
		this->showPauseMenu = false;
		this->timer.unpause();
		this->timerPiece.unpause();
	}
}

