#ifndef GAMEMODE_H_DEFINED
#define GAMEMODE_H_DEFINED

/// Abstract interface to any kind of game mode.
///
/// Game modes are ways to determine specific rules for
/// a Tetris game.
///
/// Like, make the Board react in an specific way or
/// create a time limit.
///
class GameMode
{
public:
	GameMode();
	virtual ~GameMode();

	/// Starts (and restarts) the game.
	///
	/// @note Should be safe to be called several times.
	virtual void start() = 0;

	/// Uses #c to control the game.
	virtual void handleInput(int c) = 0;

	/// Updates internal game state.
	virtual void update() = 0;

	/// Tells if the game ended.
	virtual bool isOver() = 0;

	/// Tells that we need to quit right now.
	virtual bool willQuit() = 0;
};

#endif //GAMEMODE_H_DEFINED

