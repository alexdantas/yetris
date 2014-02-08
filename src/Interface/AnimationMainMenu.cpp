#include <Interface/AnimationMainMenu.hpp>
#include <Interface/Colors.hpp>
#include <Misc/Utils.hpp>

// Local stuff
std::vector<std::string> appearances;

std::string bubble1(".");
std::string bubble2("o");
std::string bubble3("O");
std::string bubble4("0");
std::string bubble5("~");
std::string bubble6("'");

// Bubble

Bubble::Bubble(int x, int y, int delay, std::string appearance):
	x(x),
	y(y),
	delay(delay),
	appearance(appearance)
{
	this->timer.start();
}
void Bubble::update()
{
	if (this->timer.delta_ms() > this->delay)
	{
		this->y--;
		this->timer.start();
	}
}
void Bubble::draw(Window* window)
{
	window->print_multiline(this->appearance,
	                        this->x,
	                        this->y,
	                        Colors::pair(COLOR_MAGENTA, COLOR_DEFAULT));
}

Bubble* random_bubble(Window* window)
{
	Bubble* b;

	int appearance_index = Utils::Random::between(0, appearances.size() - 1);

	b = new Bubble(Utils::Random::between(0, window->getW() - 2),
	               window->getH(),
	               Utils::Random::between(2, 10) * 100,
	               appearances[appearance_index]);

	return b;
}

// Animation

AnimationMainMenu::AnimationMainMenu(Window* window, int amount):
	bubblesThreshold(amount)
{
	appearances.push_back(bubble1);
	appearances.push_back(bubble2);
	appearances.push_back(bubble3);
	appearances.push_back(bubble4);
	appearances.push_back(bubble5);
	appearances.push_back(bubble6);

	for (int i = 0; i < (this->bubblesThreshold); i++)
		this->bubbles.push_back(random_bubble(window));
}
AnimationMainMenu::~AnimationMainMenu()
{
	for (unsigned int i = 0; i < (this->bubbles.size()); i++)
		SAFE_DELETE(this->bubbles[i]);

	this->bubbles.clear();
}
void AnimationMainMenu::update(Window* window)
{
	// Deleting all bubbles that are invalid.
	// HOORAY for C++11!
	auto i = std::begin(this->bubbles);

	while (i != std::end(this->bubbles))
	{
		if ((*i)->y <= 0)
			i = this->bubbles.erase(i);
		else
			++i;
	}

	// Adding new Bubbles if a handful have disappeared.
	while ((int)this->bubbles.size() < (this->bubblesThreshold))
		this->bubbles.push_back(random_bubble(window));

	for (unsigned int j = 0; j < (this->bubbles.size()); j++)
		this->bubbles[j]->update();
}
void AnimationMainMenu::draw(Window* window)
{
	window->clear();

	for (unsigned int i = 0; i < (this->bubbles.size()); i++)
		this->bubbles[i]->draw(window);

	window->refresh();
}

