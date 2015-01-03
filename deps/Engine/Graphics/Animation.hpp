#ifndef ANIMATION_H_DEFINED
#define ANIMATION_H_DEFINED

#include <Engine/Graphics/Window.hpp>

/// Animations that can happen on any Window you want.
/// You just create a Window, select an Animation and throw it
/// inside - done!
///
/// You can think of these as screensavers. Put them on
/// the background of the game or even on top of anything.
///
/// For example:
///
///     Window*    window = new Window(0, 0, 20, 20);
///     Animation* animation = new Animation(window);
///     animation->load();
///     // Once per frame
///     animation->update();
///     animation->draw();
///
/// This is an abstract interface to any kind of Animation.
///
/// If you want to create your own you should subclass this and
/// override all these methods. Follow the examples seen on the
/// existing animations.
///
class Animation
{
public:
	/// Creates an Animation that will occur on #window.
	Animation(Window* window):
		window(window)
	{ };

	virtual ~Animation() {};

	/// Loads all internal things.
	virtual void load() = 0;

	/// Updates Animation's internal state.
	virtual void update() = 0;

	/// Shows Animation on the screen.
	virtual void draw() = 0;

protected:
	Window* window;
};

#endif //ANIMATION_H_DEFINED

