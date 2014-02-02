#include <Interface/LayoutGameDefault.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

LayoutGameDefault::LayoutGameDefault(GameModeSurvival* game, int width, int height):
	Layout(width, height),
	game(game),
	board(nullptr),
	info(nullptr),
	help(nullptr),
	leftmost(nullptr),
	middle_left(nullptr),
	middle_right(nullptr),
	rightmost(nullptr),
	next_container(nullptr),
	leftmost_container(nullptr),
	score(nullptr),
	help_container(nullptr),
	hscores_container(nullptr),
	hscores(nullptr),
	hold(nullptr)
{
	this->windowsInit();
}
LayoutGameDefault::~LayoutGameDefault()
{
	this->windowsExit();
}
void LayoutGameDefault::windowsInit()
{
	// We'll start all the windows inside the Layout

	// Leftmost window
	this->leftmost = new Window(this->main,
	                            0,
	                            0,
	                            6*2 + 2,
	                            0);

	if (Globals::Screen::fancy_borders)
	{
		this->leftmost->borders(Window::BORDER_FANCY);

		// If the player has no hold,
		// doesn't make sense printing these parts
		if (Globals::Game::can_hold)
		{
			// making the top line between
			// hold and score windows
			this->leftmost->printChar(ACS_LLCORNER,
			                          0,
			                          5,
			                          Colors::pair(COLOR_WHITE, COLOR_DEFAULT));
			this->leftmost->horizontalLine(1,
			                               5,
			                               ACS_HLINE,
			                               this->leftmost->getW() - 2,
			                               Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));

			this->leftmost->printChar(ACS_LRCORNER,
			                          this->leftmost->getW() - 1,
			                          5,
			                          Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));

			// Making the bottom line
			// between hold and score windows
			this->leftmost->printChar(ACS_ULCORNER,
			                          0,
			                          6,
			                          Colors::pair(COLOR_WHITE, COLOR_DEFAULT, true));

			this->leftmost->horizontalLine(1,
			                               6,
			                               ACS_HLINE,
			                               this->leftmost->getW() - 2,
			                               Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false));

			this->leftmost->printChar(ACS_URCORNER,
			                          this->leftmost->getW() - 1,
			                          6,
			                          Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false));
		}
	}
	else
	{
		this->leftmost->borders(Window::BORDER_FANCY);
		this->leftmost->horizontalLine(1,
		                               5,
		                               '-',
		                               this->leftmost->getW() - 2,
		                               Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));
	}
	this->leftmost->refresh();

	// Middle-left
	this->middle_left = new Window(this->main,
	                               this->leftmost->getX() + this->leftmost->getW(),
	                               1,
	                               10 * 2 + 2,
	                               0);

	if (Globals::Screen::show_borders)
	{
		this->middle_left->borders(Globals::Screen::fancy_borders ?
		                           Window::BORDER_FANCY :
		                           Window::BORDER_REGULAR);
	}
	this->middle_left->refresh();

	// Middle-right
	this->middle_right = new Window(this->main,
	                                this->middle_left->getX() + this->middle_left->getW(),
	                                1,
	                                4 * 2 + 2,
	                                0);

	if (Globals::Screen::fancy_borders)
	{
		this->middle_right->borders(Window::BORDER_FANCY);

		// Making the top line between 1st next and the rest
		this->middle_right->printChar(ACS_LLCORNER,
		                              0,
		                              3,
		                              Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false));

		this->middle_right->horizontalLine(1,
		                                   3,
		                                   ACS_HLINE,
		                                   this->middle_right->getW() - 2,
		                                   Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));

		this->middle_right->printChar(ACS_LRCORNER,
		                              this->middle_right->getW() - 1,
		                              3,

		                              Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));

		// Making the bottom line between 1st next and the rest
		this->middle_right->printChar(ACS_ULCORNER,
		                              0,
		                              4,
		                              Colors::pair(COLOR_WHITE, COLOR_DEFAULT, true));

		this->middle_right->horizontalLine(1,
		                                   4,
		                                   ACS_HLINE,
		                                   this->middle_right->getW() - 2,
		                                   Colors::pair(COLOR_WHITE, COLOR_DEFAULT));

		this->middle_right->printChar(ACS_URCORNER,
		                              this->middle_right->getW() - 1,
		                              4,
		                              Colors::pair(COLOR_WHITE, COLOR_DEFAULT));
	}
	else
	{
		this->middle_right->borders(Window::BORDER_REGULAR);

		// wattrset(w.win, Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));
		// mvwhline(w.win, 3, 1, '-', w->getW() - 2);
	}
	this->middle_right->refresh();

	// Right-most
	this->rightmost = new Window(this->main,
	                             this->middle_right->getX() + this->middle_right->getW(),
	                             1,
	                             this->main->getW() - (this->middle_right->getX() + this->middle_right->getW()) - 1,
	                             0);

	if (Globals::Screen::show_borders)
	{
		this->rightmost->borders(Globals::Screen::fancy_borders ?
		                         Window::BORDER_FANCY :
		                         Window::BORDER_REGULAR);
	}
	this->rightmost->refresh();

	// Next Piece Container
	this->next_container = new Window(this->middle_right,
	                                  1,
	                                  1,
	                                  0,
	                                  0);
	this->next_container->refresh();

	// First next piece
	this->next.resize(Globals::Game::next_pieces);
	this->next[0] = new Window(this->next_container,
	                           0,
	                           0,
	                           this->next_container->getW(),
	                           2);
	this->next[0]->refresh();

	// The rest of the next pieces
	int y_offset = 2;

	for (unsigned int i = 1; i < this->next.size(); i++)
	{
		/* making all the next pieces 1 line lower */
		if (i != 1)
			y_offset = 0;

		this->next[i] = new Window(this->next_container,
		                           0,
		                           this->next[i - 1]->getY() + this->next[i - 1]->getH() + 1 + y_offset,
		                           this->next_container->getW(),
		                           2);

		this->next[i]->refresh();
	}

	this->board = new Window(this->middle_left,
	                         1,
	                         1,
	                         0,
	                         0);

	this->info = new Window(this->rightmost,
	                        2,
	                        1,
	                        this->rightmost->getW() - 4,
	                        0);

	this->leftmost_container = new Window(this->leftmost,
	                                      1,
	                                      1,
	                                      0,
	                                      0);

	this->hold = new Window(this->leftmost_container,
	                        0,
	                        0,
	                        this->leftmost_container->getW(),
	                        4);

	this->score = new Window(this->leftmost_container,
	                         0,
	                         this->hold->getY() + this->hold->getH() + 2,
	                         this->leftmost_container->getW(),
	                         this->leftmost_container->getH() - (this->hold->getH()) - 2);

	/* w->getW()  = s->leftmost_container->getW(); */
	/* w->getH() = s->leftmost_container->getH() - (s->hold->getH()) - 2; */
	/* w->getX()      = 0; */
	/* w->getY()      = s->hold->getY() + s->hold->getH() + 2; */
	/* w.win    = derwin(s->leftmost_container.win, w->getH(), w->getW(), w->getY(), w->getX()); */
	/* wnoutrefresh(w.win); */
	/* s->score = w; */

	this->info->print("Loading",
	                  0,
	                  this->info->getH() - 1,
	                  Colors::pair(COLOR_WHITE, COLOR_DEFAULT, true));
	this->info->refresh();
}
void LayoutGameDefault::windowsExit()
{
	SAFE_DELETE(this->main);

	SAFE_DELETE(this->leftmost);
	SAFE_DELETE(this->middle_left);
	SAFE_DELETE(this->middle_right);
	SAFE_DELETE(this->rightmost);
	SAFE_DELETE(this->next_container);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->info);

	for (unsigned int i = 0; i < this->next.size(); i++)
		SAFE_DELETE(this->next[i]);

	this->next.clear();

	SAFE_DELETE(this->hold);
	SAFE_DELETE(this->leftmost_container);
	SAFE_DELETE(this->score);
	SAFE_DELETE(this->help_container);
	SAFE_DELETE(this->help);
	SAFE_DELETE(this->hscores_container);
	SAFE_DELETE(this->hscores);
}
void LayoutGameDefault::draw()
{
	if (! this->game)
		return;

	this->board->clear();

	this->game->board->draw(this->board);

	this->game->pieceGhost->draw(this->board);
	this->game->pieceCurrent->draw(this->board);

	this->board->refresh();

	if (this->game->pieceHold)
	{
		this->hold->clear();
		this->game->pieceHold->draw(this->hold);
		this->hold->refresh();
	}

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
}

