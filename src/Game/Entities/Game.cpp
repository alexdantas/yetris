#include <Game/Entities/Game.hpp>
#include <Game/Entities/RotationSystemSRS.hpp>
#include <Game/Entities/PieceDefinitions.hpp>
#include <Game/Config/Globals.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Game/Display/Layouts/LayoutGame.hpp>
#include <Engine/InputManager.hpp>
#include <Game/Entities/Profile.hpp>

#include <stdlib.h>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	RESUME, QUIT_MENU, QUIT_GAME
};

Game::Game():
	layout(NULL),
	gameOver(false),
	pieceCurrent(NULL),
	pieceGhost(NULL),
	pieceHold(NULL),
	board(NULL),
	rotationSystem(NULL),
	movedPieceDown(NULL),
	canHold(true),
	willClearLines(true),
	isInvisible(false),
	isPaused(false),
	showPauseMenu(false),
	showHelp(false),
	pauseMenu(NULL)
{ }

void Game::start()
{
	SAFE_DELETE(this->pieceCurrent);
	SAFE_DELETE(this->pieceGhost);
	SAFE_DELETE(this->pieceHold);
	SAFE_DELETE(this->rotationSystem);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->pieceGhost);
	SAFE_DELETE(this->pauseMenu);

	this->nextPieces.clear();

	this->userAskedToQuit     = false;
	this->userAskedToGoToMenu = false;
	this->gameOver            = false;

	this->layout = new LayoutGame(this, 80, 24);

	// Creating the board and adding noise.
	this->board = new Board(0, 0, DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);

	if (Globals::Profiles::current->scores->score.initial_noise != 0)
		this->board->addNoise(Globals::Profiles::current->scores->score.initial_noise);

	// Populating all the next pieces
	this->nextPieces.resize(Globals::Profiles::current->settings.game.next_pieces);
	for (unsigned int i = 0; i < (this->nextPieces.size()); i++)
		this->nextPieces[i] = Piece::random();

	// And the first piece
	this->pieceCurrent = this->getNextPiece();

	if (Globals::Profiles::current->settings.game.has_ghost)
		this->pieceGhost = new PieceGhost();

	this->pieceHold = NULL;

	this->rotationSystem = new RotationSystemSRS();

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
void Game::handleInput()
{
	if (InputManager::noKeyPressed())
		return;

	// The only two absolute inputs are to quit and pause.
	// Others depend if the game is paused or not.

	if (InputManager::isPressed("quit"))
	{
		this->userAskedToQuit = true;
	}
	else if (InputManager::isPressed("pause"))
	{
		(this->isPaused) ?
			this->pause(false) :
			this->pause(true);

		return;
	}
	else if (InputManager::isPressed((int)'l'))
	{
		Globals::Profiles::current->scores->score.points += 100;
	}
	else if (InputManager::isPressed((int)'\n') ||
	         InputManager::isPressed(KEY_ENTER))
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
	else if (InputManager::isPressed("help"))
	{
		// Toggling Pause and Help window
		if (this->isPaused)
		{
			this->showHelp = false;
			this->timer.unpause();
			this->timerPiece.unpause();
		}
		else
		{
			this->showHelp = true;
			this->timer.pause();
			this->timerPiece.pause();
		}
	}

	// Other keys are not used when paused.
	if (this->isPaused || this->showHelp)
	{
		this->pauseMenu->handleInput();
		return;
	}

	if (InputManager::isPressed("left"))
	{
		movePieceIfPossible(Piece::DIR_LEFT);
		this->movedPieceDown = true;
	}
	else if (InputManager::isPressed("down"))
	{
		if (! movePieceIfPossible(Piece::DIR_DOWN))
			this->lockCurrentPiece();

		this->movedPieceDown = true;
	}
	else if (InputManager::isPressed("right"))
	{
		movePieceIfPossible(Piece::DIR_RIGHT);
		this->movedPieceDown = true;
	}
	else if (InputManager::isPressed("rotate_clockwise"))
	{
		this->rotationSystem->rotate(this->pieceCurrent,
		                             this->board,
		                             1);
	}
	else if (InputManager::isPressed("rotate_counterclockwise"))
	{
		this->rotationSystem->rotate(this->pieceCurrent,
		                             this->board,
		                             -1);
	}
	else if (InputManager::isPressed("rotate_180"))
	{
		this->rotationSystem->rotate(this->pieceCurrent,
		                             this->board,
		                             2);
	}
	else if (InputManager::isPressed("drop"))
	{
		this->board->hardDrop(this->pieceCurrent);
		this->lockCurrentPiece();
	}
	else if (InputManager::isPressed("hold"))
	{
		this->holdCurrentPiece();
	}
}
void Game::update()
{
	// If we're paused, only handle the menu.
	if (this->isPaused)
	{
		if (this->pauseMenu->willQuit())
		{
			int option = this->pauseMenu->currentID();

			switch(option)
			{
			case RESUME:
				this->pause(false);
				break;

			case QUIT_MENU:
				this->userAskedToGoToMenu = true;
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
	int delta = this->getDelay(Globals::Profiles::current->scores->score.level);

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

	if (Globals::Profiles::current->settings.game.has_ghost)
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
		Globals::Profiles::current->scores->score.lines += lines;

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

		Globals::Profiles::current->scores->score.points += line_score;
	}

	// Updating level based on total lines cleared.
	//
	// Will only update the level if it's greater than
	// what currently is.
	// It allows you to set a high current level even
	// without clearing enough lines to get there.
	unsigned int new_level = this->getLevel(Globals::Profiles::current->scores->score.lines);

	if (new_level > Globals::Profiles::current->scores->score.level)
		Globals::Profiles::current->scores->score.level = new_level;

	// Checking if game over
	if (this->board->isFull())
		this->gameOver = true;

	// If on invisible mode, will flash the pieces
	// once in a while
	if (Globals::Profiles::current->scores->score.invisible)
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
void Game::draw()
{
	this->layout->draw(this->pauseMenu);
}
bool Game::isOver()
{
	return (this->gameOver);
}
bool Game::movePieceIfPossible(Piece::PieceDirection direction)
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
void Game::lockCurrentPiece()
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
	if (Globals::Profiles::current->scores->score.invisible)
		this->pieceCurrent->block = Globals::Profiles::current->settings.theme.invisible;

	// Actually locking the current piece
	this->board->lockPiece(this->pieceCurrent);

	// Free score
	Globals::Profiles::current->scores->score.points += 10;

	// Sliding left/right based on options
	if (Globals::Profiles::current->scores->score.slide_right)
		this->board->pushRight();

	if (Globals::Profiles::current->scores->score.slide_left)
		this->board->pushLeft();

	// Getting next piece
	delete this->pieceCurrent;
	this->pieceCurrent = this->getNextPiece();

	// Since we've dropped the piece, we can hold now
	this->canHold = true;

	this->timerPiece.start();
}
Piece* Game::getNextPiece()
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
void Game::holdCurrentPiece()
{
	if ((! Globals::Profiles::current->settings.game.can_hold) ||
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
bool Game::willQuit()
{
	return this->userAskedToQuit;
}
bool Game::willReturnToMenu()
{
	return this->userAskedToGoToMenu;
}
int Game::getLevel(int lines)
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
	if (lines < 280) return 18;
	if (lines < 310) return 19;
	if (lines < 350) return 20;
	if (lines < 400) return 21;
	if (lines < 450) return 22;

	return 0;
}
int Game::getDelay(int level)
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
	if (level < 10) return 550;
	if (level < 11) return 500;
	if (level < 12) return 450;
	if (level < 13) return 400;
	if (level < 14) return 350;
	if (level < 15) return 300;
	if (level < 16) return 250;
	if (level < 17) return 200;
	if (level < 18) return 150;
	if (level < 19) return 100;
	if (level < 20) return 80;
	if (level < 21) return 50;
	if (level < 22) return 25;
	if (level < 23) return 0;

	return 0;
}
void Game::pause(bool option)
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

