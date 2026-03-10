# AABBs Roblox

Enables AABBs by toggling the DebugDrawBroadPhaseAABBs FFlag in Roblox memory.

## Requirements

- Visual Studio or any MSVC-compatible compiler

## Build

Open the project in Visual Studio, make sure you're targeting **Release x64**, then build and run.

## Usage

1. Launch Roblox and join a game (not just the main menu)
2. Run the compiled binary
3. Type "yes" to enable AABBs or "no" to disable it
4. Type "exit" to quit the program

## What it does

Shows bounding boxes around all objects in the game:
- Players, parts, and all physics objects become visible through walls
- Works by enabling Roblox's internal physics debug visualization

## Notes
- Functionality depends on the current FFlag from the server. If the server is down, it uses a fallback FFlag
- FFlags break on Roblox updates; you'll need updated FFlags after each update
- The tool automatically fetches the latest FFlag address from the server
