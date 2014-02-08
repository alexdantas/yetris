#include <Interface/LayoutGameModeSurvival.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

LayoutGameModeSurvival::LayoutGameModeSurvival(GameModeSurvival* game, int width, int height):
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
LayoutGameModeSurvival::~LayoutGameModeSurvival()
{
	this->windowsExit();
}
void LayoutGameModeSurvival::windowsInit()
{
	// We'll start all the windows inside the Layout

	// Leftmost window
	this->leftmost = new Window(this->main,
	                            0,
	                            0,
	                            4*2 + 2, // width of a piece
	                            0);

	if (Globals::Game::can_hold)
	{
		this->hold = new Window(this->leftmost,
		                        0,
		                        0,
		                        this->leftmost->getW(),
		                        4);

		if (Globals::Screen::show_borders)
		{
			this->hold->borders(Globals::Screen::fancy_borders ?
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


	if (Globals::Screen::show_borders)
	{
		this->score->borders(Globals::Screen::fancy_borders ?
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

	if (Globals::Screen::show_borders)
	{
		this->middle_left->borders(Globals::Screen::fancy_borders ?
		                           Window::BORDER_FANCY :
		                           Window::BORDER_REGULAR);
	}
	this->middle_left->refresh();

	// The actual game border, inside that container.
	this->board = new Window(this->middle_left, 0, 0, 0, 0);

	// Middle-right, container of all the next pieces.
	// It's height depents on how many next pieces we're showing.
	int middle_right_height = Globals::Game::next_pieces * 3 + 1;

	this->middle_right = new Window(this->main,
	                                this->middle_left->getX() + this->middle_left->getW(),
	                                0,
	                                4*2 + 2, // 4 blocks + borders
	                                middle_right_height);

	if (Globals::Screen::show_borders)
	{
		this->middle_right->borders(Globals::Screen::fancy_borders ?
		                            Window::BORDER_FANCY :
		                            Window::BORDER_REGULAR);
	}
	this->middle_right->setTitle("Next");
	this->middle_right->clear();
	this->middle_right->refresh();

	// First next piece
	this->next.resize(Globals::Game::next_pieces);
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

	// Container for Statistics and Miscellaneous information.
	this->rightmost = new Window(this->main,
	                             this->middle_right->getX() + this->middle_right->getW(),
	                             0,
	                             this->main->getW() - (this->middle_right->getX() + this->middle_right->getW()) - 1,
	                             0);

	if (Globals::Screen::show_borders)
	{
		this->rightmost->borders(Globals::Screen::fancy_borders ?
		                         Window::BORDER_FANCY :
		                         Window::BORDER_REGULAR);
	}
	this->rightmost->setTitle("Statistics");
	this->rightmost->clear();
	this->rightmost->refresh();
}
void LayoutGameModeSurvival::windowsExit()
{
	SAFE_DELETE(this->main);

	SAFE_DELETE(this->leftmost);
	SAFE_DELETE(this->hold);
	SAFE_DELETE(this->middle_left);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->middle_right);
	SAFE_DELETE(this->rightmost);
	SAFE_DELETE(this->rightmost);

	for (unsigned int i = 0; i < this->next.size(); i++)
		SAFE_DELETE(this->next[i]);

	this->next.clear();
}
void LayoutGameModeSurvival::draw()
{
	if (! this->game)
		return;

	this->board->clear();

	this->game->board->draw(this->board);

	this->game->pieceGhost->draw(this->board);
	this->game->pieceCurrent->draw(this->board);

	this->board->refresh();

	// Awkward way of clearing lines.
	// We first clear the lines then wait for a little bit.
	if (this->game->willClearLines)
	{
		this->board->clear();
		this->game->board->draw(this->board);
		this->board->refresh();

		Utils::delay_us(Globals::Game::line_clear_timeout_us);
	}

	// Hold piece
	if (this->game->pieceHold)
	{
		this->hold->clear();
		this->game->pieceHold->draw(this->hold);
		this->hold->refresh();
	}

	// Next pieces
	for (int i = 0; i < Globals::Game::next_pieces; i++)
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
	mvwprintw(this->score->win, lowest_y    , 1, "%8d", 1);

	this->score->refresh();

	// Statistics and Misc. info
	// on the right part of the screen
	if (Globals::Screen::show_statistics)
	{
		this->rightmost->clear();

		// User name
		this->rightmost->print(this->game->score->name, this->rightmost->getW() - this->game->score->name.size() - 2, 01, Colors::pair(COLOR_BLUE, COLOR_DEFAULT));

		// Piece and Line Statistics
		this->rightmost->print("[I]", 2, 2, Globals::Theme::piece_I->color);
		this->rightmost->print("[T]", 2, 3, Globals::Theme::piece_T->color);
		this->rightmost->print("[L]", 2, 4, Globals::Theme::piece_L->color);
		this->rightmost->print("[J]", 2, 5, Globals::Theme::piece_J->color);
		this->rightmost->print("[S]", 2, 6, Globals::Theme::piece_S->color);
		this->rightmost->print("[Z]", 2, 7, Globals::Theme::piece_Z->color);
		this->rightmost->print("[O]", 2, 8, Globals::Theme::piece_O->color);

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

		mvwprintw(this->rightmost->win, 7, x_offset + 8, "x %3d", this->game->stats.pieces);
		mvwprintw(this->rightmost->win, 8, x_offset + 8, "x %3d", this->game->stats.lines);

		// Timer - how much time has passed since game start
		this->rightmost->print("Timer", 2, 10, hilite);

		long delta_s = this->game->timer.delta_s();

		int seconds = delta_s % 60;
		int minutes = (delta_s / 60) % 60;
		int hours   = ((delta_s / 60) / 60) % 24;

		wattrset(this->rightmost->win, COLOR_PAIR(0));

		mvwprintw(this->rightmost->win, 10, 8, "%02d:%02d:%02d", hours, minutes, seconds);

		// Bottom line - version and Help
		this->rightmost->print("yetris v" VERSION, 1, this->rightmost->getH() - 2, Colors::pair(COLOR_CYAN, COLOR_DEFAULT));

		this->rightmost->print("H", this->rightmost->getW() - 5, this->rightmost->getH() - 2, Colors::pair(COLOR_YELLOW, COLOR_DEFAULT));
		this->rightmost->print("elp", this->rightmost->getW() - 4, this->rightmost->getH() - 2, Colors::pair(COLOR_CYAN, COLOR_DEFAULT));
		this->rightmost->refresh();
	}
}

