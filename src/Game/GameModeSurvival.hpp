#ifndef GAMEMODESURVIVAL_H_DEFINED
#define GAMEMODESURVIVAL_H_DEFINED

#include <Game/GameMode.hpp>
#include <Game/Piece.hpp>
#include <Game/Board.hpp>
#include <Game/RotationSystem.hpp>
#include <Game/Statistics.hpp>
#include <Game/PieceGhost.hpp>
#include <Game/Score.hpp>
#include <Misc/Timer.hpp>

#include <vector>

///
class LayoutGameModeSurvival;

///
class GameModeSurvival: public GameMode
{
	friend class LayoutGameModeSurvival;

public:
	// For documentation, see GameMode.hpp

	GameModeSurvival();
	~GameModeSurvival() {};

	void start();
	void handleInput(int c);
	void update();
	void draw();

	bool isOver();

	/// Moves a piece only if it's possible within the board.
	/// Returns if moved.
	bool movePieceIfPossible(Piece::PieceDirection direction);

	/// Returns the next piece on the line,
	/// bumping all the subsequent Pieces on the array.
	///
	Piece* getNextPiece();

	/// Locks current piece on the board and gets the next one.
	virtual void lockCurrentPiece();

	/// Holds current piece and gets the next accordingly.
	void holdCurrentPiece();

	/// If we'll quit the game right away.
	bool willQuit();

	/// Returns the game level for how many #lines were cleared.
	int getLevel(int lines);

	/// Returns how much time (millisseconds) we need to wait
	/// for the piece to fall for a specific #level.
	int getDelay(int level);

protected:
	LayoutGameModeSurvival* layout;

	/// If the game is over (board is full of blocks).
	bool gameOver;

	bool userAskedToQuit;

	Piece* pieceCurrent;
	PieceGhost* pieceGhost;
	Piece* pieceHold;

	/// All the next pieces on the line.
	std::vector<Piece::PieceType> nextPieces;

	/// Where the current piece exists and the blocks form lines.
	Board* board;

	/// Defines how the current piece rotates on the Board.
	RotationSystem* rotationSystem;

	/// Timer that tells when to force the piece down.
	Timer timerPiece;

	/// Tells if on this frame we've moved a piece down.
	///
	/// That's needed so when the player move a piece
	/// the drop theme restarts.
	/// But only once each drop (otherwise the player
	/// could keep the piece on the air forever).
	///
	bool movedPieceDown;

	/// Information on the current status of the game.
	Statistics stats;

	/// Tells if the player can hold a piece right now.
	bool canHold;

	bool willClearLines;

	// Things related to the invisible game mode
	Timer timerInvisible;
	bool isInvisible;

	// Times how long the user have been playing the game.
	Timer timer;

	Score* score;

	/// If the game is paused.
	/// May show other Windows while paused.
	bool isPaused;

	/// If it's showing the pause menu.
	/// Goes together with #isPaused.
	bool showPauseMenu;

	/// If it's showing the help screen.
	/// Goes together with #isPaused.
	bool showHelp;
};

#endif //GAMEMODESURVIVAL_H_DEFINED

