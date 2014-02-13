#include <Interface/LayoutGame.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

LayoutGame::LayoutGame(Game* game, int width, int height):
	Layout(width, height),
	game(game),
	leftmost(nullptr),
	hold(nullptr),
	score(nullptr),
	middle_left(nullptr),
	board(nullptr),
	middle_right(nullptr),
	rightmost(nullptr)
{
	this->windowsInit();
}
LayoutGame::~LayoutGame()
{
	this->windowsExit();
}
void LayoutGame::windowsInit()
{
	Layout::windowsInit();

	// We'll start all the windows inside the Layout
	this->main->clear();

	// If we're not going to show statistics, will
	// leave a big hole on the left.
	// Let's align it then.
	int leftmost_x = 0;

	if (! Globals::Profiles::current->settings.screen.show_statistics)
		leftmost_x = 18;

	// Leftmost window
	this->leftmost = new Window(this->main,
	                            leftmost_x,
	                            0,
	                            4*2 + 2, // width of a piece
	                            0);

	if (Globals::Profiles::current->settings.game.can_hold)
	{
		this->hold = new Window(this->leftmost,
		                        0,
		                        0,
		                        this->leftmost->getW(),
		                        4);

		if (Globals::Profiles::current->settings.screen.show_borders)
		{
			this->hold->borders(Globals::Profiles::current->settings.screen.fancy_borders ?
			                    Window::BORDER_FANCY :
			                    Window::BORDER_REGULAR);
		}
		this->hold->refresh();

		this->hold->setTitle("Hold");
		this->hold->clear();
		this->hold->refresh();
	}

	// This window will span all available height if
	// we can't hold.
	int score_y = ((this->hold) ?
	               this->hold->getH():
	               0);

	int score_height = ((this->hold) ?
	                    this->leftmost->getH() - this->hold->getH():
	                    this->leftmost->getH());

	this->score = new Window(this->leftmost,
	                         0,
	                         score_y,
	                         0,
	                         score_height);


	if (Globals::Profiles::current->settings.screen.show_borders)
	{
		this->score->borders(Globals::Profiles::current->settings.screen.fancy_borders ?
		                     Window::BORDER_FANCY :
		                     Window::BORDER_REGULAR);
	}
	this->score->refresh();

	// Middle-left, container for the board
	this->middle_left = new Window(this->main,
	                               this->leftmost->getX() + this->leftmost->getW(),
	                               0,
	                               10*2 + 2, // 10 blocks + borders
	                               0);

	if (Globals::Profiles::current->settings.screen.show_borders)
	{
		this->middle_left->borders(Globals::Profiles::current->settings.screen.fancy_borders ?
		                           Window::BORDER_FANCY :
		                           Window::BORDER_REGULAR);
	}
	this->middle_left->refresh();

	// The actual game border, inside that container.
	this->board = new Window(this->middle_left, 0, 0, 0, 0);

	// Middle-right, container of all the next pieces.
	// It's height depents on how many next pieces we're showing.
	int middle_right_height = Globals::Profiles::current->settings.game.next_pieces * 3 + 1;

	this->middle_right = new Window(this->main,
	                                this->middle_left->getX() + this->middle_left->getW(),
	                                0,
	                                4*2 + 2, // 4 blocks + borders
	                                middle_right_height);

	if (Globals::Profiles::current->settings.screen.show_borders)
	{
		this->middle_right->borders(Globals::Profiles::current->settings.screen.fancy_borders ?
		                            Window::BORDER_FANCY :
		                            Window::BORDER_REGULAR);
	}
	this->middle_right->setTitle("Next");
	this->middle_right->clear();
	this->middle_right->refresh();

	// First next piece
	this->next.resize(Globals::Profiles::current->settings.game.next_pieces);
	this->next[0] = new Window(this->middle_right, 0, 0, 0, 2);

	// The rest of the next pieces
	for (unsigned int i = 1; i < (this->next.size()); i++)
	{
		this->next[i] = new Window(this->middle_right,
		                           0,
		                           this->next[i - 1]->getY() + this->next[i - 1]->getH() + 1,
		                           0,
		                           2);

		this->next[i]->refresh();
	}

	// Will only show the right window if statistics is enabled
	if (Globals::Profiles::current->settings.screen.show_statistics)
	{
		// Container for Statistics and Miscellaneous information.
		this->rightmost = new Window(this->main,
		                             this->middle_right->getX() + this->middle_right->getW(),
		                             0,
		                             this->main->getW() - (this->middle_right->getX() + this->middle_right->getW()) - 1,
		                             0);

		if (Globals::Profiles::current->settings.screen.show_borders)
		{
			this->rightmost->borders(Globals::Profiles::current->settings.screen.fancy_borders ?
			                         Window::BORDER_FANCY :
			                         Window::BORDER_REGULAR);
		}
		this->rightmost->setTitle("Statistics");
		this->rightmost->clear();
		this->rightmost->refresh();
	}

	// Le pause window.
	this->pause = new Window(this->main,
	                         this->main->getW() / 4,
	                         this->main->getH() / 2 - 1, //center
	                         this->main->getW() / 2,
	                         6);

	if (Globals::Profiles::current->settings.screen.show_borders)
	{
		this->pause->borders(Globals::Profiles::current->settings.screen.fancy_borders ?
		                     Window::BORDER_FANCY :
		                     Window::BORDER_REGULAR);
	}
	this->pause->setTitle("Paused");

	// Le help window.
	this->help = new Window(this->main,
	                        this->main->getW() / 4,
	                        this->main->getH() / 4,
	                        this->main->getW() / 2,
	                        this->main->getH() / 2);
	if (Globals::Profiles::current->settings.screen.show_borders)
	{
		this->help->borders(Globals::Profiles::current->settings.screen.fancy_borders ?
		                     Window::BORDER_FANCY :
		                     Window::BORDER_REGULAR);
	}
	this->help->setTitle("Help");

}
void LayoutGame::windowsExit()
{
	SAFE_DELETE(this->leftmost);
	SAFE_DELETE(this->hold);
	SAFE_DELETE(this->score);
	SAFE_DELETE(this->middle_left);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->middle_right);
	SAFE_DELETE(this->rightmost);
	SAFE_DELETE(this->pause);
	SAFE_DELETE(this->help);

	for (unsigned int i = 0; i < this->next.size(); i++)
		SAFE_DELETE(this->next[i]);

	this->next.clear(); // clear() as in std::vector

	this->main->clear(); // clear() as in Window
	this->main->refresh(); // clear() as in Window

	Layout::windowsExit();
}
void LayoutGame::draw(Menu* menu)
{
	if (! this->game)
		return;

	// Will only show the requested windows then exit.
	if (this->game->isPaused)
	{
		if (this->game->showPauseMenu)
		{
			this->pause->clear();
			menu->draw(this->pause);
			this->pause->refresh();
		}
		else if (this->game->showHelp)
		{
			this->help->clear();
			this->help->print("Game keys",
			                  this->help->getW()/2 - 9/2, // center
			                  1,
			                  Globals::Profiles::current->settings.theme.hilite_text);

			this->help->print_multiline("Arrow keys     Move piece\n"
			                            "Space bar      Drop piece\n"
			                            "z              Rotate counterclockwise\n"
			                            "x              Rotate clockwise\n"
			                            "c              Hold piece\n"
			                            "q              Quit\n"
			                            "p              Pause/Unpause\n"
			                            "h              Show help",
			                            1,
			                            3,
			                            Globals::Profiles::current->settings.theme.text);
			this->help->refresh();
		}

		// NCURSES NEEDS THIS
		refresh();
		return;
	}

	// First, updating all the container windows
	// (we need to do this otherwise when we draw something
	//  on top of them, they'll be broken with dirty things
	//  left on top)
	this->main->clear();
	this->main->refresh();
	this->leftmost->clear();
	this->leftmost->refresh();
	this->middle_left->clear();
	this->middle_left->refresh();
	this->middle_right->clear();
	this->middle_right->refresh();

	if (Globals::Profiles::current->settings.screen.show_statistics)
	{
		this->rightmost->clear();
		this->rightmost->refresh();
	}

	// Now we'll draw the internal windows

	// Hold piece
	if (Globals::Profiles::current->settings.game.can_hold)
	{
		this->hold->clear();

		if (this->game->pieceHold)
			this->game->pieceHold->draw(this->hold);

		this->hold->refresh();
	}

	// Next pieces
	for (int i = 0; i < Globals::Profiles::current->settings.game.next_pieces; i++)
	{
		Window* w = this->next[i];
		w->clear();

		Piece p(this->game->nextPieces[i], -1, -1);
		if (p.getType() == Piece::O)
			p.moveBy(0, 1);

		p.draw(w);

		w->refresh();
	}

	// Statistics on the left side
	// A mess of direct Ncurses calls - fix this later
	this->score->clear();

	ColorPair hilite = Colors::pair(COLOR_BLUE, COLOR_DEFAULT, true);

	int lowest_y = this->score->getH() - 2; // border

	this->score->print("Hi-Score", 1, lowest_y - 7, hilite);
	this->score->print("Score",    1, lowest_y - 4, hilite);
	this->score->print("Level",    1, lowest_y - 1, hilite);

	// Default color
	wattrset(this->score->win, COLOR_PAIR(0));

	mvwprintw(this->score->win, lowest_y - 6, 1, "%8u", 9000);
	mvwprintw(this->score->win, lowest_y - 3, 1, "%8u", this->game->score->points);
	mvwprintw(this->score->win, lowest_y    , 1, "%8d", this->game->score->level);

	this->score->refresh();

	// Statistics and Misc. info
	// on the right part of the screen
	if (Globals::Profiles::current->settings.screen.show_statistics)
	{
		this->rightmost->clear();

		// User name
		this->rightmost->print(this->game->score->name, this->rightmost->getW() - this->game->score->name.size() - 2, 01, Colors::pair(COLOR_BLUE, COLOR_DEFAULT));

		// Piece and Line Statistics
		this->rightmost->print("[I]", 2, 2, Globals::Profiles::current->settings.theme.piece_I->color);
		this->rightmost->print("[T]", 2, 3, Globals::Profiles::current->settings.theme.piece_T->color);
		this->rightmost->print("[L]", 2, 4, Globals::Profiles::current->settings.theme.piece_L->color);
		this->rightmost->print("[J]", 2, 5, Globals::Profiles::current->settings.theme.piece_J->color);
		this->rightmost->print("[S]", 2, 6, Globals::Profiles::current->settings.theme.piece_S->color);
		this->rightmost->print("[Z]", 2, 7, Globals::Profiles::current->settings.theme.piece_Z->color);
		this->rightmost->print("[O]", 2, 8, Globals::Profiles::current->settings.theme.piece_O->color);

		int x_offset = this->rightmost->getW() - 15;

		this->rightmost->print("Singles", x_offset, 2, hilite);
		this->rightmost->print("Doubles", x_offset, 3, hilite);
		this->rightmost->print("Triples", x_offset, 4, hilite);
		this->rightmost->print("Tetris",  x_offset, 5, hilite);

		this->rightmost->print("Pieces", x_offset, 7, hilite);
		this->rightmost->print("Lines",  x_offset, 8, hilite);

		wattrset(this->rightmost->win, COLOR_PAIR(0));
		mvwprintw(this->rightmost->win, 2, 6, "x %3d", this->game->stats.I);
		mvwprintw(this->rightmost->win, 3, 6, "x %3d", this->game->stats.T);
		mvwprintw(this->rightmost->win, 4, 6, "x %3d", this->game->stats.L);
		mvwprintw(this->rightmost->win, 5, 6, "x %3d", this->game->stats.J);
		mvwprintw(this->rightmost->win, 6, 6, "x %3d", this->game->stats.S);
		mvwprintw(this->rightmost->win, 7, 6, "x %3d", this->game->stats.Z);
		mvwprintw(this->rightmost->win, 8, 6, "x %3d", this->game->stats.O);

		mvwprintw(this->rightmost->win, 2, x_offset + 8, "x %3d", this->game->stats.singles);
		mvwprintw(this->rightmost->win, 3, x_offset + 8, "x %3d", this->game->stats.doubles);
		mvwprintw(this->rightmost->win, 4, x_offset + 8, "x %3d", this->game->stats.triples);
		mvwprintw(this->rightmost->win, 5, x_offset + 8, "x %3d", this->game->stats.tetris);

		mvwprintw(this->rightmost->win, 7, x_offset + 7, "x %4d", this->game->stats.pieces);
		mvwprintw(this->rightmost->win, 8, x_offset + 7, "x %4d", this->game->stats.lines);

		// Timer - how much time has passed since game start
		this->rightmost->print("Timer", 2, 10, hilite);

		long delta_s = this->game->timer.delta_s();

		int seconds = delta_s % 60;
		int minutes = (delta_s / 60) % 60;
		int hours   = ((delta_s / 60) / 60) % 24;

		wattrset(this->rightmost->win, COLOR_PAIR(0));

		mvwprintw(this->rightmost->win, 10, 8, "%02d:%02d:%02d", hours, minutes, seconds);

		// Delay
		this->rightmost->print("Delay", 2, 12, hilite);
		wattrset(this->rightmost->win, COLOR_PAIR(0));

		mvwprintw(this->rightmost->win, 12, 8, "%dms", this->game->getDelay(this->game->score->level));

		// Bottom line - version and Help
		this->rightmost->print("yetris v" VERSION, 1, this->rightmost->getH() - 2, Colors::pair(COLOR_CYAN, COLOR_DEFAULT));

		this->rightmost->print("H", this->rightmost->getW() - 5, this->rightmost->getH() - 2, Colors::pair(COLOR_YELLOW, COLOR_DEFAULT));
		this->rightmost->print("elp", this->rightmost->getW() - 4, this->rightmost->getH() - 2, Colors::pair(COLOR_CYAN, COLOR_DEFAULT));
		this->rightmost->refresh();
	}

	// Board and main game stuff

	// This is an awkward way of clearing lines.
	// We first clear the lines then wait for a little bit.
	// FIXME
	if (this->game->willClearLines)
	{
		this->board->clear();
		this->game->board->draw(this->board);
		this->board->refresh();

		Utils::Time::delay_ms(Globals::Profiles::current->settings.game.line_clear_delay);
	}

	this->board->clear();

	this->game->board->draw(this->board);

	if (Globals::Profiles::current->settings.game.has_ghost)
		this->game->pieceGhost->draw(this->board);

	this->game->pieceCurrent->draw(this->board);

	this->board->refresh();

	// NCURSES NEEDS THIS
	refresh();
}

