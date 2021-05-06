#include <fstream.h>          // files
#include <math.h>             // math (plot quad functions)
#include <stdlib.h>           // exit()
#include <dos.h>              // registers
#include <string.h>           // string functions
#include <alloc.h>            // memory allocation
#include <stdio.h>            // sprintf()

#include "PGphV120.H"    // graphics
#include "PxmsV111.H"    // xms
#include "PButV200.H"    // buttons

#include "Aa4shop.H"     // shop
#include "Aa4GDat.H"     // game data

extern GraphObject G;    // graphics
extern XMSObject X;      // xms
extern char quit;        // quit flag
extern char shop_file[20], store_file[20];     // filenames
extern GameData GDat;    // game data
extern int mem_size;     // temp mem counter
extern ButtonObject B;   // button object


void Upgrade(Pilot &P);  // upgrade ship
void Save(Pilot &P);     // save pilot
void DispItem(int itemnum, int num_items, Pilot &P);   // display item
void SellItem(int &itemnum, char *msg, char &msg_disp, Pilot &P);   // sell item
void BuyItem(int itemnum, char *msg, char &msg_disp, Pilot &P);    // buy item
void GoLeft(int &itemnum, char mode, Pilot &P);   // go left
void GoRight(int &itemnum, char mode, Pilot &P);  // go right
void SwitchMode(char &mode, int &itemnum, Pilot &P);   // switch buy/sell mode


void DrawCursor(int x_cur, int y_cur)
// draw mouse cursor
{
	G.setcolor(CYAN);
	G.hline(x_cur-2, y_cur, x_cur+2, y_cur);
	G.vline(x_cur, y_cur-2, x_cur, y_cur+2);
	G.setcolor(BLACK);
	G.hline(x_cur-3, y_cur-1, x_cur-1, y_cur-1);
	G.hline(x_cur-3, y_cur+1, x_cur-1, y_cur+1);
	G.hline(x_cur+1, y_cur-1, x_cur+3, y_cur-1);
	G.hline(x_cur+1, y_cur+1, x_cur+3, y_cur+1);
	G.vline(x_cur-1, y_cur-3, x_cur-1, y_cur-2);
	G.vline(x_cur+1, y_cur-3, x_cur+1, y_cur-2);
	G.vline(x_cur-1, y_cur+1, x_cur-1, y_cur+3);
	G.vline(x_cur+1, y_cur+1, x_cur+1, y_cur+3);
	G.putpixel(x_cur-3, y_cur, BLACK);
	G.putpixel(x_cur+3, y_cur, BLACK);
	G.putpixel(x_cur, y_cur-3, BLACK);
	G.putpixel(x_cur, y_cur+3, BLACK);
}

void y_plot(float a, float b, float c,
				float start, float end,
				float x_off, float y_off)
// plot quad function: y = ax^2 + bx + c from x=start to x=end, with an added offset
//  of (+x_off, +yoff)
{
	int i;

	for (i = start; i <= end-1; i++)  // draw the function
		G.line(x_off+i, y_off + a*i*i + b*i + c, x_off+i+1, y_off + a*(i+1)*(i+1) + b*(i+1) + c);
}

void x_plot(float a, float b, float c,
				float start, float end,
				float x_off, float y_off)
// plot x = ay^2 + by + c from y=start to y=end, with offset of (+x_off, +y_off).
{
	int i;

	for (i = start; i <= end-1; i++)
		G.line(x_off + a*i*i + b*i + c, y_off+i, x_off + a*(i+1)*(i+1) + b*(i+1) + c, y_off+i+1);
}

