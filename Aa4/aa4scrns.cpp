#include <conio.h>      // getch()
#include <fstream.h>    // instructions & story files
#include <stdio.h>      // sprintf()

#include "PGphV120.H"     // my graphics

#include "Aa4Scrns.H"     // header for this module
#include "Aa4Kybd.H"      // header for keyboard funcs


const int credits_rows = 27;  // rows in credits
char *text[credits_rows] =
{
	"Alien Attack IV Credits",
   "------------------------------------",
   "",
   "",
   "Engine Programmer:",
   "  Po-Shen Loh",
	"",
   "Sound Library:",
   "  SMIX, by Ethan Brodsky",
   "",
	"Game Testers:",
   "  Chi-Bong Chan",
   "  Eric Chow",
   "  Andrew Holbrook",
   "  Jacob Kitzman",
   "  Howard Liu",
   "  Andrew Schepler",
   "  Sasha Schwartz",
	"  Dan Sklansky",
	"  Josh Sinykin",
   "",
   "Graphics Design:",
   "  Po-Shen Loh",
   "",
   "Level Design:",
   "  Po-Shen Loh",
   ""
};


extern GraphObject G;  // external graphics object
extern char EscFlag;   // escape flag


void Instructions(void)
// Pre: in graphics mode.
// Post: instr displayed
{
	while (kbhit())   getch();             // clear keyboard buffer
	ifstream story("AA4instr.TXT");        // instruction file
	int row = 3, stop = 0;                 // counters
   char line[81];                         // temp string for line

   G.clearviewport();       // clear screen

   G.setcolor(10);                                // write the heading
   G.settextjustify(CENTER_TEXT, CENTER_TEXT);
   G.outtextxy(159, 5, "Instructions");

   G.settextjustify(LEFT_TEXT, CENTER_TEXT);
	while (!stop)                            // output story, line-by-line.
	{
		if (story) story.getline(line, 80);    // load in a line of the story
		else { stop = 1; continue; }

		G.setcolor(row+64);            // draw the line on screen.
		G.outtextxy(3, row*10, line);
		row++;

		if (row > 17)     // if one screenful of lines is on, then...
		{
			G.setcolor(12);                              // prompt for a key and fade in screen
			G.outtextxy(30, 195, "Press a key to continue...");
			G.fade_in();

			if (getch() == 0) getch();    // get a key

			G.fade_out();     // fade out screen

			G.clearviewport();         // clear screen
			G.setcolor(LIGHTGREEN);                   // re-output heading
			G.settextjustify(CENTER_TEXT, CENTER_TEXT);
			G.outtextxy(159, 5, "Instructions");

			G.settextjustify(LEFT_TEXT, CENTER_TEXT);
			row = 3;  // reset row counter
		}
	}

	G.setcolor(LIGHTRED);                      // prompt for a keyboard hit
	G.outtextxy(30, 195, "Press a key to continue...");
	G.fade_in();   // fade in
	if (getch() == 0) getch();   // get a keypress

	G.fade_out();   // fade back out
}

void Story(void)
// Pre: in graphics mode.
// Post: hiscores displayed.
{
   while (kbhit())   getch();        // this is the same as above, except that it loads the story instead.
	ifstream story("AA4story.TXT");
	int row = 3, stop = 0;
   char line[81];

   G.clearviewport();

   G.setcolor(10);
   G.settextjustify(CENTER_TEXT, CENTER_TEXT);
   G.outtextxy(159, 5, "The Story");

   G.settextjustify(LEFT_TEXT, CENTER_TEXT);
   while (!stop)
   {
      if (story) story.getline(line, 80);
      else { stop = 1; continue; }

      G.setcolor(row+64);
      G.outtextxy(3, row*10, line);
      row++;

      if (row > 17)
      {
      	G.setcolor(12);
         G.outtextxy(30, 195, "Press a key to continue...");
         G.fade_in();

         if (getch() == 0) getch();

         G.fade_out();

         G.clearviewport();
         G.setcolor(LIGHTGREEN);
		   G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		   G.outtextxy(159, 5, "The Story");

   		G.settextjustify(LEFT_TEXT, CENTER_TEXT);
         row = 3;
      }
   }

   G.setcolor(LIGHTRED);
	G.outtextxy(30, 195, "Press a key to continue...");
   G.fade_in();
   if (getch() == 0) getch();

   G.fade_out();
}

