#ifndef PIECEGHOST_H_DEFINED
#define PIECEGHOST_H_DEFINED

#include <Game/Entities/Piece.hpp>
#include <Game/Entities/Board.hpp>

/// Special type of Piece, that shows
/// where other Piece will land, by always being at the
/// floor of a Board.
///
class PieceGhost: public Piece
{
public:
	PieceGhost();
	virtual ~PieceGhost() { };

	/// Makes the ghost piece mimic the current position
	/// of the #master piece, dropping completely on #board.
	void update(Piece* master, Board* board);
};

#endif //PIECEGHOST_H_DEFINED