void Interpret(char *filename)
// interpret a file and put it on screen
{
	ifstream inf(filename);
	char opcode[30];
   float x1, y1, x2, y2;
   float a, b, c;
   float start, end;
   float x_off, y_off;
   float x, y, x_rad, y_rad;
   char string[501];

	G.clearviewport();

	while (filename)                   // parse file.. this segment is pretty self-explanatory
   {
   	inf >> opcode;
      if (!strcmpi(opcode, "HLINE"))
		{
      	inf >> x1 >> y1 >> x2 >> y2;
         G.hline(x1, y1, x2, y2);
      }
      else if (!strcmpi(opcode, "VLINE"))
      {
      	inf >> x1 >> y1 >> x2 >> y2;
         G.vline(x1, y1, x2, y2);
		}
      else if (!strcmpi(opcode, "LINE"))
      {
      	inf >> x1 >> y1 >> x2 >> y2;
         G.line(x1, y1, x2, y2);
      }
      else if (!strcmpi(opcode, "QUAD_Y="))   // y=
      {
      	inf >> a >> b >> c >> start >> end >> x_off >> y_off;
      	y_plot(a, b, c, start, end, x_off, y_off);          // y = ax*x + bx + c
      }
      else if (!strcmpi(opcode, "QUAD_X="))   // x=
		{
      	inf >> a >> b >> c >> start >> end >> x_off >> y_off;
         x_plot(a, b, c, start, end, x_off, y_off);          // x = a*y*y + by + c
      }
      else if (!strcmpi(opcode, "ELLIPSE"))
      {
      	inf >> x >> y >> start >> end >> x_rad >> y_rad;
			G.ellipse(x, y, start, end, x_rad, y_rad);
      }
      else if (!strcmpi(opcode, "PUTPIXEL"))
      {
      	inf >> a >> b >> c;
			G.putpixel(a, b, c);
      }
      else if (!strcmpi(opcode, "SETCOLOR"))
      {
      	inf >> a;
         G.setcolor(a);
		}
		else if (!strcmpi(opcode, "SETFILLCOLOR"))
		{
			inf >> a;
			G.setfillcolor(a);
		}
		else if (!strcmpi(opcode, "SETBACK"))
      {
      	inf >> a;
         G.setbkcolor(a);
         G.clearviewport();
      }
      else if (!strcmpi(opcode, "OUTTEXTXY"))
      {
      	inf >> x >> y >> string;
         G.outtextxy(x, y, string);
      }
      else if (!strcmpi(opcode, "SETTEXTJUSTIFY"))
      {
      	inf >> a >> b;
         G.settextjustify(a, b);
      }
		else if (!strcmpi(opcode, "FLOODFILL"))
      {
      	inf >> a >> b >> c;
         G.h_floodfill(a, b, c);
      }
      else if (!strcmpi(opcode, "SETFILLCOLOR"))
      {
      	inf >> a;
         G.setfillcolor(a);
      }
      else if (!strcmpi(opcode, "REM"))
      	inf.getline(string, 500);
      else if (!strcmpi(opcode, "RECTANGLE"))
      {
      	inf >> x1 >> y1 >> x2 >> y2;
         G.rectangle(x1, y1, x2, y2);
		}
		else if (!strcmpi(opcode, "BAR"))
		{
			inf >> x1 >> y1 >> x2 >> y2;
			G.bar(x1, y1, x2, y2);
		}
		else
      	break;
   }
   inf.close();

   G.setbkcolor(BLACK);    // set background color back to black
}


