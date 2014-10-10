#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

#include <Game/Entities/Piece.hpp>
#include <Game/Entities/Board.hpp>
#include <Game/Entities/RotationSystem.hpp>
#include <Game/Entities/Statistics.hpp>
#include <Game/Entities/PieceGhost.hpp>
#include <Engine/Helpers/Timer.hpp>
#include <Engine/Graphics/Widgets/Menu.hpp>

#include <vector>

/// Pre-defining it's layout to avoid circular dependency.
class LayoutGame;

/// All the rules and behaviour of a T*tris game.
///
class Game
{
	friend class LayoutGame;

public:
	Game();
	virtual ~Game() {};

	void start();
	void handleInput();
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

	/// If we'll return to the main menu.
	bool willReturnToMenu();

	/// Returns the game level for how many #lines were cleared.
	int getLevel(int lines);

	/// Returns how much time (millisseconds) we need to wait
	/// for the piece to fall for a specific #level.
	int getDelay(int level);

	void pause(bool option);

protected:
	LayoutGame* layout;

	/// If the game is over (board is full of blocks).
	bool gameOver;

	bool userAskedToQuit;
	bool userAskedToGoToMenu;

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

	/// If the game is paused.
	/// May show other Windows while paused.
	bool isPaused;

	/// If it's showing the pause menu.
	/// Goes together with #isPaused.
	bool showPauseMenu;

	/// If it's showing the help screen.
	/// Goes together with #isPaused.
	bool showHelp;

	/// Menu that's shown only when the user presses Pause.
	Menu* pauseMenu;
};

#endif //GAME_H_DEFINED

