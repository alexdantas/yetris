#include <Interface/Menu.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

Menu::Menu(int x, int y, int width, int height):
	current(nullptr),
	currentIndex(0),
	first(nullptr),
	last(nullptr),
	x(x),
	y(y),
	width(width),
	height(height),
	selected(false),
	selectedItem(nullptr)
{ }
Menu::~Menu()
{
	for (unsigned int i = 0; i < (this->item.size()); i++)
		SAFE_DELETE(this->item[i]);

	this->item.clear();
}
void Menu::add(MenuItem* item)
{
	this->item.push_back(item);

	unsigned int size = this->item.size();

	// First item added
	if (size == 1)
	{
		this->first = this->item.front();
		this->current = this->item.front();
		this->currentIndex = 0;
	}

	this->last = this->item.back();
}
void Menu::addBlank()
{
	this->item.push_back(nullptr);
}
void Menu::draw(Window* window)
{
	for (unsigned int i = 0; i < (this->item.size()); i++)
	{
		// <rant>
		// THIS IS VERY UGLY
		// HOW CAN I LAY DOWN A CLASS HIERARCHY
		// AND OVERRIDE PARENT FUNCTIONS ON CHILD CLASSES
		// IF WHEN I HAVE A PARENT POINTER I CANT LET THE
		// COMPILER DECIDE WETHER TO CALL PARENT OR CHILD
		// FUNCTIONS?
		// </rant>

//		MenuItemCheckbox* pCheckbox = dynamic_cast<MenuItemCheckbox*>a

		// Blank Menu Item, will show horizontal line
		if (! this->item[i])
		{
			for (int j = 0; j < (this->width); j++)
				window->printChar(((Globals::Screen::fancy_borders) ?
				                   ACS_HLINE :
				                   '-'),
				                  this->x + j,
				                  this->y + i,
				                  Colors::pair(COLOR_WHITE, COLOR_DEFAULT));

			continue;
		}

		if (this->item[i] == this->current)
		{
			this->item[i]->draw(window,
			                    this->x,
			                    this->y + i,
			                    this->width,
			                    true);
		}
		else
		{
			this->item[i]->draw(window,
			                    this->x,
			                    this->y + i,
			                    this->width);
		}
	}
}
void Menu::handleInput(int input)
{
	switch(input)
	{
	case KEY_DOWN:
	case '\t':
		this->goNext();
		break;

	case KEY_UP:
	case KEY_BTAB:
		this->goPrevious();
		break;

	case KEY_HOME:
	case KEY_PPAGE:
		this->goFirst();
		break;

	case KEY_END:
	case KEY_NPAGE:
		this->goLast();
		break;

	case KEY_ENTER:
	case '\n':

		// Will only quit if the selected item is a simple
		// item - more complex ones doesn't quit.
		if (this->current->type == MenuItem::ITEM)
		{
			this->selected = true;
			this->selectedItem = this->current;
		}
		else
			this->current->handleInput(input);
		break;

	default:
		this->current->handleInput(input);
		break;
	}
}
void Menu::goNext()
{
	if (this->current == this->last)
	{
		this->current = this->first;
		this->currentIndex = 0;
	}
	else
	{
		this->currentIndex++;
		this->current = this->item[this->currentIndex];
	}

	if (! this->current)
		this->goNext();
}
void Menu::goPrevious()
{
	if (this->current == this->first)
	{
		this->current = this->last;
		this->currentIndex = (this->item.size() - 1);
	}
	else
	{
		this->currentIndex--;
		this->current = this->item[this->currentIndex];
	}

	if (! this->current)
		this->goPrevious();
}
void Menu::goFirst()
{
	this->current = this->first;
	this->currentIndex = 0;

	if (! this->current)
		this->goNext();
}
void Menu::goLast()
{
	this->current = this->last;
	this->currentIndex = (this->item.size() - 1);

	if (! this->current)
		this->goPrevious();
}
bool Menu::willQuit()
{
	// Will only quit if the user selected an item
	// and the item selected is valid.
	return (this->selected && this->selectedItem);
}
int Menu::getSelectedValue()
{
	if (! this->willQuit())
		return -1;

	return (this->selectedItem->value);
}
bool Menu::getBool(int value)
{
	for (unsigned int i = 0; i < (this->item.size()); i++)
	{
		if (! this->item[i])
			continue;

		if (this->item[i]->value == value)
		{
			// Either the type got messed up or we have
			// two items with the same value.
			if (this->item[i]->type != MenuItem::CHECKBOX)
				return false;

			// This cast may be dangerous if the type was
			// somehow changed.
			MenuItemCheckbox* c = (MenuItemCheckbox*)this->item[i];
			return c->isChecked();
		}
	}
	return false;
}
int Menu::getInt(int value)
{
	for (unsigned int i = 0; i < (this->item.size()); i++)
	{
		if (! this->item[i])
			continue;

		if (this->item[i]->value == value)
		{
			// Either the type got messed up or we have
			// two items with the same value.
			if (this->item[i]->type != MenuItem::NUMBERBOX)
				return -1;

			// This cast may be dangerous if the type was
			// somehow changed.
			MenuItemNumberbox* c = (MenuItemNumberbox*)this->item[i];
			return c->current;
		}
	}
	return -1;
}
void Menu::reset()
{
	this->selected = false;
	this->selectedItem = nullptr;
}


