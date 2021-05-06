#include <alloc.h>      // memory allocation
#include <iostream.h>   // cerr
#include <stdlib.h>     // string functions
#include <string.h>     // string manipulation functions

#include "PGphV120.H"   // my graphics
#include "PxmsV111.H"   // my adapted xms

#include "Aa4GDat.H"    // game data header

#include "Aa4YShip.H"   // header for this module

extern GameData GDat;    // game data
extern XMSObject X;       // xms object
extern GraphObject G;    // graphics object
extern char UpFlag, DownFlag, LeftFlag, RightFlag;  // keyboard
extern long row;   // "time" -- sort of
extern int mem_size;  // temp mem counter
extern VirtualScreen Vscr_G;  // extern vscr


YShip::YShip(void)
// Pre: None.
// Post: yship init.
{
	x = 160;                            // init vars
	y = 200-(GDat.ys_ydim/2+20);
	xmove = 0;
	ymove = 0;
	velocity = 12;
	acc = 3;
	power = 100;

   next_move = 0;
   next_shoot[0] = next_shoot[1] = next_shoot[2] = 0;
   next_acc = 0;
   next_wswitch = 0;
	next_power_up = next_shield_up = 0;

	target_num = 0;
}

YShip::~YShip(void)
// Pre: None.
// Post: None.
{
	return;
}

void YShip::Animate(void)
// Pre: yship init.
// Post: yship animated.
{
	if (power == 100)
   	if (next_shield_up == 0 && shields < 100)
      {
      	power -= 1;
         shields += 1;
         next_shield_up = 0;
      }
      else ;
   else if (next_power_up == 0)
   {
   	power += 1;
      next_power_up = 0;
	}

	x += xmove;                        // move
   y += ymove;
   if (x < GDat.ys_xdim/2+10)              // check out-of-bounds
	{
   	x = GDat.ys_xdim/2+10;
      xmove = 0;
   }
   else if (x > 310 - GDat.ys_xdim/2)
   {
   	x = 310 - GDat.ys_xdim/2;
      xmove = 0;
	}

   if (y < GDat.ys_ydim/2+10)
   {
   	y = GDat.ys_ydim/2+10;
      ymove = 0;
   }
   else if (y > 190 - GDat.ys_ydim/2)
   {
   	y = 190 - GDat.ys_ydim/2;
      ymove = 0;
   }

   mem_size = (GDat.ys_isize+1)/2 * 2;
	img = malloc(mem_size);                // get yship image
	if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.ys_pic_offst, GDat.ys_isize, img) == 0)  // AND
   {
   	cerr << "XMS copy-from error";
      exit(1);
   }
   G.putimage(x-GDat.ys_xdim/2, y-GDat.ys_ydim/2, img, AND_PUT, Vscr_G);   // stuff image on vscr

   if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.ys_pic_offst+mem_size, GDat.ys_isize, img) == 0)   //XOR
   {
   	cerr << "XMS copy-from error";
      exit(1);
   }
	G.putimage(x-GDat.ys_xdim/2, y-GDat.ys_ydim/2, img, XOR_PUT, Vscr_G);   // stuff image on vscr
	free(img);    // free the mem
}

void YShip::InitYShip(Pilot &P)
// Pre: None.
// Post: yship init.
{
	x = 160;                             // init vars
	y = 200-(GDat.ys_ydim/2+20);
   xmove = 0;
   ymove = 0;
   velocity = 12;
	acc = 3;
   power = 100;
   shields = P.shields;

   next_move = 0;
   next_shoot[0] = next_shoot[1] = next_shoot[2] = 0;
   next_acc = 0;
   next_wswitch = 0;
	next_power_up = next_shield_up = 0;

   target_num = 0;
}

void YShip::DrawDash(int primary, int secondary, long money)
// draw status indicators
{
	int i;
	void *img;        // image
	char string[80];  // temp string

	G.setfillcolor(0);                     // delete out the left and right columns
	G.bar(0, 0, 4, 199, Vscr_G);
	G.bar(315, 0, 319, 199, Vscr_G);

	for (i = 0; i < power; i++)    // draw the rainbow-colored power indicator
	{
		G.setcolor(64+i/3);
		G.hline(1, 199-i*2, 3, 199-i*2, Vscr_G);
	}

	for (i = 0; i < shields; i++)  // draw the multi-color shield indicator
	{
		G.setcolor(32+i/3);
		G.hline(316, 199-i*2, 318, 199-i*2, Vscr_G);
	}


   mem_size = (GDat.yweapons[primary].isize+1)/2 * 2;
	img = malloc(mem_size);                // get weapon image
	if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.yweapons[primary].offset, GDat.yweapons[primary].isize, img) == 0)  // AND
	{
		cerr << "XMS copy-from error";
		exit(1);
	}
	G.putimage(7, 198-GDat.yweapons[primary].ydim, img, AND_PUT, Vscr_G);   // stuff image on vscr

	if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.yweapons[primary].offset+mem_size, GDat.yweapons[primary].isize, img) == 0)   //XOR
	{
		cerr << "XMS copy-from error";
		exit(1);
	}
	G.putimage(7, 198-GDat.yweapons[primary].ydim, img, XOR_PUT, Vscr_G);   // stuff image on vscr
	free(img);    // free the mem


	if (secondary != -1)
	{
   	mem_size = (GDat.yweapons[secondary].isize+1)/2 * 2;
		img = malloc(mem_size);                // get weapon image
		if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.yweapons[secondary].offset, GDat.yweapons[secondary].isize, img) == 0)  // AND
		{
			cerr << "XMS copy-from error";
			exit(1);
		}
		G.putimage(7 + GDat.yweapons[primary].xdim + 7, 198-GDat.yweapons[secondary].ydim, img, AND_PUT, Vscr_G);   // stuff image on vscr

		if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.yweapons[secondary].offset+mem_size, GDat.yweapons[secondary].isize, img) == 0)   //XOR
		{
			cerr << "XMS copy-from error";
			exit(1);
		}
		G.putimage(7 + GDat.yweapons[primary].xdim + 7, 198-GDat.yweapons[secondary].ydim, img, XOR_PUT, Vscr_G);   // stuff image on vscr
		free(img);    // free the mem
	}

	G.settextjustify(CENTER_TEXT, CENTER_TEXT);      // output the kilocredits
	G.setcolor(LIGHTGREEN);
	ltoa(money, string, 10);
	strcat(string, " kilocredits");
	G.outtextxy(159, 5, string, Vscr_G);
}
