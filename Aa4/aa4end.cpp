#include <conio.h>    // for getch()
#include <fstream.h>

#include "PGphV120.H"   // graphics

#include "Aa4end.H"    // header for endgame


extern GraphObject G;   // external graphics object.. for vga 320x200x256 mode.


void Victory(void)
// Pre: In graphics mode.
// Post: Victory message displayed.
{
	while (kbhit())   getch();             // clear keyboard buffer
	ifstream story("AA4win.TXT");        // instruction file
	int row = 3, stop = 0;                 // counters
	char line[81];                         // temp string for line

	G.clearviewport();       // clear screen

	G.setcolor(10);                                // write the heading
	G.settextjustify(CENTER_TEXT, CENTER_TEXT);
	G.outtextxy(159, 5, "Victory!");

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
			G.outtextxy(159, 5, "Victory!");

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

void Defeat(void)
// Pre: In graphics mode.
// Post: Defeat message displayed.
{
	while (kbhit())   getch();             // clear keyboard buffer
	ifstream story("AA4die.TXT");        // instruction file
	int row = 3, stop = 0;                 // counters
	char line[81];                         // temp string for line

	G.clearviewport();       // clear screen

	G.setcolor(10);                                // write the heading
	G.settextjustify(CENTER_TEXT, CENTER_TEXT);
	G.outtextxy(159, 5, "Loser!");

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
			G.outtextxy(159, 5, "Loser!");

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