void Shop(Pilot &P)
// Pre: In graphics mode.
// Post: User shops for upgrades.
{
	char color;                // color of screen beneath mouse cursor
	char descrip[101] = "";    // description of what mouse is under
	REGPACK regs;              // registers
	int nx, ny, x_cur, y_cur;  // position of cursor
	void *oldimg;

	oldimg = farmalloc(G.imagesize(0, 0, 6, 6));  // get mem for old mouse image

	do
	{
		Interpret(shop_file);   // interpret the file
		G.fade_in();    // fade in

		regs.r_ax = 0x0003;       // do mouse stuff
		intr(0x33, &regs);
		x_cur = regs.r_cx/2; //The column of the mouse cursor is returned
		y_cur = regs.r_dx; //The row of the mouse cursor is returned

		G.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg);
		G.setcolor(CYAN);
		DrawCursor(x_cur, y_cur);

	// make mouse move around
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		do
		{
			regs.r_ax = 0x0003;
			intr(0x33, &regs);
			nx = regs.r_cx/2;
			ny = regs.r_dx;
			if (nx != x_cur || ny != y_cur)
			{
				G.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT);
				x_cur = nx;     y_cur = ny;
				G.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg);
				DrawCursor(x_cur, y_cur);
			}

			color = *((char*)oldimg + 2*sizeof(int) + 24);  // find the color of the pixel beneath the cursor
			if (color == 26 || color == 54)              // branch based on what the cursor is on top of
			{
				if (x_cur < 160)
					if (strcmp(descrip, "UPGRADE SHIP"))
					{
						G.setcolor(139);
						G.outtextxy(159, 185, descrip);
						strcpy(descrip, "UPGRADE SHIP");
					}
					else ;
				else
					if (strcmp(descrip, "EXIT TO MENU"))
					{
						G.setcolor(139);
						G.outtextxy(159, 185, descrip);
						strcpy(descrip, "EXIT TO MENU");
					}
				G.setcolor(10);
				G.outtextxy(159, 185, descrip);
			}
			else if (color == 0 || color == 98)
			{
				if (strcmp(descrip, "SAVE GAME"))
				{
					G.setcolor(139);
					G.outtextxy(159, 185, descrip);
					strcpy(descrip, "SAVE GAME");
				}
				G.setcolor(10);
				G.outtextxy(159, 185, descrip);
			}
			else if (x_cur >= 170 && x_cur < 240 && y_cur > 50 && y_cur < 100)
			{
				if (strcmp(descrip, "FLY MISSION"))
				{
					G.setcolor(139);
					G.outtextxy(159, 185, descrip);
					strcpy(descrip, "FLY MISSION");
				}
				G.setcolor(10);
				G.outtextxy(159, 185, descrip);
			}
			else
			{
				if (strcmp(descrip, ""))
				{
					G.setcolor(139);
					G.outtextxy(159, 185, descrip);
					strcpy(descrip, "");
				}
				G.setcolor(10);
				G.outtextxy(159, 185, descrip);
				}
		} while (!(regs.r_bx%2) || !strcmp(descrip, ""));


		G.fade_out();        // fade out
		G.clearviewport();   // clear screen
		switch (descrip[0])         // branch on the user choice
		{
			case 'U' : Upgrade(P);  // upgrade ship
						  break;
			case 'E' : break;
			case 'F' : break;
			case 'S' : Save(P);    // save game
						  break;
			default  : cerr << "OH NO!";
		}
	} while (descrip[0] != 'E' && descrip[0] != 'F');

	if (descrip[0] == 'E')   // "exit to menu"
	{
		quit = 1;
		Save(P);
	}

	farfree(oldimg);    // free mem
	G.fade_out();       // fade out
}

void Upgrade(Pilot &P)
// upgrade ship
{
	char mode = 'B';       // mode
	char *choice;
	int itemnum;           // item counter
	char msg[80] = "";
	char msg_disp = 0;     // message displayed

	Interpret(store_file);   // interpret the store file
	B.AddButton(270, 170, 310, 190, RED, "Exit", "Exit");     // add the buttons
	B.AddButton(40, 170, 80, 190, CYAN, "<<", "<<");
	B.AddButton(100, 170, 180, 190, GREEN, "Buy Item", "Buy Item");
	B.AddButton(200, 170, 240, 190, CYAN, ">>", ">>");
	B.AddButton(260, 60, 300, 100, BLUE, "MODE", "MODE");
	B.ShowAllButtons();             // show all buttons
	itemnum = 0;
	G.setcolor(12);
	DispItem(itemnum, GDat.num_yweaps, P);     // display the item
	G.fade_in();

	do
	{
		choice = B.TrapClick(LEFT_BUTTON);

		if (msg_disp)
		{
			G.setcolor(22);
			G.settextjustify(CENTER_TEXT, CENTER_TEXT);
			G.outtextxy(130, 10, msg);
			msg_disp = 0;
		}

		switch (choice[0])          // branch on user choice
		{
			case 'E' : break;
			case '<' : GoLeft(itemnum, mode, P);
						  break;
			case '>' : GoRight(itemnum, mode, P);
						  break;
			case 'B' : BuyItem(itemnum, msg, msg_disp, P);
						  break;
			case 'S' : SellItem(itemnum, msg, msg_disp, P);
						  break;
			case 'M' : SwitchMode(mode, itemnum, P);
						  break;
			default  : break;
		}
	} while (choice[0] != 'E');

	G.fade_out();
	B.RemoveButton("Exit");                       // clean out buttons
	B.RemoveButton("<<");
	if (mode == 'B')
		B.RemoveButton("Buy Item");
	else
	   B.RemoveButton("Sell Item");
   B.RemoveButton(">>");
	B.RemoveButton("MODE");
}

