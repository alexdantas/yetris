#ifndef ROTATIONSYSTEMSRS_H_DEFINED
#define ROTATIONSYSTEMSRS_H_DEFINED

#include <Game/Entities/RotationSystem.hpp>

///
class RotationSystemSRS: public RotationSystem
{
public:
	virtual ~RotationSystemSRS();

	void rotate(Piece* piece, Board* board, short rotation);
};

#endif //ROTATIONSYSTEMSRS_H_DEFINED