void Credits(void)
// Pre: in graphics mode.
// Post: credits displayed.
{
	int i;

	ClrFlags();              // clear keyboard flags
	SetKyb();                // set keyboard handler
	G.fade_to(100);          // fade to 100% intensity
	for (i = 0; i < credits_rows; i++)       // output all rows of credits
	{
		G.write(30, 180, 55, LIGHTCYAN, text[i], 10, EscFlag);
		if (EscFlag)
			break;
		G.scroll_up(10);
		if (EscFlag)
			break;
	}
	UnsetKyb();              // unset keyboard

	G.fade_out();            // fade screen out
}

void EndingInfo(void)
// Pre: In text mode.
// Post: Ending Info printed.
{
	textcolor(LIGHTCYAN);   // self explanatory....
	cprintf("+------------------------------------------------------------------------------+");
   cprintf("|");
   textcolor(LIGHTBLUE);
   cprintf("                            ALIEN ATTACK IV INFO                              ");
   textcolor(LIGHTCYAN);
   cprintf("|");
   cprintf("+------------------------------------------------------------------------------+");
   cprintf("|");
   textcolor(RED);
   cprintf("     This game is freeware.  Please distribute to everybody you know.         ");
   textcolor(LIGHTCYAN);
   cprintf("||");
   textcolor(RED);
   cprintf("   However, do not attempt to incorporate it into any form of a commercial    ");
   textcolor(LIGHTCYAN);
   cprintf("||");
   textcolor(RED);
   cprintf("   software package before contacting me.                                     ");
   textcolor(LIGHTCYAN);
   cprintf("|");
   cprintf("|                                                                              |");
   cprintf("|");
   textcolor(RED);
   cprintf("     All that I ask in return is that if you like this game, and continue     ");
   textcolor(LIGHTCYAN);
   cprintf("||");
   textcolor(RED);
   cprintf("   to play it, then please send me an ");
   textcolor(LIGHTCYAN);
   cprintf("email");
   textcolor(RED);
   cprintf(" at                                ");
   textcolor(LIGHTCYAN);
   cprintf("|");
   cprintf("|                                                                              |");
   cprintf("|");
   textcolor(LIGHTGREEN);
   cprintf("                             ploh618@hotmail.com                              ");
   textcolor(LIGHTCYAN);
   cprintf("|");
   cprintf("|                                                                              |");
   cprintf("|");
   textcolor(RED);
   cprintf("     It does not take more than five minutes to do this simple task, and it   ");
   textcolor(LIGHTCYAN);
   cprintf("||");
   textcolor(RED);
   cprintf("   is completely ");
   textcolor(LIGHTRED);
   cprintf("free");
   textcolor(RED);
   cprintf(", so please do it.                                       ");
   textcolor(LIGHTCYAN);
   cprintf("|");
   cprintf("|                                                                              |");
   cprintf("|");
   cprintf("     Visit my website                                                         ");
   textcolor(LIGHTCYAN);
   cprintf("|");
   cprintf("|                                                                              |");
   cprintf("|");
   textcolor(LIGHTGREEN);
   cprintf("                     http://members.tripod.com/~ploh618                       ");
   textcolor(LIGHTCYAN);
   cprintf("|");
	cprintf("|                                                                              |");
   cprintf("|");
   textcolor(RED);
   cprintf("   for more games and potential updates.                                      ");
   textcolor(LIGHTCYAN);
   cprintf("|");
   cprintf("+------------------------------------------------------------------------------+");
   cprintf("\n");
   textcolor(LIGHTGRAY);
   cprintf("Thank you for playing ");
   textcolor(LIGHTGREEN);
   cprintf("Alien Attack IV.\n");
	cprintf("\n");
   while (kbhit()) getch();
   textcolor(GREEN);
   cprintf("Press a key to quit... ");
   if (getch() == 0) getch();
}

void StartInfo(void)
// Pre: In text mode.
// Post: Starting info printed.
{
	clrscr();
   textbackground(GREEN);
   textcolor(LIGHTCYAN);
   cprintf("                                Alien Attack IV                                 ");
   textbackground(BLACK);
   cprintf("\n");
   textcolor(WHITE);
   cprintf("Copyright (C) 1998 Po-Shen Loh.  All rights reserved.                           ");
   cprintf("\n\n");
   textcolor(LIGHTGRAY);
}