void Save(Pilot &P)
// save pilot
{
	char *choice;
	long max_id;
	char filename[20];
   ifstream inf;            // update save files
   ofstream outf;

	B.AddButton(120, 100, 150, 115, GREEN, "Yes", "Yes");      // prompt
	B.AddButton(170, 100, 200, 115, GREEN, "No", "No");
	G.fade_to(100);
	G.write(10, 50, 55, LIGHTCYAN, "Do you wish to save?", 30);     // prompt animation
	B.ShowAllButtons();
	choice = B.TrapClick(LEFT_BUTTON);
	G.fade_out();

	if (choice[0] == 'Y')
	{
   	inf.open("AA4.SV", ios::in);
		inf >> max_id;
		inf.close();

      if (max_id < P.ID)
		{
      	outf.open("AA4.SV", ios::out);
         outf << P.ID;
         outf.close();
      }

      sprintf(filename, "SAVGAME.%03d", P.ID);
      outf.open(filename, ios::out);
		outf.write((char*)&P, sizeof(Pilot));
      outf.close();
   }

	B.RemoveButton("Yes");  // remove buttons
	B.RemoveButton("No");
}



void DispItem(int itemnum, int num_items, Pilot &P)
// display item
{
	void *img;
	int i;
	char str[20];

	G.setfillcolor(0);
	G.bar(23, 23, 58, 58);
   mem_size = (GDat.yweapons[itemnum].isize+1)/2 * 2;
	img = malloc(mem_size);   // get image
	if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.yweapons[itemnum].offset + mem_size, GDat.yweapons[itemnum].isize, img) == 0)
	{
		cerr << "XMS copy-from error";
		exit(1);
	}
	G.putimage(40-GDat.yweapons[itemnum].xdim/2, 40-GDat.yweapons[itemnum].ydim/2, img, COPY_PUT);
	free(img);


	G.settextjustify(LEFT_TEXT, CENTER_TEXT);
	for (i = 0; i < GDat.yweapons[itemnum].lines; i++)
		G.outtextxy(67, 30 + i*10, GDat.yweapons[itemnum].descrip[i]);

	G.settextjustify(CENTER_TEXT, CENTER_TEXT);               // output data on item in shop
	if (GDat.yweapons[itemnum].primary)
		G.outtextxy(40, 80, "YES");
	else
		G.outtextxy(40, 80, "NO");

	sprintf(str, "%d", GDat.yweapons[itemnum].power);
	G.outtextxy(40, 120, str);

	sprintf(str, "PRICE: %ldC", GDat.yweapons[itemnum].price);
	G.outtextxy(60, 149, str);

	sprintf(str, "YOU: %ldC", P.credits);
	G.outtextxy(140, 149, str);

	sprintf(str, "%d/%d", (itemnum+1), num_items);
	G.outtextxy(210, 149, str);
}

void SellItem(int &itemnum, char *msg, char &msg_disp, Pilot &P)
// sell item
{
	char str[30];   // this is very self-explanatory

	if (!P.weapon_list[itemnum])
	{
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		G.setcolor(64);
		strcpy(msg, "You do not own this weapon");
		msg_disp = 1;
		G.outtextxy(130, 10, msg);
	}
	else if (GDat.yweapons[itemnum].primary)
	{
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		G.setcolor(64);
		strcpy(msg, "You cannot sell primary weapons -- only replace them");
		msg_disp = 1;
		G.outtextxy(130, 10, msg);
	}
	else
	{
		if (P.secondary == itemnum)        // if it is a secondary, then just sell it
		{
			do
			{
				P.secondary++;
				if (P.secondary >= GDat.num_yweaps)
					P.secondary -= GDat.num_yweaps;
				if (P.weapon_list[P.secondary] && !GDat.yweapons[P.secondary].primary)
					break;
			} while (P.secondary != itemnum);

			if (P.secondary == itemnum)
				P.secondary = -1;
		}
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		G.setcolor(64);
		strcpy(msg, "Secondary weapon sold");
		msg_disp = 1;
		G.outtextxy(130, 10, msg);
		P.weapon_list[itemnum] = 0;
		G.setcolor(0);
		sprintf(str, "YOU: %ldC", P.credits);
		G.outtextxy(140, 149, str);
		P.credits = P.credits + GDat.yweapons[itemnum].price;
		G.setcolor(12);
		sprintf(str, "YOU: %ldC", P.credits);
		G.outtextxy(140, 149, str);
      GoRight(itemnum, 'S', P);
	}
}

