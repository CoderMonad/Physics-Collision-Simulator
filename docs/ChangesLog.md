# Change Log

All notable changes to this project will be documented here.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.5.0-alpha.4] - 2026-05-27
### Fixed
* `clsTick`: replaced `clock()` (CPU time) with `std::chrono::steady_clock` so delta-time reflects real wall time, fixing physics running at wrong speed when the OS sleeps the process
* `clsCannonball::drawPath`: per-instance `pathUpdateCounter_` replaces a `static` local variable that was shared across all ball instances, causing balls to interfere with each other's path recording cadence
* `clsCannonball::drawPath`: `push_back + erase(begin) + shrink_to_fit` on every recorded frame replaced with a circular buffer (`pathHead_` index); eliminates two potential reallocations and an O(n) element shift per recorded point
* `clsCannonball::dragUpdateAcc`: drag force was skipped when a ball moved purely horizontally or vertically (`vel_.x != 0 && vel_.y != 0` guard); replaced with `getVectorLength(vel_) > 0`
* `clsCannonball` constructor: `srand` was called with uninitialized `ballID_`; both `srand` and `color_` initialisation moved into `setValues()` after `ballID_` is assigned
* `cannonballs::doCollide`: `ContactAngle` was computed as the difference of velocity angles, not the true centre-to-centre collision normal; replaced with `atan2` of the ball-centre delta vector (y-flipped to physics-space)
* `cannonballs::clean_up`: erase-while-iterating loop skipped the element that shifted into the erased index; replaced with `std::remove_if` erase-remove idiom for both balls and ropes vectors
* `clsConfig::exists`: `fopen` handle was never closed; added `fclose` before returning
* `clsConfig::load`: two copy-paste bugs where parse-failure fallbacks assigned to `uintScreenHeight` instead of `uintMaxNumPastPoints` and `uintPastDelay` respectively
* `main`: `event_return` was uninitialised; a missing `SDL_PollEvent` result on the first frame could trigger a spurious `quit` or `fire` via the switch
* `main`: loop indices changed from `int` to `size_t` to eliminate signed/unsigned comparison against `balls.size()` / `ropes.size()`
* `clsScreen::drawline`: pixel-by-pixel `SDL_RenderCopy` loop replaced with a single `SDL_RenderDrawLine` call; eliminates O(n) per-pixel texture blits for every line segment
* `clsRope::ballWallForces`: `3/2 * M_PI` was integer division (`= M_PI`), producing a wrong threshold angle; fixed to `3.0/2.0 * M_PI`; floating-point `==` comparisons replaced with epsilon checks
* `math::vectorDiv`: added `assert` guards against division by zero in both the scalar and vector overloads
* `toolbar::show`: `pause_location.x + 28` was a no-op expression (result discarded); corrected to `pause_location.x += 28`

### Added
* Ball-to-ball rope tension implemented in `clsRope::update` using the constraint-dynamics formula `T = (m1·F2·n̂ − m2·F1·n̂) / (m1 + m2)` derived from `(a1 − a2)·n̂ = 0`; tension is clamped to ≥ 0 (rope can only pull)


### Added
* `kRadiusMin` (0.01 m) and `kRadiusMax` (0.20 m) constants in `global::equations` namespace; ball radius is now clamped to this range on creation, preventing near-zero mass that caused division-by-zero in acceleration and `log(0)` in alpha calculations

### Removed
* Dead `DEFINED_PUSH_BALLS_OUT_OF_OVERLAP` code path and its feature flag — superseded by the circle-based post-collision separation added in 1.5.0-alpha.2

## [1.5.0-alpha.2] - 2026-05-27
### Changed
* `cannonballs::checkOverlap` now uses a two-phase collision detection approach: a fast AABB broadphase rejects clearly non-overlapping pairs, followed by a pixel-accurate circle distance narrowphase (`dx² + dy² ≤ (r_A + r_B)²`) that correctly handles round balls instead of squares; also fixes a pre-existing bug where a ball fully contained inside another was never detected
* Post-collision ball separation now resolves along the true centre-to-centre axis (instead of the axis of least AABB penetration), giving correct push direction for diagonal collisions; degenerate case of coincident centres falls back to a horizontal push

## [1.5.0-alpha.1] - 2026-05-27
### Fixed
* Balls sticking together after collision: added a post-collision minimum translation vector (MTV) separation step so overlapping balls are physically pushed apart each frame, preventing repeated collision resolution from draining their energy
* Copy-paste bug in `CollideInelastic` mode where ball B's post-collision velocity was incorrectly scaled using ball A's momentum instead of its own
* Rope rendering as a straight line: rope now draws as a gravity-sagging catenary curve using a quadratic Bezier parabolic approximation; sag direction is computed as the gravity component perpendicular to the rope chord, so ropes hang correctly at any angle

