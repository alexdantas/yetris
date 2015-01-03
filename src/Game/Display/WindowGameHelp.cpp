#include <Game/Display/WindowGameHelp.hpp>
#include <Game/Entities/Profile.hpp>
#include <Engine/Graphics/Layout.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Game/Config/Globals.hpp>
#include <Engine/InputManager.hpp>

WindowGameHelp::WindowGameHelp()
{
	int width  = 50;
	int height = 20;

	int windowx = Layout::screenWidth/2  - width/2;
	int windowy = Layout::screenHeight/2 - height/2;

	this->main = new Window(windowx,
	                        windowy,
	                        width,
	                        height);

	Window* win;

	// Help
	win = new Window(this->main, 0, 0, WINDOW_FILL, WINDOW_FILL);
	win->borders(Window::BORDER_NONE);
	this->windows.push_back(win);

	win = new Window(this->main, 0, 0, WINDOW_FILL, WINDOW_FILL);
	win->borders(Window::BORDER_NONE);
	this->windows.push_back(win);
}
void WindowGameHelp::run()
{
	int activatedIndex = 0;

	while (true)
	{
		// Refreshing Windows
		this->main->clear();
		this->windows[activatedIndex]->clear();

		this->main->print(((activatedIndex == 0) ?
		                   "(Help)" :
		                   " Help "),
		                  2,
		                  0,
		                  ((activatedIndex == 0) ?
		                   EngineGlobals::Theme::hilite_hilite_text :
		                   EngineGlobals::Theme::text));

		this->main->print(((activatedIndex == 1) ?
		                   "(Credits)" :
		                   " Credits "),
		                  12,
		                  0,
		                  ((activatedIndex == 1) ?
		                   EngineGlobals::Theme::hilite_hilite_text :
		                   EngineGlobals::Theme::text));

		// HACK TO AVOID THE BORDERS FROM BEING
		// BOLD. GOTTA SOLVE THIS MISTERY.
		Colors::pairActivate(this->main->win, EngineGlobals::Theme::text);

		// Help Window
		if (activatedIndex == 0)
		{
			this->windows[0]->print(Utils::String::split("yetris: customizable Tetris(tm) clone\n"
			                                             "        for the terminal.\n", '\n'),
			                        1, 1,
			                        EngineGlobals::Theme::text);

			this->windows[0]->print("Controls:", 1, 4, EngineGlobals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split(" Move Left\n"
			                                             " Move Right\n"
			                                             " Soft Drop\n"
			                                             " Hard Drop\n"
			                                             " Hold Piece\n"
			                                             " Rotate Clockwise\n"
			                                             " Rotate Counterclockwise\n"
			                                             " Show Help\n"
			                                             " Pause\n"
			                                             " Quit Anytime\n", '\n'),
			                        11, 4,
			                        EngineGlobals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split(InputManager::keyToString(InputManager::getBind("left")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("right")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("down")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("drop")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("hold")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("rotate_clockwise")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("rotate_counterclockwise")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("rotate_180")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("help")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("pause")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("quit")) + "\n", '\n'),
			                        37, 4,
			                        EngineGlobals::Theme::text);

			this->windows[0]->print("Your settings and scores are stored at:",
			                        1, 16,
			                        EngineGlobals::Theme::text);

			this->windows[0]->print("~/.local/share/yetris/" + Globals::Profiles::current->name,
			                        0, 17,
			                        EngineGlobals::Theme::hilite_text);
		}
		//
		// Credits
		else if (activatedIndex == 1)
		{
			this->windows[1]->print(Utils::String::split(" ### ### ####### ####### ######  ####### #####\n"
			                                             " ### ### ####      ###   ### ###   ###  ### ###\n"
			                                             "  #####  ######    ###   ### ###   ###   ###\n"
			                                             "   ###   ####      ###   ######    ###     ###\n"
			                                             "   ###   ####      ###   ### ###   ###  ### ###\n"
			                                             "   ###   #######   ###   ### ### ####### #####\n", '\n'),
			                        0, 0, EngineGlobals::Theme::hilite_text);

			this->windows[1]->print(Utils::String::split(" v" VERSION " (built " DATE ")\n", '\n'),
			                        24, 6, EngineGlobals::Theme::hilite_hilite_text);

			this->windows[1]->print(Utils::String::split("yetris was created by Alexandre Dantas,\n"
			                                             "contact him at <eu@alexdantas.net>\n"
			                                             "\n"
			                                             "Thanks for playing this game :)\n"
			                                             "\n"
			                                             "Homepage: http://yetris.alexdantas.net/\n"
			                                             "\n"
			                                             "Source code and other contributors:\n"
			                                             "https://github.com/alexdantas/yetris/\n"
			                                             "\n"
			                                             "Support free software! http://www.gnu.org/", '\n'),
			                        1, 7, EngineGlobals::Theme::text);
		}

		this->windows[activatedIndex]->refresh();
		this->main->refresh();
		refresh();

		// Getting Input
		InputManager::update();

		if (InputManager::isPressed("left") || // user-defined
		    InputManager::isPressed(KEY_LEFT))
		{
			activatedIndex--;
			if (activatedIndex < 0)
				activatedIndex = 0;
		}
		else if (InputManager::isPressed("right") || // user-defined
		         InputManager::isPressed(KEY_RIGHT))
		{
			activatedIndex++;
			if (activatedIndex > 1)
				activatedIndex = 1;
		}
		else if (InputManager::isPressed("quit") ||
		         InputManager::isPressed(KEY_ENTER) ||
		         InputManager::isPressed('\n'))
			return;
	}
}

