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

1. Everything starts at `main.cpp` of course. There we start the `Interface/Ncurses`,
   plus a lot of minor things. Then we simply create and run `Flow/StateManager`.
2. It is responsible to coordinate the main game flow by switching `Flow/GameState`s.
   By default, we instantiate `Flow/GameStateMainMenu`.
3. Inside the main menu we have a `Interface/Layout` with `Interface/Window`s describing
   where to show things. According to user input we can either quit the game or
   go straight to `Flow/GameStateGame`.
4. On it, we create a `Game/GameMode`, with rules on how the different tetris things
   will react to each other. We create most of the `Game/*` classes, leaving the
   control almost exclusively to it.
5. At each frame, a `Interface/LayoutGame*` notices changes on the `Game/GameMode*` and
   draws it on the screen. This way, the game only worries about itself, while the
   layout must keep a watch on it.

