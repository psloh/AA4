#include <math.h>         // math functions
#include <iostream.h>     // cerr
#include <stdlib.h>       // exit()
#include <alloc.h>        // memory allocation functions

#include "PGphV120.H"     // my graphics
#include "PxmsV111.H"     // my adapted xms
extern "C"
{
	#include "detect.h"         // ethan's sound
	#include "smix.h"
}

#include "Aa4GDat.H"      // game data
#include "Aa4Snds.H"      // sounds & music

#include "Aa4Level.H"     // header for this module

extern long row;    // external row of ship... in game module
extern GraphObject G;  // external graph object.. main module
extern XMSObject X;  // external xms object... main
extern GameData GDat;  // external game data
extern char EscFlag;   // escape flag
extern char lev_fin;   // level finished.
extern char Music;     // music flag
extern SOUND *Sound[];     // in aa4snds.cpp
extern VirtualScreen Vscr_G;  // extern vscr

const int isize = G.imagesize(0, 0, 15, 15);      // constant tile size


inline long min(long a, long b)   // calculates minimum of a & b
{
	return a<b?a:b;
}

Level::Level(void)
// Pre: None.
// Post: None.
{
	return;
}

Level::~Level(void)
// Pre: None.
// Post: None.
{
	return;
}

void Level::Animate(void)
// Pre: Level initialized.
// Post: Level animated (background).
{
	long row_on_top = min(lev_rows-30, row+200);          // row at top of screen
	long tile_row_top = ceil((row_on_top+1) / 16.0);      // tile row # at top
	long tile_row_bot = ceil((row_on_top-200+1) / 16.0);  // tile row # at bottom

	if (row >= lev_rows-200-30) lev_fin = 1;

	long row_ctr;        // row counter
	int i;               // counter
	lev_node_ptr cur;    // traversal temp ptr

#pragma warn -sig
	int y = (row_on_top)%16 - 16;    // compute y value of first strip
#pragma warn .sig

	for (row_ctr = tile_row_top-1; row_ctr >= tile_row_bot-1; row_ctr--)  // loop thru strips
	{
		cur = lev_strip_defs;                         // get the right strip definition
#pragma warn -sig
		for (i = 0; i < lev_strips[row_ctr]; i++)
			cur = cur->next;
#pragma warn .sig

		for (i = 0; i < 20; i++)   // put down the 20 tiles
		{
			if (i == 0 || cur->tiles[i] != cur->tiles[i-1])

			if (X.CopyFromEMB(GDat.xms_pic_handle, land_offset+isize*(cur->tiles[i]-1), isize, img) == 0)   // get a tile from xms
			{
				cerr << "XMS copy-from error";
				exit(1);
			}
			G.putimage_fast(i*16, y, img, Vscr_G);  // stuff it on the virtual screen
		}
		y += 16;   // increment y counter
	}
}

void Level::ShowInfo(int level_no)
// show level briefing
{
	char string[80];   // temp string
	int i;

	G.clearviewport();                           // clear screen
	G.setcolor(93);
	G.settextjustify(LEFT_TEXT, TOP_TEXT);
	G.hline(160-34, 87, 160+33, 87);
	G.outtextxy(160-32, 90, "Mission briefing");
	G.hline(160-34, 97, 160+33, 97);
	itoa(level_no, string, 10);
	G.moveto(5, 185);
	G.outtext("Level: ");
	G.outtext(string);
	G.fade_in();                                 // show heading
   G.scroll_up(20, EscFlag);                    // scroll up 20 pix, with escape-checking
   if (EscFlag)
   {
#pragma warn -eff
   	while (EscFlag) ;
#pragma warn .eff
		G.fade_out();
   	return;
   }

	for (i = 0; i < ld_lines; i++)   // loop thru description lines and output them all.
	{
		G.write(5, 185, 55, LIGHTCYAN, lev_descrip[i], 20, EscFlag);    // write one line on screen, with esc-checking
		if (EscFlag)
		{
#pragma warn -eff
			while (EscFlag) ;
#pragma warn .eff
			G.fade_out();
			return;
		}
		G.scroll_up(10, EscFlag);     // scroll up 10 pix, with esc-checking.
		if (EscFlag)
		{
#pragma warn -eff
			while (EscFlag) ;
#pragma warn .eff
			G.fade_out();
			return;
		}
	}

	if (!EscFlag)
		G.scroll_up(30);       // scroll up 30 pixels.
	else
	{
#pragma warn -eff
		while (EscFlag) ;
#pragma warn .eff
		G.fade_out();
		return;
	}

	G.outtextxy(5, 190, "Press escape to continue...");    // prompt for and get an esc.
#pragma warn -eff
	while (!EscFlag) ;
	while (EscFlag)  ;
#pragma warn .eff
	G.fade_out();
}

void Level::ResetMusic(void)
// reset the shop music
{
	if (Music)    // if music enabled
	{
		stop_sound(track);    // play it
		start_sound(Sound[SHOP_SOUND], SHOP_SOUND, ON);
	}
}
