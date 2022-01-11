# Alien Attack IV

## Po-Shen Loh

This repository is a window into 1998, when 320x200 resolution with 256
colors was standard. It contains the code for a video game that was written
by Po-Shen Loh when he was in 10th grade, now playable through full DOS
emulation in modern web browsers at:

https://www.math.cmu.edu/~ploh/em-dosbox/aa4.html

Back then, it was nontrivial to smoothly process keyboard and mouse input
and draw graphics. The sound code came from [Ethan
Brodsky](https://web.archive.org/web/20210211181817/http://homepages.cae.wisc.edu/~brodskye/smix/smix.html), who
coincidentally was on the same Wisconsin statewide math team.

This game was submitted as a final project in a high school computer
programming class at James Madison Memorial High School (now named [Vel
Phillips Memorial High
School](https://en.wikipedia.org/wiki/James_Madison_Memorial_High_School)
in Madison, Wisconsin. For laughs, the project journal (`Aa4/Journal.doc`
in this repository) as written by the then-teenage author is copied below.


## PROJECT JOURNAL

Po-Shen Loh

Period 5

Mr. Williams

Project Title: Alien Attack IV

Date begun: March 23, 1998

Description: Vertically scrolling 2-D scroller.


3/23/98:
Idea refined.  I will be making a two-dimensional vertically scrolling shoot-em game.  There will be level files, thus allowing tremendous flexibility (and making it possible for me to tinker with game elements easily).  This will also make updates simple.  Data file format devised.  File formats typed out into computer.  Ready to begin game planning.  Next step: rough game plan.

3/24/98:
Program planned out.  Work begun on title screen.  So far, all I've gotten are the buttons, a mouse, and part of the left panel.  I've resurrected (hopefully) a graphic I drew last September.  The graphic will be the ship.  The left panel of the menu screen will be the ship flying.  I've upgraded my button unit from version 1.10 to version 2.00 with the new addition of VGA 256 color graphics.  Also, my own mouse has been implemented.  The graphics unit has been upgraded because of the addition of automatic text justification.

3/25/98:
Image of ship resurrected.  GrafMk2 also updated to allow AND image maps.  Enlarge.cpp written to enlarge an image created by GrafMk2.  Original image of ship enlarged to allow for AND image maps.  Installed all used libraries at school and verified them.  They all work just fine.

3/26/98:
At school, loaded up ship image and wrote a program to test it.  It was displayed on screen.  Also, the button library was tested; there was an OK button next to the ship image.  Test program for moving ship written.  It worked, after quite a bit of work.  I fixed up my graphics library again.  Now the predefined enum types for BitBlt types are in my graphics header file too.  Now I will begin coding the introduction screen.

3/27/98:
Fixed up menu screen.  Played with the concept of "virtual screens".  Showed Shawn and Marc a site for graphics.  It was in Pascal, but they knew Pascal so it was fine.  Now my menu screen is in 256 color mode and contains two panels.  The right one is the menu.  After much wasted effort, I decided to confine the mouse cursor to this region.  If it had been included in the other region, then there would be a big headache caused by screen refreshes.  Used virtual screens to soup up a flickering animation.  [that was on the left]  The left panel had a ship moving left and right with a star field passing it (moving from top to bottom).  Using the concept of files, the ship image was loaded from a file, to simplify modifications.  Lots of time was spent debugging because of a silly little mistake concerning virtual screens.  I had accidentally written to the real screen in one of my commands.  After fixing that and another silly mistake regarding setfillcolor, the title screen worked.  Now I can proceed to write the game.

3/28/98:
As I began to think about writing the game (i.e. the 'new game' button), I noticed that I would need some sort of graphical user input.  The user needs to be able to type.  So, I spent this day writing an input algorithm from scratch.  I also added some more characters to the font's letter-writing capacity.

3/29/98:
Wrote the stub code for the buttons in the menu.  Now they all lead to some sort of notification screen.  Pilot structure devised to hold user data.  The 'new game' code now calls the 'game' code... the 'game' code plays the game.  So, now I will write the 'game' code and link it in.  Then I will have a functioning 'new game' node.

3/30/98:
Began writing code for body of game.  I wrote the header files for 3 classes today: the level class, the eprojs, and the goodies.  I have quite a few more classes to write before I can begin writing their function bodies.  But as of now, I have thought out what I need to write.

3/31/98:
Finished writing all class declarations.  Now am ready to tackle the job of writing function bodies.  Attached XMS unit to program.  This will allow extended memory usage.  Also added keyboard handler.

4/1/98:
Began to write class function bodies.  Wrote all bodies for non-friend functions for Level class.  Now am writing the first friend-function body: the load-level function.

4/2/98:
Completed friend function bodies.

4/6/98:
Wrote all stubs for all functions.  Tried to run program... there were many run-time errors.  Time to debug.  I had set up the level scrolling device (just to test it).  It was supposed to work.  Instead, I now have an error when the program returns to the main menu.  I think it is a pointer bug.

4/7/98:
Made program run without virtual screens.  Fixed a few errors.  Made the data-loading come fully from files.  However, couldn't figure out why virtual screens didn't work.

4/8/98:
Attempted to make program run under BC++ 3.0 (so that I could use the debugger).  It didn't work.

4/9/98:
Finally succeeded in making program compile under BC++ 3.0... but it crashed right after the title screen in the debugging environment.  Attempted to compile again under BC++ 5.02 and succeeded, but still found problems with the virtual screens.

4/10/98:
Tried to find bug.  Traced it into the XMS function.  Tried to fix it.. failed.

4/13/98:
BUG FIXED!  Program now runs smoothly, utilizing virtual screens for flicker-free animation.  User can move ship around.  This means that I have already reached my goal for the 20th.  I am one week ahead of schedule.  The problem was apparently a function which was too large... there wasn't enough space in the call stack for it to fit.  I made the variables global, and then it worked.  --  Program now runs smoothly, and background changing works.

4/14/98:
Program documented, screen transitions smoothened, sound added.

4/16/98:
Program prepared for next stage of construction: the completion of the game data file.  Also, many other people contacted for testing.

4/17/98:
Began adding the implementation of the game data file.

4/18/98:
Completed projectile implementation.

4/19/98:
Drew graphics.

4/20/98:
Fixed choppy-error.  No longer is choppy.  Instead, it is way too fast now.

4/21/98:
Updated the credits screen.  Also, I drew lots of graphics.  Smooth scrolling and mission briefings were added.  Shield and power bars added.  Projectile firing now takes power.  Weapon systems integrated into program.  I am ready to begin adding the bad guys.

4/22/98:
Began adding the bad guys.  Drew images and attempted to load them into the program.  It didn't work.  I wrote it in such that they would just load, but do nothing (i.e. not appear on the screen).  But that crashed right after the mission briefing.

4/23/98:
Debugged the enemy section.  Now, they are loaded into memory with no trouble.  I am ready to display them on screen and animate them.  In fact, that is exactly what I am doing right now.

4/24/98:
Fixed enemies up.  Enemy animation complete.  Enemies also shoot now.  Projectiles fully implemented.  I am about to add the collision checking.  (When you crash into an enemy or bullets collide with objects, they are supposed to be hit.)

4/25/98:
Enemies, shots, and you now collide.  Explosions ensue.  Your sheilds and enemy shields are diminished.  Ready to implement goodies.  Goodies implemented.  (it's now 10 pm)  You can even pick them up.

4/26/98:
Minor bugs fixed.  Program streamlined.  Cash implemented; now there is a cash bar at the top of the screen.  It is now ready to enter its final stage of development: the shop.  Began design of shop engine.  It will interpret .PO files.  (My own file format --- invented it.)

4/27/98:
Began drawing up the .PO file for the main concourse.  It will look "3d" and be a curving hallway.  Got the basic outline completed.

4/28/98:
Completed most of the .PO file for the main concourse.  Was very busy that day because of the math test.

4/29/98:
Completed all of the .PO file for the main concourse.  Incorporated it into the game.  It will now appear on screen between missions.  Also, I began the .PO file for the store.  It will look like a computer.  So far, all but the buttons are drawn.

4/30/98:
Integrated shop scene into main program.

5/1/98:
Made the mouse actually do something in the shop scene.  Now, the temporary flag for quitting has been updated.  Before, there was an auto-quit at the last level... now you can quit at any time by selecting the right door at the hallway scene.  Also, the click-region for flying a mission has been added.  The stubs for the Upgrade and Save regions are in.  I am ready to put the code in.

5/2/98:
Wrote the Save/Load pair of trees.  Now, you can save and load all you wish.  Of course, there really isn't much point to saving/loading, as there are only two measly levels.  The next task is to write the code for the Upgrade door.

5/3/98:
Wrote code for the Upgrade door... almost done, except that for some reason there cannot be more than one type of weapon.  Hm... needs some figuring out.  Also, program crashes often and is unstable.. and Upgrade screen flickers.

5/4/98:
Fixed flickering problem.  Fixed problem about having more than one type of weapon (the real problem was that for some reason the file size used for the other weapon image was incompatible with my concatenation program [!]).  Now there are two weapons, one primary/one secondary.  ALT switches the secondary weapons... buy/sell works great!  Ready for next stage: instructions/information.

5/5/98:
Instructions/information set up to read from files.  Began to write files.  Also, attempted to smoothen animation and eradicate bugs.  Fixed bug which let you sell items you did not own... and get full price for them.  

5/6/98:
Level design!  Created two new types of enemies, and they are now incorporated into the program.  One is a 2nd class starship, and the other one is the mother ship.  Also, a bug which only would be triggered at the end of the program was fixed.  Another bug with enemy hitpoints was fixed.  Yet another bug regarding false messages was found and fixed.  Story and instruction files completed.  It is now time to document/write up web page/design more levels.

5/7/98:
More level design!  Created two new levels.  [with briefings]  Enemy projectiles now can track on you.  The game has become many times more challenging.  Now there is a game of dodgeball within the program.  [well, sort of.  eventually you have to play dodge-the-enemy-bullets]  Aliens are more formidable.  Continued to document code.

5/8/98:
Wrote yet another level.  Now there are four levels.  Also recorded two new musical tracks off my keyboard.  Added three new weapon systems (all guided) and three new weapon types.  Updated money system (it was too easy before).  Made levels more challenging.  Made your shots able to track enemies.

5/9/98:
Created a new set of tiles for the "green" planets.  They are much nicer than the brown tiles.  New bad guy added: ground turret.  Bug with enemies escaping to the right and never being deleted... fixed!  Musical tracks vary with the level.  Shop computer fixed.

5/10/98:
Created levels and modified weapons.

5/11/98:
More level design and weapon modifications... also, documented.

5/12/98:
Created new weapons and documented.

5/13/98:
FINISHED DOCUMENTING!!!!!!!  :-)
Designed many new aliens and a few new projectiles.  Also modified weapons.

5/14/98:
Modified more weapons, made more levels, and made more types of enemies.

5/15/98:
Created random-level-generator!  Now I can make levels really quickly.  Skeletons for levels 11-20 were made.

5/16/98:
Filled in bodies for levels 11-20.  Also updated webpage.  Project is now completely finished.  For the rest of the time, I get to spiff up my random-level generator and crank out many many new levels!

5/17/98:
Improved smoothness by coordinating iterations to the system clock instead of the monitor refresh rate.

5/18/98:
Discovered an insidious bug in my XMS library.  I had figured out about a month ago that it was not possible to copy blocks of memory to and from XMS unless their lengths were even.  At that time, I thought that it would not be a matter of concern.  It turned out that my program was crashing partly because odd-lengths were being copied.  That error was fixed.

5/19/98:
Discovered yet another bug: yesterday I fixed the one about copying TO extended memory.  Today I fixed the one for copying FROM extended memory.  This took a long time, as many sections of AA4 had to be rewritten.  Hopefully, this will take off a few more bugs from the buglist.

5/20/98:
Fixed more insidious XMS bugs.  Pause function implementation begun.

5/21/98:
Fully implemented pause function.  Debugged.

5/22/98:
Debugged.  Updated version number.  Went into old libraries in search of the bug.. to no avail.  This random-crash is very annoying.

5/23/98:
Out all day.  Could not work on program.

5/24/98:
Tried to fix bug.  Fixed it up a bit.

5/25/98:
Tried again, and fixed it up a bit.

5/26/98:
Kept on fixing minor bugs.

5/27/98:
Implemented no-sound command-line argument.  Fixed a few more minor bugs.  Still cannot find major bug.
