# Wonky-Kong

## Overview

Wonky Kong is a retro-inspired platformer where players navigate platforms, climb ladders, and dodge hazards like rolling barrels and fireballs while trying to reach Kong. With burp-powered attacks and collectible goodies, players must survive each level and advance through increasingly challenging stages.

## Features

- Classic platformer mechanics: Jump, climb, and move across levels.

- Enemies: Rolling barrels, fireballs, and Koopas that freeze the player.

- Collectibles: Garlic for burp attacks, extra lives, and more.

- Objective: Reach Kong to complete each level while avoiding hazards.

## Installation and Setup

### macOS:

1. Install XQuartz:

```
brew install --cask xquartz
```

2. Log out and log back in to apply changes.

3. Verify installation:

```
echo $DISPLAY
```

The output should end with `org.xquartz:0`.

4. Open a Terminal window and navigate to the folder:

```
cd /path/to/WonkyKong
```

Replace `/path/to/WonkyKong` with the actual path.

5. Confirm you are in the correct folder:

```
ls
```

You should see the `Assets` folder and the `WonkyKong` executable.

6. Run the game:

```
./WonkyKong
```

### Linux:

1. Install OpenGL and freeGLUT:

```
# Ubuntu
sudo apt install freeglut3-dev

# Fedora
sudo dnf install freeglut-devel
```
2. Ensure X11 is running and DISPLAY is set up:

```
echo $DISPLAY
```

3. Run the program using:

```
./WonkyKong
```

## How to Play

- Move Left/Right: Arrow keys (←, →) or A and D

- Climb Ladders: Arrow keys (↑, ↓) or W and S

- Jump: Spacebar

- Burp Attack: Tab key (requires garlic goodie)

## Contributing

Feel free to contribute by submitting pull requests. Bug reports and feature suggestions are welcome!

## About

This project is from UCLA's CS32 Data Structures and Algorithms class.

