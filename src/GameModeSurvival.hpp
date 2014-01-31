#ifndef GAMEMODESURVIVAL_H_DEFINED
#define GAMEMODESURVIVAL_H_DEFINED

#include "GameMode.hpp"
#include "Piece.hpp"
#include "Board.hpp"
#include "RotationSystem.hpp"
#include <vector>
#include "Timer.hpp"
#include "Statistics.hpp"

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

	Piece* getNextPiece(int x, int y);

	/// Locks current piece on the board and gets the next one.
	void lockCurrentPiece();

	void holdCurrentPiece();

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

	/// Tells if on this frame we've moved a piece down.
	///
	/// That's needed so when the player move a piece
	/// the drop theme restarts.
	/// But only once each drop (otherwise the player
	/// could keep the piece on the air forever).
	///
	bool movedPieceDown;

	Statistics stats;

	bool canHold;
};

#endif //GAMEMODESURVIVAL_H_DEFINED

