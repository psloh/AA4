#include <dos.h>          // delay()
#include <fstream.h>      // file for title.gph
#include <alloc.h>        // memory allocation

#include "PGphV120.H"                      // my graphics
#include "PMseV101.H"                      // my mouse
#include "PButV200.H"                      // my buttons

#include "Aa4Menus.H"                      // header for module
#include "Aa4Game.H"                       // header for game
#include "Aa4Scrns.H"                      // header for screens.


extern GraphObject G;  // external graphics object
ButtonObject B;        // global button object
extern VirtualScreen Vscr_G;  // external virtual screen

void Menu(void)
// Pre: None.
// Post: All game played.
{
	MouseObject M;                       // objects declared
   char *choice;                        // choice of user
	int xdim, ydim, isize;               // data for ship image in title
	void far *xor;                       // images for title screen ship
	void far *and;
	char first_time=1;                   // flag
	char ch;                             // user choice

	G.makevirtual(&Vscr_G);              // get mem for virtual screen

	ifstream inf("title.gph");                // load pic for title screen
	inf.read((char*)&xdim, sizeof(int));
	inf.read((char*)&ydim, sizeof(int));
	inf.read((char*)&isize, sizeof(int));
	xor = farmalloc(isize);
	and = farmalloc(isize);
	inf.read((char*)xor, isize);
	inf.read((char*)and, isize);
	inf.close();

	G.initgraph();                // init graphics
	G.fade_to(0);                 // fade to intensity 0%

	M.ColdReset();                // reset mouse

	do
	{
		B.AddButton(200, 35, 280, 35+15, GREEN, "New Game", "New Game");    // add all buttons
		B.AddButton(200, 35+23, 280, 35+15+23, GREEN, "Load Game", "Load Game");
		B.AddButton(200, 36+23*2, 280, 35+15+23*2, GREEN, "Instructions", "Instructions");
		B.AddButton(200, 36+23*3, 280, 35+15+23*3, GREEN, "Story", "Story");
		B.AddButton(200, 36+23*4, 280, 35+15+23*4, GREEN, "Credits", "Credits");
		B.AddButton(225, 36+23*5, 255, 35+15+23*5, GREEN, "Quit", "Quit");

		G.setcolor(2);                                 // set colors & draw
		G.rectangle(170, 10, 310, 190);

		G.setcolor(28);
		G.rectangle(10, 10, 150, 190);

		if (first_time)                          // at first time, wait 3 sec for screen to adjust
		{
			delay(3000);
			first_time = 0;
		}

		G.fade_in();      // fade in

		G.write(240-58, 20-2, 55, LIGHTCYAN, "A L I E N   A T T A C K   I V", 30);     // title text animation
		G.write(240-54, 180-2, 55, LIGHTCYAN, "B Y   P O - S H E N   L O H", 30);
      G.write(5, 195, RED, LIGHTRED, "Copyright (C) 1998 Po-Shen Loh.  All rights reserved.", 30);
      G.write(291, 195, RED, LIGHTRED, "V 4.11", 30);

		B.ShowAllButtons();    // display all buttons

		M.MoveCursor(100, 480);     // set mouse cursor stuff
		M.VertLimit(10, 190);
		M.HorizLimit(340, 620);

		choice = B.TrapClick_Anim(LEFT_BUTTON, xor, and, xdim, ydim, Vscr_G);   // get a button click
		ch = choice[0];
		G.fade_out();       // fade out
		G.clearviewport();  // clear screen

		M.VertLimit(0, 199);
		M.HorizLimit(0, 638);
		B.RemoveButton("New Game");      // free all mem for buttons
		B.RemoveButton("Load Game");
		B.RemoveButton("Instructions");
		B.RemoveButton("Story");
		B.RemoveButton("Credits");
		B.RemoveButton("Quit");

		switch (ch)   // branch on choice
		{
			case 'N' : NewGame();                  // all self-explanatory
						  break;
			case 'L' : LoadGame();
						  break;
			case 'I' : Instructions();
						  break;
			case 'S' : Story();
						  break;
			case 'C' : Credits();
						  break;
			case 'Q' : ;
			default  : ;
		}
		G.clearviewport();            // clear screen again
	} while (ch != 'Q');

	farfree(xor);                          // free all mem
	farfree(and);
	G.clearvirtual(&Vscr_G);            // free mem for vscr

	G.closegraph();      // quit graphics mode
}
