# Physics-Simulation

A really simple program that uses SDL library to simulate cannonballs being fired and colliding and bouncing off of each other.

I used it to test the waters on using SDL for graphics, and collisions. Putting it on GitHub so that I may easily reference it later.

There are a few branches of this that use different programming languages.

# License

This program has been released in the Public Domain meaning you can use it however you wish, read the license file [here](https://github.com/Dragon-Wonder/Physics-Collision-Simulator/blob/master/docs/LICENSE.md) 
or you can read [here](http://unlicense.org) for information of the Unlicense in general.

## Summary of what license means

* First and foremost, this code is "AS IS", meaning we cannot be sued if it doesn't work

* You can modify this code any way you wish without credit (but we would certainty appreciate it)

* You can use any/all of this code for private and/or commercial use

# Building and Running

## Windows
In order to run the program properly you will need to download SDL2 and SDL2-image. 

A link for SDL2 can be found [here](https://www.libsdl.org/download-2.0.php).

A link for SDL2 image can be found [here](https://www.libsdl.org/projects/SDL_image/) .

When all is said and done you should have the following files in your folder.

```
FolderName\
FolderName\physics-simulation.exe
FolderName\SDL2.dll
FolderName\SDL2_image.dll
```

## Linux
To build on Linux, you will need the SDL2 and SDL2_image development libraries.

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev
```

Then run `make` in the root directory:
```bash
make
./physics-simulation
```

# Using the Program

The first time you run the program a "Config.ini" will be created it should look something like below:

```
Config File for the Cannon.exe
1.5.0
Screen Width: 640
Screen Height: 480
Log Ball's path: 0
Enable Drag Mode (experimental): 0
Draw Ball path on screen: 0
Collision Method: 1
Number of points to track: 5
Delay between recording points: 25
```

You can use this to change things like the screen height and width, and other stuff.

"Log Ball's Path" (if enabled with a 1 instead of 0), will cause a a file called "logfile.log", which will contain the placement of every ball on every update (it gets quite big as you can guess).

"Enable drag Mode" (if enabled with a 1 instead of 0), will cause the cannonballs to be effected by drag. It is listed as experimental because I based the equations used off of several things I found 
online and I cannot verify how accurate it is.

"Draw Ball path on Screen" (if enabled with a 1 instead of 0), will cause small pixels to trail after each ball.

"Collision Method" you can change how the balls will interact with each other when they collide based on the number in the table below.

| Name                    | Number | Explanation                                                                                                |
|-------------------------|--------|------------------------------------------------------------------------------------------------------------|
| CollideElastic          | 0      | This is normal collision in a perfect world without  friction, they hit and bounce off, no energy is lost. |
| CollideInelastic        | 1      | This is the real life situation. Balls collide and,  bounce off of each other losing some energy.          |
| CollidePerfectInelastic | 2      | In this method the balls collide and then stick together.                                                  |
| CollideNone             | 3      | The balls do not collide and pass through each other.                                                      |


"Number of points to track" is the number of past points that will be kept track of if "Draw Ball Path on screen" is enabled.

"Delay between recording points" is the delay (in ticks) between everytime it records a point, if "Draw Ball Path on screen" is enabled.
A smaller number means that the points are recorded much faster.

Once you start the program you will be greeted with a beautiful black screen. To create a new cannonball you click (and hold down the button) 
you can now move your mouse around to change the angle that the new cannonball will be fired at; the longer the line, the faster it will go. 
When you are happy with the angle and speed, simply release the mouse button and the ball will be created at the point where you first clicked.

The longer you hold down the mouse button the larger the radius of the ball will be and as a result, the more mass the ball will have. You can get an 
idea of how much mass a ball has based on how transparent it is; the more transparent it is, the lighter its mass.

## Keybinds

You can also press the following keys to yield effects:

```
K = All Balls will be "killed".
R = All motion will be stopped.
Q / ESC = Program will exit.
F = Toggle random fire (periodically fires random cannonballs)
T = Toggle toolbox on/off
P = Toggle pause for the simulation
Right Arrow = Select next tool
Left Arrow = Select previous tool
1 = Select Fire tool
2 = Select Drop tool
3 = Select Rope tool
4 = Select Delete tool
5 = Select Drag tool
6 = Select Info tool
```

## Tools

### Fire tool
Clicking and dragging on the screen will draw a white line from the original click location. When the mouse is released, a ball will be fired in that direction with velocity proportional to the line length. Mass is determined by how long the button was held.

### Drop tool
Clicking on the screen will create a ball at that location with no initial velocity. Mass is determined by hold duration.

### Rope tool
Clicking on a ball attaches one end of a rope. The other end can be attached to another ball or pinned to the background (by clicking where there is no ball).
The rope uses a hybrid force-position model to ensure it is inextensible, pulling connected objects along rather than stretching.

### Delete tool
Clicking on a ball will delete it. Any ropes attached to that ball will also be automatically removed.

### Drag tool
Clicking and dragging on a ball allows you to move it. While dragging, the ball is "paused" in the physics simulation. When released, it will retain its initial velocity.

### Info Tool
Clicking on a ball will print detailed information about its physical properties and state to the console.
