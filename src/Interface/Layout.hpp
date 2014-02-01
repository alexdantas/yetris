#ifndef LAYOUT_H_DEFINED
#define LAYOUT_H_DEFINED

#include <Interface/Window.hpp>

/// Abstract interface for any kind of layout.
///
class Layout
{
public:
	virtual void draw() = 0;

protected:
};

#endif //LAYOUT_H_DEFINED

