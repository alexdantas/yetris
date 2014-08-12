#include <Game/Entities/Block.hpp>

Block::Block(ColorPair color, char first, char second):
	color(color)
{
	this->appearance[0] = first;
	this->appearance[1] = second;
}

void Block::draw(Window* win, int x, int y)
{
	if (!win)
		return;

	win->printChar(this->appearance[0], x,     y, this->color);
	win->printChar(this->appearance[1], x + 1, y, this->color);
}

