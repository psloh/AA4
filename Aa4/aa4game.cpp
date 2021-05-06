#include <conio.h>               // getch()
#include <string.h>              // string conversions
#include <dos.h>                 // dos functions
#include <fstream.h>             // files
#include <stdio.h>               // conversions of numbers->strings

#include "PGphV120.H"            // my graphics
#include "PButV200.H"            // my buttons

#include "Aa4Kybd.H"             // my keyboard

#include "Aa4Game.H"             // header for this module

#include "Aa4Level.H"            // other classes used
#include "Aa4Good.H"
#include "Aa4EProj.H"
#include "Aa4YProj.H"
#include "Aa4EShip.H"
#include "Aa4YShip.H"
#include "Aa4GDat.H"             // struct for game data
#include "Aa4Exps.H"             // explosions
#include "Aa4Shop.H"             // shop
#include "Aa4end.H"              // endgame


extern GraphObject G;   // external graphics object
extern ButtonObject B;  // external button object
extern VirtualScreen Vscr_G;  // extern vscr


char dead, won, quit, lev_fin, e_fin;    // globals for view in other files
long row;   // row of level; at the start you see row 0.
long money;   // amt of money you have
int brightness;    // brightness ctr.

YShip Y;               // game classes
EShips E;
YProjs YP;
EProjs EP;
Goodies Gd;
Level L;
GameData GDat;
Explosions XP;


void DrawStats(Pilot &P);     // draw user stats
char PromptDel(void);         // prompt for deletion
void DoDelete(int current);   // delete from files


void NewGame(void)
// Pre: in graphics mode.
// Post: new pilot created.
{
	char player_name[80];      // name of player
	Pilot P;                   // pilot structure
	int i;                     // counter

	G.fade_to(100);    // fade to 100% intensity

	G.write(10, 75, 55, LIGHTCYAN, "Please enter your name:", 30);   // get name
	G.setcolor(93);
	G.rectangle(10, 105, 310, 116);

	ClrKybBuf();    // clear keyboard buffer
	G.intext(13, 108, 307, 113, GREEN, player_name);   // input name

   strcpy(P.name, player_name);              // move all data to pilot struct
   P.credits = 10000;
   P.level = 1;
   P.weapon_list[0] = 1;  // basic cannon.
   for (i = 1; i < NUM_WEAPONS; i++)
   	P.weapon_list[i] = 0;
	P.primary = 0;
   P.secondary = -1;
   P.shields = 100;

   ifstream inf("Aa4.SV");
   inf >> P.ID;                // # of ID's saved so far
   inf.close();
   P.ID++;                     // take next one

   G.fade_out();

   Game(P);         // begin the game!
}

