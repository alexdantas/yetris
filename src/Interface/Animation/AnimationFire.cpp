#include <Interface/Animation/AnimationFire.hpp>
#include <Interface/Colors.hpp>
#include <Misc/Utils.hpp>

ColorPair red        = Colors::pair(COLOR_RED, COLOR_DEFAULT);
ColorPair redBold    = Colors::pair(COLOR_RED, COLOR_DEFAULT, true);
ColorPair white      = 0;
ColorPair yellow     = Colors::pair(COLOR_YELLOW, COLOR_DEFAULT);
ColorPair yellowBold = Colors::pair(COLOR_YELLOW, COLOR_DEFAULT, true);

AnimationFire::AnimationFire(Window* window):
	Animation(window),
	particle(nullptr)
{ }
AnimationFire::~AnimationFire()
{
	SAFE_DELETE(particle);
}
void AnimationFire::load()
{
	particle = new Array2D<ParticleFire>(window->getW(),
	                                           window->getH());

	for (unsigned int i = 0; i < (particle->width()); i++)
	{
		for (unsigned int j = 0; j < (particle->height()); j++)
		{
			particle->at(i, j).x = Utils::Random::between(0, particle->width());
			particle->at(i, j).y = Utils::Random::between(0, particle->height());
		}
	}

	timer.start();
}
void AnimationFire::update()
{
	// Updating only at the right time!
	if (timer.delta_ms() < 300)
		return;

	// How fast the fire cools down each frame
	int cooling_ratio = Utils::Random::between(INTENSITY_PERCENT(3),
	                                           INTENSITY_PERCENT(16));

	// Spawning high-intensity flames on the bottom particles
	for (unsigned int i = 0; i < (particle->width()-1); i++)
		particle->at(i, particle->height() - 1).intensity = Utils::Random::between(INTENSITY_PERCENT(90), INTENSITY_MAX);

	for (unsigned int i = 0; i < (particle->width()-1); i++)
	{
		for (unsigned int j = 0; j < (particle->height()-1); j++)
		{
			// Cooling all particles based on the ones below
			particle->at(i, j).intensity = particle->at(i, j + 1).intensity - cooling_ratio;
		}
	}

	timer.start();
}
void AnimationFire::draw()
{
	for (unsigned int i = 0; i < (particle->width()); i++)
	{
		for (unsigned int j = 0; j < (particle->height()); j++)
		{
			int       c = ' ';
			ColorPair p = white;
			int       s = particle->at(i, j).intensity;

			if (s > INTENSITY_PERCENT(90))
			{
				c = '#';
				p = white;
			}
			else if (s > INTENSITY_PERCENT(80))
			{
				c = '@';
				p = yellowBold;
			}
			else if (s > INTENSITY_PERCENT(70))
			{
				c = '%';
				p = yellow;
			}
			else if (s > INTENSITY_PERCENT(60))
			{
				c = '*';
				p = redBold;
			}
			else if (s > INTENSITY_PERCENT(50))
			{
				c = '*';
				p = red;
			}
			else if (s > INTENSITY_PERCENT(40))
			{
				c = '"';
				p = red;
			}
			else if (s > INTENSITY_PERCENT(30))
			{
				c = '\'';
				p = red;
			}
			else if (s > INTENSITY_PERCENT(20))
			{
				c = '.';
				p = red;
			}
			else
			{
				// Too low intensity
				continue;
			}
			window->printChar(c, i, j, p);
		}
	}
}

