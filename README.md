# Soccer-Game-AI
Autonomous 2D top-down soccer simulation built in C++ with SDL2, inspired by the architecture from *Programming Game AI by Example*.

This project demonstrates coordinated multi-agent behavior, team-state logic, ball physics, and a complete playable match loop (kickoff, goals, score tracking, winner detection).

## Match Preview

https://github.com/user-attachments/assets/3701d5b8-477b-4c7f-b7d7-349dea2c2631



## What This Project Demonstrates
- Autonomous gameplay with no player input required.
- Team-level and player-level decision flow using finite state machines.
- Real-time role transitions (chase, support, receive, return home, defend/attack switching).
- Ball possession and scoring lifecycle (goal detection, reset, kickoff preparation).
- Visual game presentation with a rendered pitch, team entities, keeper behavior, and top-center HUD scoreboard.

## Core Systems
### AI and Decision Model
- **Team FSM**: `PrepareForKickOff`, `Attacking`, `Defending`.
- **Player FSM**: `Wait`, `ReturnToHomeRegion`, `ChaseBall`, `Dribble`, `ReceiveBall`, `SupportAttacker`.
- **Role selection**: Closest-player-to-ball updates each frame to drive who pressures the ball.

### Physics and Movement
- Vector-based movement with steering behaviors (`seek`, `arrive`, `pursuit`).
- Ball movement with friction and velocity decay.
- Post-update overlap resolution to prevent players and keepers from visually passing through each other.

### Match Flow
- Real-time score tracking (Red vs Blue).
- Automatic reset after non-winning goals.
- Game freeze when a team reaches winning score.
- Winner highlighting in the HUD.

### Visual and Audio Layer
- Procedural pitch rendering (gradient grass, stripes, center line/circle, boxes, goal visuals).
- Stylized procedural players/keepers/ball for clear tactical readability.
- Optional looping crowd ambience from WAV file.

## Project Structure
```text
include/   Headers (entities, states, math, FSM, messaging)
src/       C++ implementation (game loop, pitch, teams, players, ball, rendering)
imgs/      Optional image assets
screenshots/
```

## Build and Run
### Requirements
- `g++` with C++17 support
- SDL2 development libraries
- SDL2_image development libraries

### Commands
```bash
make
make run
```


## Current Scope
This repository focuses on simulation architecture, autonomous flow, and readability of game AI behavior. It is intentionally lightweight and suitable as a foundation for:
- richer tactical logic,
- stronger passing/shooting heuristics,
- animation and polish,
- and deeper match analytics.
