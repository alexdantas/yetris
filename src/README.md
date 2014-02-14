# yetris source code

The source is divided into the following directories:

| Folder      | Responsibility |
| :---------: | :------------: |
| `Config`    | Settings for the entire program, command-line argument switches and configuration file parsing. |
| `Flow`      | Controls the execution flow of the game - which actions changes the `GameState`s and such. |
| `Game`      | The actual *tetris* game logic, with `Piece`, `Board` and `GameMode`s' rules. |
| `Interface` | How the game is shown on the screen. Specific `Ncurses` things and `Layout`s. |
| `Misc`      | Things that are not tied directly to the game. Tools used here and there that, generally, could be copied and pasted on any project without modification. |

Notes:

* On the rest of this document,
  I'll call class names like `Folder/ClassName`.

## Game Architecture

`yetris` has a lot of classes.
It aims to be as flexible as possible, with lots of settings, game modes and layouts.

**(TODO: Include here an UML diagram)**

## Execution Flow

Here's what happens when you run `yetris`:

1. Everything starts at `main.cpp` of course.
   There we initialize the `Interface/Ncurses`, plus a lot of minor things.
   Then we simply create and run `Flow/StateManager`.
2. It is responsible to coordinate the main game flow by
   switching `Flow/GameState`s.
   By default, we instantiate `Flow/GameStateFirstTime`. It is always launched
   whenever we don't find a configuration directory.
3. Then we ask the user for a Profile name and head up to `Flow/GameStateMainMenu`.
   On the next times the game is executed, we go straight to this game state.
4. Inside the main menu we have an `Interface/Layout` with `Interface/Window`s
   describing where to show things. Also, we create several `Interface/Menus`,
   that are ways to interact with the user.
   According to those we can either quit the game or go straight to `Flow/GameStateGame`.
4. On it, we create a `Game/Game`, that controls how the different tetris things
   will react to each other. We create most of the `Game/*` classes, leaving the
   control almost exclusively to it.
5. At each frame, an `Interface/LayoutGame` notices changes on the `Game/Game` and
   draws it on the screen. This way, the game only worries about itself, while the
   layout must keep a watch on it.

