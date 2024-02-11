# SEA BATTLE
This application is a classic game "Sea Battle" created using SFML 2.5.1 crossplatform graphic library.

# Rules
The playing field has a size of 10x10 cells. Each player places their ships on their side of the game board. Ships can be of different lengths, but should not overlap or touch each other.

When the players have finished placing the ships, the players begin to attack the enemy ships in turn. Each player chooses a cell on the board that he wants to shoot at, and reports the address of this cell to the opponent.

If a shot hits one of the opponent's ship cells, the opponent reports it, and the attacking player marks the cell on his side of the board as a "hit". If the enemy ship is destroyed, the player marks all the cells of the ship as "sunk".

If the shot misses, the attacking player marks the cell on his side of the board as a "miss". Players continue to attack in turn until one of the players destroys all the enemy ships.

The player who first destroys all the enemy ships wins the game.

## Functional
The user is playing against the computer.
Before starting the game, the user places his ships on the field by dragging. In order to change the orientation of the ship, you need to press the PCM on it, while holding its LMB. In addition, the user is given the opportunity to automatically arrange their ships. To do this, click the appropriate button. When all the ships are placed, you need to press the "Start" button - the game will begin

## Build & run
To build amd run this applications you can use CMakeLists.txt from this repositiry.
To download app on your PC, you can download release from 'release_branch' and install it:
```
sudo dpkg -i SEA_BATTLE-1.0.0-Linux.deb
```
To ensure correct operation of the app use this command:
```
sudo sea_battle
```

## Demo
![Alt text](https://github.com/SerpentDragon/SeaBattle-SFML/blob/master/Demo/demo.png)