void LoadGame(void)
// Pre: in graphics mode.
// Post: loads game and plays
{
	int num_saved;          // number of savegames
	int current;            // current counter
	char *choice;           // user choice
	Pilot P;                // temp pilot
	ifstream test;          // test fstream
	char filename[13];      // temp filename
	char play;              // play/stop flags
	char stop;

	G.fade_to(100);

	ifstream inf("Aa4.SV");
	inf >> num_saved;
	inf.close();

	if (num_saved <= 0)   // no savegames ==> this error message
	{
		G.write(10, 75, 55, LIGHTCYAN, "No Saved Games Exist!", 30);
		if (getch() == 0) getch();
		G.fade_out();
		return;
	}

	G.write(10, 15, 55, LIGHTCYAN, "Select a game to load:", 30);  // prompt

	current = 1;                                   // init counter
	sprintf(filename, "SAVGAME.%03d", current);    // load in a pilot
	test.open(filename, ios::in);
	test.read((char*)&P, sizeof(Pilot));
	test.close();
	G.setcolor(64);
	DrawStats(P);           // draw pilot's stats

	B.AddButton(120, 100, 155, 115, CYAN, "<<", "<<");    // add buttons
	B.AddButton(165, 100, 200, 115, CYAN, ">>", ">>");
	B.AddButton(120, 120, 155, 135, CYAN, "CANCEL", "CANCEL");
	B.AddButton(165, 120, 200, 135, CYAN, "OK", "OK");
	B.AddButton(135, 140, 185, 155, RED, "DELETE", "DELETE");
	B.ShowAllButtons();   // show them all

	stop = 0;
	do
	{
		choice = B.TrapClick(LEFT_BUTTON);  // get user choice

		switch (choice[0])                // branch on 1st char of user choice
		{
			case '<' : G.setcolor(0);           // cycle left (backwards)
						  DrawStats(P);
						  current--;
						  if (current < 1)  current += num_saved;
						  sprintf(filename, "SAVGAME.%03d", current);
						  test.open(filename, ios::in);
						  test.read((char*)&P, sizeof(Pilot));
						  test.close();
						  G.setcolor(64);
						  DrawStats(P);
						  break;
			case '>' : G.setcolor(0);        // cycle right (forwards)
						  DrawStats(P);
						  current++;
						  if (current > num_saved)  current -= num_saved;
						  sprintf(filename, "SAVGAME.%03d", current);
						  test.open(filename, ios::in);
						  test.read((char*)&P, sizeof(Pilot));
						  test.close();
						  G.setcolor(64);
						  DrawStats(P);
                    break;
			case 'C' : play = 0;     // "cancel" -- don't play
                    break;
			case 'O' : play = 1;     // "ok" -- do play
                    break;
			case 'D' : if (PromptDel() == 'N') break;     // delete, but prompt first
						  G.setcolor(0);
						  DrawStats(P);
						  DoDelete(current);   // delete it
						  current--;       // update stuff
						  num_saved--;
						  if (num_saved <= 0)
						  {
							  G.clearviewport();
							  G.write(10, 75, 55, LIGHTCYAN, "No Saved Games Exist!", 30);
							  if (getch() == 0) getch();
							  play = 0;
							  stop = 1;
							  break;
						  }
						  if (current < 1)   current += num_saved;
						  sprintf(filename, "SAVGAME.%03d", current);    // load new pilot
						  test.open(filename, ios::in);
						  test.read((char*)&P, sizeof(Pilot));
						  test.close();
						  G.setcolor(64);
						  DrawStats(P);
						  break;
			default  : break;
		}
	} while (choice[0] != 'C' && choice[0] != 'O' && !stop);

	B.RemoveButton("<<");       // remove all buttons
	B.RemoveButton(">>");
	B.RemoveButton("CANCEL");
	B.RemoveButton("OK");
	B.RemoveButton("DELETE");

	G.fade_out();              // fade out

	if (play)
		Game(P);      // if "ok" selected, then play!
}


// real game section

void Game(Pilot &P)
// Pre: graphics.
// Post: game played with pilot.
{
	int i;
   long next_animate, cur_time;   // animation timers
   struct time t;  // time struct

	quit = 0;     // not quit yet
	LoadGameData(GDat, XP);   // load game data

	do
	{
		dead = won = 0;           // init flags
		Shop(P);        // go to shop

		if (!quit)
		{
			money = P.credits;       // set global money flag to pilot field
			LoadLevel(L, E, EP, P.level);   // load level
			Y.InitYShip(P);                            // init stuff
			InitProjs(YP, EP);     // init projectiles
			Gd.InitGoodies();      // init goodies
			XP.Init();             // init explosions
			lev_fin = e_fin = 0;   // set finished flags
			row = 0;               // set current row to zero (start of level)

			ClrFlags();   // clear keyboard flags
			SetKyb();     // set keyboard handler

			L.ShowInfo(P.level);      // show level info

			brightness = 0;                            // intensity to 0%
         next_animate = 0;      // animate right away.

			while (!dead && !quit && !(lev_fin && e_fin))     // game loop
			{
				if (brightness < 100)    // if not yet at 100% intensity
				{
					G.fade_to(brightness);         // brighten up
					brightness += 7;
					if (brightness > 100)   // check for overflow
						brightness = 100;
				}

				L.Animate();             // animate everything
				YP.Animate();
				EP.Animate();
				Gd.Animate();
				Animate(E, EP, Y);
				Y.Animate();
				XP.Animate();

				CheckYP(YP, E, Gd);                // check everything
				CheckEP(EP, Y);
				CheckE(E, Y, Gd);
				CheckGd(Gd, Y, P);
				CheckKybd(Y, YP, E, P);

				Y.DrawDash(P.primary, P.secondary, money);    // draw status indicators

				do
				{
					gettime(&t);
					cur_time = 360000L*t.ti_hour + 6000L*t.ti_min + 100L*t.ti_sec + t.ti_hund;
				} while (cur_time < next_animate && next_animate < 2*cur_time);
				next_animate = cur_time + GDat.speed_mode;

				G.swap(VGAaddr, FP_SEG(Vscr_G));    // swap screens lightning-quick
				row += SPEED;                   // move up a few rows
			}

			for (i = 0; i < XP.num_stages+20 && !quit; i++)
			{
				L.Animate();             // animate everything
				YP.Animate();
				EP.Animate();
				Gd.Animate();
				Animate(E, EP, Y);
				if (!dead)
					Y.Animate();
				XP.Animate();

				CheckYP(YP, E, Gd);                // check everything

				Y.DrawDash(P.primary, P.secondary, money);      // draw indicators

            do
            {
            	gettime(&t);
               cur_time = 360000L*t.ti_hour + 6000L*t.ti_min + 100L*t.ti_sec + t.ti_hund;
            } while (cur_time < next_animate && next_animate < 2*cur_time);
            next_animate = cur_time + GDat.speed_mode;

				G.swap(VGAaddr, FP_SEG(Vscr_G));    // swap screens lightning-quick
				row += SPEED;                   // move up a few rows
			}

			UnsetKyb();    // unset keyboard handler

         G.fade_out();    // self-explanatory

			if (e_fin && lev_fin && !dead)      // if you legally finished, then up your level
			{
				if (P.level == GDat.max_level)
					won = 1;
				P.level++;
			}
			if (won)
			{
				P.level = 1;    // if you won, then loop back around
				Victory();                                   // victory screens
			}

			L.ResetMusic();      // go back to playing shop music

			XP.Destruct();       // clean out all memory for explosions
			Destroy(E, YP, EP, Gd, L);     // clean out all memory

			P.credits = money;    // reset your pilot's field for credits to global money var
		}
	} while (!dead && !quit);

	if (dead)     // if you died
		Defeat();  // view defeat sequence
}


