# Source Code README

This file describes how the source code is laid down, which parts are
responsible for what and so on...

**Notes**:

* On the rest of this document,
  I'll call class names like `Directory/ClassName`.

## Summary

This section is very important; if you'll keep only a few things in mind,
read through this and forget everything else.

The whole code is separated between three parts:

1. `main.cpp`: Starting point of the game. This is where every program starts
   (_main()_ function) and serves as a jump point to the game states.
2. `Engine`: _Immutable_ code for the game engine. It has lots of classes that
   take care of the _back-end stuff_ so you can focus on making the game.  If
   something is _not directly related_ to the game (like showing things on the
   screen, handling input, opening files...) then it most likely goes here.
3. `Game`: _Mutable_ code for the game logic. It uses everything on the _Engine_
   part to build the actual game. This is where you define your classes (like
   Player, Enemy, Boss...).

When I say _mutable_/_immutable_ I mean that, for each game you make, you
probably **won't need** to change the code on the _Engine_. On the other hand,
every different game will most likely have a different _Game_ code
hierarchy/classes/stuff.

If you keep the above points in mind then you should have no problem figuring
out the rest of the code hierarchy.

## Directory structure

The files are grouped the same way they relate to each other.

* **Engine**: Immutable code for the program back-end (see section above).
  This has a well-defined hierarchy of things, and you shouldn't modify
  it from game to game.
  * **Graphics**: Stuff that will show things on the screen.
    * **Widgets**: Self-contained ways to show things on the screen.
	  For example, _Dialog_, _Menu_...
  * **Helpers**: Random things that take care of a single task and doesn't
    depend on anything else. They can get used anywhere without any
    consequences (for example, a _Timer_ or a _INI_ parser).
  * **Flow**: Handles how the program execution jumps from one point to the
    other. Basically they show you what _Game States_ are and leaves you with
    the task to create your own.
* **Game**: Mutable code for the game logic (see section above).
  It has a flexible hierarchy - basically you define it the way you want.
  The directories _can_ change from game to game.
* **main.cpp**: Entry for the program execution.
  See section above.

## Execution Flow

Here's what happens when you run the game:

1. Everything starts at `main.cpp` of course.
   * There we initialize `Engine/Ncurses`, plus a lot of minor things.
2. Then we create and run `Engine/Flow/StateManager`.
  * It coordinates the main game flow by switching `Game/States/*`s.
    By default, we instantiate `Game/State/GameStateFirstTime`.
	It is always launched whenever we don't find a configuration directory.
3. Then we ask the user for a _Profile_ name and head up to
   `Game/State/GameStateMainMenu`.  On the next times the game is executed, we
   go straight to this game state.
4. Inside the main menu we have a `Engine/Graphics/Layout` with
   `Engine/Graphics/Window`s describing where to show things. Also, we create
   several `Engine/Graphics/Widgets/Menus`, that are ways to interact with the
   user.  According to those we can either quit the game or go straight to
   `Flow/GameStateGame`.
4. On it, we create a `Game/Entities/Game`, that controls how the different
   tetris things will react to each other. We create most of the `Game/Entities/*`
   classes, leaving the control almost exclusively to it.
5. At each frame, a `Game/Display/Layouts/LayoutGame` notices changes on the
   `Game/Entities/Game` and draws it on the screen. This way, the game only
   worries about itself, while the layout must keep a watch on it.