void BuyItem(int itemnum, char *msg, char &msg_disp, Pilot &P)
// buy item
{
	char str[30];
   
	if (P.weapon_list[itemnum])   // if you own this
	{
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		G.setcolor(64);
		strcpy(msg, "You already own this weapon");
		msg_disp = 1;
		G.outtextxy(130, 10, msg);
	}
	else if ((!GDat.yweapons[itemnum].primary && GDat.yweapons[itemnum].price > P.credits) || (GDat.yweapons[itemnum].primary && GDat.yweapons[itemnum].price > P.credits+GDat.yweapons[P.primary].price))  // not enough money
	{
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		G.setcolor(64);
		strcpy(msg, "Insufficient Credit!");
		msg_disp = 1;
		G.outtextxy(130, 10, msg);
	}
	else if (GDat.yweapons[itemnum].primary)  // it is a primary... must replace
	{
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		G.setcolor(64);
		strcpy(msg, "Your primary weapon has been replaced (by this)");
		msg_disp = 1;
		G.outtextxy(130, 10, msg);
		P.weapon_list[P.primary] = 0;
		P.weapon_list[itemnum] = 1;
      G.setcolor(0);
      sprintf(str, "YOU: %ldC", P.credits);
		G.outtextxy(140, 149, str);
		P.credits = P.credits + GDat.yweapons[P.primary].price - GDat.yweapons[itemnum].price;
      G.setcolor(12);
      sprintf(str, "YOU: %ldC", P.credits);
		G.outtextxy(140, 149, str);
		P.primary = itemnum;
	}
	else  // arm secondary
	{
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		G.setcolor(64);
		strcpy(msg, "Secondary weapon armed");
		msg_disp = 1;
		G.outtextxy(130, 10, msg);
      G.setcolor(0);
      sprintf(str, "YOU: %ldC", P.credits);
		G.outtextxy(140, 149, str);
		P.credits = P.credits - GDat.yweapons[itemnum].price;
      G.setcolor(12);
      sprintf(str, "YOU: %ldC", P.credits);
		G.outtextxy(140, 149, str);
		P.secondary = itemnum;
      P.weapon_list[itemnum] = 1;
	}
}

void GoLeft(int &itemnum, char mode, Pilot &P)
// go left
{
	int old_itemnum = itemnum;       // this moves left in the list

	if (mode == 'B')
	{
		itemnum--;
		if (itemnum < 0)    itemnum += GDat.num_yweaps;
	}
	else
	{
		do
		{
			itemnum--;
			if (itemnum < 0) itemnum += GDat.num_yweaps;
		} while (!P.weapon_list[itemnum]);
	}

	if (old_itemnum != itemnum)    // only update if need be.
	{
		G.setcolor(0);
		DispItem(old_itemnum, GDat.num_yweaps, P);
		G.setcolor(12);
		DispItem(itemnum, GDat.num_yweaps, P);
	}
}

void GoRight(int &itemnum, char mode, Pilot &P)
// go right.
{
	int old_itemnum = itemnum;   // this is same as before.

	if (mode == 'B')
	{
		itemnum++;
		if (itemnum >= GDat.num_yweaps)    itemnum -= GDat.num_yweaps;
	}
	else
	{
		do
		{
			itemnum++;
			if (itemnum >= GDat.num_yweaps) itemnum -= GDat.num_yweaps;
		} while (!P.weapon_list[itemnum]);
	}

	if (old_itemnum != itemnum)
	{
		G.setcolor(0);
		DispItem(old_itemnum, GDat.num_yweaps, P);
		G.setcolor(12);
		DispItem(itemnum, GDat.num_yweaps, P);
	}
}

void SwitchMode(char &mode, int &itemnum, Pilot &P)
// switch buy/sell modes.
{
	if (mode == 'B')
	{
		mode = 'S';
		B.RemoveButton("Buy Item");   // update buttons
		B.AddButton(100, 170, 180, 190, GREEN, "Sell Item", "Sell Item");

		int old_itemnum = itemnum;
		do                           // move to next element to sell
		{
			itemnum++;
			if (itemnum >= GDat.num_yweaps) itemnum -= GDat.num_yweaps;
		} while (!P.weapon_list[itemnum]);
		if (old_itemnum != itemnum)  // update if you have to
		{
			G.setcolor(0);
			DispItem(old_itemnum, GDat.num_yweaps, P);
			G.setcolor(12);
			DispItem(itemnum, GDat.num_yweaps, P);
		}
	}
	else
	{
		mode = 'B';
		B.RemoveButton("Sell Item");   // update buttons
		B.AddButton(100, 170, 180, 190, GREEN, "Buy Item", "Buy Item");
	}
	B.ShowAllButtons();  // show all new buttons
}