### Changed
* Rope `length_` changed from `uint` to `double` to eliminate truncation artifacts when the rope is created

### Added
* Re-enabled rope tool: key `3` now activates the rope tool; left-arrow navigation now cycles through it

## [1.4.2-beta.3] - UNRELEASED
### Changed
* Moved src files around to group them into similar folders
* kTimeSizeRation from 0.025 -> 0.25
* Equation for the relationship between radius and time

## [1.4.1-R] - 2020-05-29
### Added
* Math function specifically to deal with vector math.
* Pause symbol for when simulation is paused (Issue #10)

### Fixed
* Spelling mistakes
* Various Code mistakes
* Drag tool not un-pausing correctly
* Defined macros not working correctly since I moved certain functions to core.cpp

### Changed
* Updated Doxygen file
* Info tool now displays if ball is paused
* Modified the check overlap function

## [1.4.0-R] - 2018-03-27
### Added
* Toolbar with different tools including: drag, fire, drop, rope, delete, and info. (Issue #5)
* Currently finished tools: fire, drag, drop, delete, and info. (Rope tool is present, but rope equations need more work, so I disabled it for now).
* SDL Licenses.
* Credits to Docs.
* More Doxygen stuff.
* Debug message that prints ball mass when created. 
* Ability to pause the simulation.
* Changeable Ratio of meters to Pixels (default is 10 pixels/meter)

### Changed
* Moved Docs to Doc folder.
* Moved various items out of main.cpp into core.cpp to avoid main becoming too large.
* How balls are updated (order and a few equations) to make it hopefully more realistic. 
* Increased ball size from 10x10 pixels to 25x25 pixels.
* Various code improvements.
* Disabled rope tool, still need to work on some of the equations. (Issue #4)
* Reduced kVelocityScalar from 1 -> 0.1
* Floors to casts (due to large number of calls of the floor functions).

### Removed 
* Inertia/energy from spins, because it did nothing.

### Fixed
* Bug of balls getting stuck in walls.
* Bug of balls velocity becoming NaN. (Issue #3)

## [1.3.1-R] - 2018-03-16
### Added
* Code of Conduct.
* Doc Folder.

### Changed
* Moved docs files into doc folder.


## [1.3.0-R] - 2018-03-06
### Added
* Inertia/energy from spins.

### Notes
* Pretty pointless addition, just wanted to add something since its been a year.

## [1.2.1-R] - 2017-03-06
### Updated
* Added offset to random fire so it should be possible to have a 0 mass any more.
* Changed the scaling factors for alpha and time to size. Done so the difference between 0.25 s and 10 s isn't as massive (was > 500, now 16 ish).


## [1.2.0-R] - 2017-02-27
### Added
* Buoyancy force.
* Magnus effect.

### Updated
* Drag equation.
* System is force based instead of acceleration.


## [1.1.2-R] - 2017-02-24
### Added
* "f" key now causes random balls to be fired off in different directions.

### Updated
* Various Code improvements.
* Cannonball location is now based on its center point.
* Changed some of the scaling factors.
* Changed how collisions with the edges are handled.

### Fixed
* Elastic collision equation being wrong (had cos when it should have been sin)


## [1.1.1-R] - 2017-02-22
### Updated
* Vectors now use shrink to fit command to clear memory.


## [1.1.0-R] - 2017-02-20
### Added
* Cannonballs are now vector based instead of array based.
* Path is now vector based.
* Random colors are now seeded based on time. Because reasons...

### Updated
* Many variable names to be more in line with [Google's style guide](https://google.github.io/styleguide/cppguide.html) .
* Various other changes to make it more in line with the style guide.
* Various other improvements to code function and variable types.
* Number of cannonballs is now limited by computer memory.
* Window values are now held more globally so I don't have to keep passing it everything that needs it.
  This likely isn't the best method to achieve it, but it prevents an issue with switching to vector based cannonballs.
* Max number of past points is now a config value.

### Notes
* There are still quite a few non-standard elements left from before I started using Google's style guide consistently. 
  I changed the worst offending members, but most of the rest of it will be left. 

## [1.0.0-RC] - 2015-11-21
### Added
* Movement Equation Image.
* Collision method set in config.

### Notes
* Moved to RC as I've done almost everything I can think of to do. All that really needs to be figured out is why the balls sometimes stick together.
	Which I believe is because of the Tick system and cannot be easily fixed.

## [1.2.7-Beta] - 2015-11-18
### Fixed
* Copy Paste Error that caused the pixel texture to remain in memory.
* Spelling and grammar mistakes.

### Added
* More comments.


## [1.2.6-Beta] - 2015-09-24
### Updated
* Cleanup.bat to include doxygen files (also added wildcard support for some files).
* Readme for the slight change to the config file.

### Changed
* Makefile (it should now work).
* Doxygen now uses DOT to make graphs.
* Functions that were in main.cpp have been renamed and moved into a namespace.
* All line separators from 170 charaters long to 80 to allow me to see if I am breaking Google's recommended 80 charaters length 
	see [here] (https://google-styleguide.googlecode.com/svn/trunk/cppguide.html#Line_Length) .
* Made most lines to be less than 80 charaters.

### Cleaned
* Dependencies on header files.
* Dependencies on external libraries.

### Added
* More doxygen comments that I missed before.


## [1.2.5-Beta] - 2015-09-23
### Added
* Doxygen Support.
* Doxygen Documentation.
* Makefile.

### Changed
* Change log format to be markdown.
* License format to be markdown.

### Removed
* Autoversioning support.
* clsCannonball::enableDrag (pointless now, it is just enabled in another function).


## [1.2.4-Beta] - 2015-09-21
### Added
* Cannonballs are now different colors.

### Changed
* Ball ID is now set in Screen set.
* Unrealistic method added in 1.1.9-Beta can be toggled easier (with define). Its funny to enable and watch even if it doesn't do what it is supposed to. 


## [1.2.3-Beta] - 2015-09-21
### Added
* Cannonballs are now more transparent the less mass they have.
* Namespace equations to hold equation values.
* Ball path is now drawn on screen (holds so many past points, can be enabled in config file).

### Changed
* Reduced fTimetoSizeRatio from 1.2458 to 0.0124.
* Screen background color from White to Black.
* Ball color from Black to White.
* Pixel color from Black to White.

### Removed
* Sky image (replaced with SDL_SetRenderDrawColor).
* My attempt at making parts of the ball image transparent (it wasn't working).


## [1.2.2-Beta] - 2015-09-18
### Fixed
* Mouse line getting thinner the closer to vertical it gets.

### Cleaned
* Up a lot of code; made things simpler.

### Changed
* All angles are now in radians so I don't have to worry about converting.
* Ball radius (and therefore mass) is now proportional to the amount of time you held down the mouse button for.
* Upped default allowable number of cannonballs from 10 to 20.


## [1.2.1-Beta] - 2015-09-17
### Fixed
* An issue with mass not being set causing values to being NaN, if drag was not enabled.


## [1.2.0-Beta] - 2015-09-17
### Fixed
* The Collision equations a bit more.
* Drag update acc would sometimes divide by 0.

### Added
* Cannonball now holds its own box.
* Keyboard Press events, (Q/ESC to quit, K to kill all balls, and R to stop all motion).
* Friction between the ball and floor / wall.

### Updated
* Readme.

### Changed
* Attempted to make the code more efficient. 
* Default Min Velocity from 0.5 to 0.0 (basically disabling it).
* Disabled the Unrealistic method I added in 1.1.9-beta as it did not help.
* Default Collision Method from Elastic to Inelastic.
* fMomentumLoss name to fCoefficientRestitution to be named after its real life counter part.


## [1.1.9-Beta] - 2015-09-16
### Fixed
* Log file is now cleared at start.

### Added
* Cannonballs now keep track of ID for logging purposes.
* Clearer Debug Message if a ball was killed because of speed or NaN.
* Unrealistic method where velocity is modified with the inverse root of the distance between the points (this was an attempt to prevent the balls from sticking together as they have been).


## [1.1.8-Beta] - 2015-09-16
### Fixed
* You can now properly fire a cannonball in all directions.
* Balls that have velocity as NaN will be removed (I should have fixed the equations so this doesn't happen, but just is case).

### Added
* More collision methods (Inelastic, Elastic, Perfect Inelastic, and No Collisions).

### Improved
* Collision equations.


## [1.1.7-Beta] - 2015-09-15
### Fixed
* Bug with mouse keeping old coordinates.
* X Fire velocity not being right.

### Added
* Collisions.


## [1.1.6-Beta] - 2015-09-15
### Fixed
* Y velocity being flipped.
* Bug with balls not correctly bouncing at certain velocities.

### Added
* Support for adding multiple cannonballs (no collision between them yet).

### Removed
* Ball position updating on debug mode (too many balls).

### Updated
* Ball will now be removed if their velocity drops below a certain value.

### Note
* Currently the array holding multiple cannonball is NOT dynamic I still have to figure that out.


## [1.1.5-Beta] - 2015-09-15
### Added
* Global.h to hold the externs of the globals.
* Balls with bounce off of screen edges.

### Removed
* Cannon image.

### Cleaned
* General code.

### Changed
* You add a ball by clicking and dragging then releasing the button. (Only one ball supported at the moment).
* How cannonballs are applied to screen.
* Method to add cannonballs.
* Drag mode now enabled in config file.


## [1.1.4-Beta] - 2015-09-15
### Fixed
* Some build issues between Windows and Unix.

### Added
* Proper quitting by click X on SDL window.


## [1.1.3-Beta] - 2015-09-01
### Added
* Cleanup.bat for deleting useless files.
* Cannon Image.

### Changed
* Log file to print out floating values.


## [1.1.2-Beta] - 2015-08-28
### Added
* Config value that holds OS (kinda pointless at the moment; mostly just so I have a reference of how to do this in the future).

### Changed
* Sky picture to be smaller (64x48 instead of 640x480) since I'm embedding so that file size will be smaller.
* Embedded Images as XPMs

### Removed
*  All but 1 ball size (since I am now embedding the images it didn't make sense; nor was it really all that useful).


## [1.1.1-Beta] - 2015-08-27
### Fixed
* Various issues on Unix systems.

### Added
* SDL Images.
* General Linux support.
* OS support for default path to images.
* Linux Build options.

### Changed
* Build options for Windows Versions.
* Pictures from BMP to PNG.


## [1.1.0-beta] - 2015-08-27
### Fixed
* A few bugs.
* Some Debug message appearing when not in debug mode.

### Changed
* Deltat is now variable on the time since the last update.


## [1.0.2-Beta] - 2015-08-27
### Fixed
* Several Bugs.

### Removed
* Version info including software status (not working).
* Loading of Images path (was causing issues).


## [1.0.1-Beta] - 2015-08-27
### Added
* Config file.
* Software state being added to boilerplate.
* Logging of ball location to a file.
* Easier way to change images path (config file).


## [1.0.0-Beta] - 2015-08-27
### Added
* Various ball sizes that are pulled when radius is entered.

### Removed
* Old sky bitmap.

### Notes
* Moved to beta.


## [1.1.6-Alpha] - 2015-08-26
### Fixed
* Autoversioning stuff (hopefully).


## [1.1.5-Alpha] - 2015-08-26
### Changed
* Reduced FPS from 120 to 80.


## [1.1.4-Alpha] - 2015-08-26
### Fixed 
* Bug where loop wouldn't end properly because x/y were uint.

### Added
* Debug mode.
* Debug messages.

### Changed
* Sky.bmp to white picture.
* Upped default FPS from 60 to 120.

### Removed
* Message about the ball position.


## [1.1.3-Alpha] - 2015-08-25
### Added
* Drag mode can now be enabled by user.

### Removed
* Wait time message.


## [1.1.2-Alpha] - 2015-08-25
### Fixed
* Several bugs.


## [1.1.1-Alpha] - 2015-08-25
### Added
* Location Structure.
* Other structures to make things nicer.
* User now prompted for values.
* Cannonball class.

### Cleaned
* main.cpp (only has main now).

### Changed
* Icon.rc merged to boilerplate.rc.

### Removed
* Compile.bat (couldn't get it working).


## [1.0.3-Alpha] - 2015-08-25
### Fixed
* Incorrect path format.
* Sky now loaded before ball.
* Bug with the location of the ball while updating.
* Ball position bug.
* Physics equation mistake.

### Added
* Tick Class.
* Console messages about where the program is at.
* Getchar just before program closes.

### Changed
* Clean up is now done before error message.
* Drag is now off by default.
* Ball.bmp picture.


## [1.0.2-Alpha] - 2015-08-25
### Added
* Define of image path so people can change it easier.


## [1.0.1-Alpha] - 2015-08-25
### Fixed
* SDL Referencing.

### Added
* Version.h.
* Code Block Support.
* Boilerplate.rc
* Skybox.
* Windows Class.

### Changed
* SDL Stuff to SDL2.


## [1.0.0-Alpha] - 2015-08-11
### Notes
* Initial.
