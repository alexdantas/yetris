#ifndef PIECE_H_DEFINED
#define PIECE_H_DEFINED

#include <Game/Entities/Block.hpp>

/// Represents a Tetromino - piece with usually 4 blocks.
///
class Piece
{
	/// We need to do this so the RotationSystem can
	/// work right.
	friend class RotationSystem;

public:
	enum PieceType
	{
		O = 0,
		I,
		L,
		J,
		Z,
		S,
		T,
		MAX,
	};
	enum PieceDirection
	{
		DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
	};

	/// Returns a random PieceType.
	static PieceType random();

	/// Creates a piece at #x #y according to it's #type.
	Piece(PieceType type, int x, int y);

	void draw(Window* win);

	// state-modifying methods
	void rotate(short rotation);
	void move(PieceDirection direction);
	void moveBy(int dx, int dy);
	void moveTo(int x, int y);

	// query methods
	int getX();
	int getY();
	Block* getBlock();
	PieceType getType();
	short getRotation();

	// Hacks that need to be fixed
	bool isVisible;

protected:
	PieceType type;

	// HACK to make piece invisible right before locking
public:
	Block* block;

protected:
	int x;
	int y;

	short rotation;
};

#endif //PIECE_H_DEFINED