void DrawStats(Pilot &P)
// draw pilot stats
{
	char string[30];   // temp string

	G.moveto(100, 50);                                // output pilot name
   G.settextjustify(LEFT_TEXT, CENTER_TEXT);
   G.outtext("Name: ");
   G.outtext(P.name);

	sprintf(string, "Kilocredits: %ld", P.credits);   // output pilot kcredits
   G.outtextxy(100, 60, string);

	sprintf(string, "Level: %d", P.level);            // output pilot level
	G.outtextxy(100, 70, string);
}

void DoDelete(int current)
// delete current savegame from files
{
	int num_saved;        // number of savegames
   int i;
	Pilot P;              // temp pilot
	char filename[20];    // filename

	ifstream inf("Aa4.SV");         // get # of savegames
   inf >> num_saved;
   inf.close();
	ofstream outf("Aa4.SV");        // update that #
   outf << (num_saved-1);
   outf.close();

	for (i = current+1; i <= num_saved; i++)         // shift all subsequent savegames forward one
	{
		sprintf(filename, "SAVGAME.%03d", i);
      inf.open(filename, ios::in);
      inf.read((char*)&P, sizeof(Pilot));
      inf.close();
      P.ID--;
      sprintf(filename, "SAVGAME.%03d", P.ID);
      outf.open(filename, ios::out);
      outf.write((char*)&P, sizeof(Pilot));
      outf.close();
   }

   sprintf(filename, "SAVGAME.%03d", num_saved);    // remove last savegame file
	remove(filename);
}

char PromptDel(void)
// prompt user for deletion of savegame
{
	char *choice;   // user choice
	char ch;        // 1st char of user choice

	G.fade_out();
	B.RemoveButton("<<");       // remove buttons
	B.RemoveButton(">>");
	B.RemoveButton("CANCEL");
	B.RemoveButton("OK");
	B.RemoveButton("DELETE");
	B.AddButton(120, 100, 150, 115, GREEN, "Yes", "Yes");   // add y/n buttons
	B.AddButton(170, 100, 200, 115, GREEN, "No", "No");
	G.clearviewport();

	G.fade_to(100);
	G.write(10, 50, 55, LIGHTCYAN, "Are you sure you want to delete?", 30);     // prompt animation
	B.ShowAllButtons();
	choice = B.TrapClick(LEFT_BUTTON);  // get a choice
	G.fade_out();

	ch = choice[0];   // get 1st char of choice

	B.RemoveButton("Yes");  // remove y/n buttons
	B.RemoveButton("No");


	G.clearviewport();  // clear screen

	G.fade_to(100);  // re-output previous prompt message
	G.write(10, 15, 55, LIGHTCYAN, "Select a game to load:", 30);

	B.AddButton(120, 100, 155, 115, CYAN, "<<", "<<");          // re-add all previous buttons
	B.AddButton(165, 100, 200, 115, CYAN, ">>", ">>");
	B.AddButton(120, 120, 155, 135, CYAN, "CANCEL", "CANCEL");
	B.AddButton(165, 120, 200, 135, CYAN, "OK", "OK");
	B.AddButton(135, 140, 185, 155, RED, "DELETE", "DELETE");
	B.ShowAllButtons();

	return ch;   // return 1st char of user choice.
}
