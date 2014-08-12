#ifndef BLOCK_H_DEFINED
#define BLOCK_H_DEFINED

#include <Engine/Graphics/Colors.hpp>
#include <Engine/Graphics/Window.hpp>

/// How a block is shown on the screen.
///
struct Block
{
	ColorPair color;

	/// How it will be shown on the screen.
	///
	/// For example, "[]" could mean a nice block,
	/// such as "()" or "{}" or even "##".
	///
	char appearance[2];

	Block(ColorPair color, char first, char second);

	void draw(Window* win, int x, int y);
};

#endif //BLOCK_H_DEFINED

