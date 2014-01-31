#ifndef ROTATIONSYSTEM_H_DEFINED
#define ROTATIONSYSTEM_H_DEFINED

#include "Piece.hpp"
#include "Board.hpp"

/// Abstract interface to rules on how to rotate pieces.
class RotationSystem
{
public:
	virtual ~RotationSystem() {};

	virtual void rotate(Piece* piece, Board* board, short rotation) = 0;
};

#endif //ROTATIONSYSTEM_H_DEFINED

