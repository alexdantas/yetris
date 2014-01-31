#ifndef GAMEMODESURVIVAL_H_DEFINED
#define GAMEMODESURVIVAL_H_DEFINED

#include "GameMode.hpp"
#include "Piece.hpp"
#include "Board.hpp"
#include "RotationSystem.hpp"
#include <vector>
#include "Timer.hpp"

///
class GameModeSurvival: public GameMode
{
public:
	// For documentation, see GameMode.hpp

	GameModeSurvival(LayoutGame* layout);
	~GameModeSurvival() {};

	void start();
	void handleInput(int c);
	void update();
	void draw();
	bool isOver();

	/// Moves a piece only if it's possible within the board.
	void movePieceIfPossible(Piece::PieceDirection direction);

	/// Locks current piece on the board and gets the next one.
	void lockCurrentPiece();

private:
	/// If we'll quit the game right away.
	bool willQuit;

	/// If the game is over (board is full of blocks).
	bool gameOver;

	Piece* pieceCurrent;
	Piece* pieceGhost;
	Piece* pieceHold;

	std::vector<Piece::PieceType> nextPieces;

	Board* board;

	RotationSystem* rotationSystem;

	Timer pieceTimer;
};

#endif //GAMEMODESURVIVAL_H_DEFINED

