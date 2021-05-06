#include <fstream.h>    // files
#include <alloc.h>      // memory allocation
#include <stdlib.h>     // exit()
#include <math.h>       // math functions

#include "PGphV120.H"    // include graphics library that i wrote
#include "PxmsV111.H"    // include XMS library that i modified
extern "C"
{
	#include "detect.h"         // ethan's sound
	#include "smix.h"
}

#include "Aa4Snds.H"     // header for sounds, with constants

#include "Aa4Level.H"    // include all header files used (this file contains all the friend functions).
#include "Aa4Good.H"
#include "Aa4EProj.H"
#include "Aa4YProj.H"
#include "Aa4EShip.H"
#include "Aa4YShip.H"
#include "Aa4GDat.H"
#include "Aa4Game.H"
#include "Aa4Exps.H"


const float DEG_RAD = 3.14159/180.0;
const float PI = 3.14159;
const float pi_2 = PI/2.0;


extern GraphObject G;    // external graphics object... in main module
extern XMSObject X;      // external xms object
extern GameData GDat;    // external game data
extern char quit;        // quit flag
extern char EscFlag,     // keyboard flags
				CtrlFlag,
            AltFlag,
				LeftFlag,
				RightFlag,
				UpFlag,
				DownFlag,
				PFlag;
extern SOUND *Sound[];     // in aa4snds.cpp
extern char SFX, Music;     // flags for sound
extern long row;    // row of level
extern Explosions XP;  // explosions
extern char dead;
extern long money;   // money you've got
extern char e_fin;   // finished with enemies
extern char lev_fin;  // finished with level scrolling
extern int mem_size;  // temp mem counter
extern int brightness;  // brightness of screen in game
extern VirtualScreen Vscr_G;  // extern vscr


inline void drop(int &a)    // decrement value of a until a = 0.
{
	a--;
	if (a < 0) a = 0;
}

inline float CalcAngle(float x1, float y1, float x2, float y2)
// calculate angle from (x1, y1) to (x2, y2).
{
	if (x1 == x2)
		if (y1 > y2) return pi_2;
		else return -pi_2;
	else if (x1 < x2)
		return -atan( (y2-y1)/(x2-x1) );
	else
		return -atan( (y2-y1)/(x2-x1) ) + PI;
}


void LoadLevel(Level &L, EShips &E, EProjs &EP, int level_number)
// Pre: All files exist.
// Post: Level is loaded into classes and XMS.
{
	ifstream IniFile("Aa4.INI");     // .ini file
	char filename[80];              // used to extract filenames from .ini and level files.
	int i, j;                        // counters
	int num_rows;                    // number of rows in level
	int isize = G.imagesize(0, 0, 15, 15);     // size of a tile.
	void *img;                       // image loaded from file.  it is put here as temporary; it moves immediately to XMS

	while (IniFile.peek() != '=')     IniFile.ignore();  // GAME DAT=
	IniFile.ignore();
	IniFile.getline(filename, 79);     // GAME DATA FILE=****

	while (IniFile.peek() != '=')     IniFile.ignore();   // NUM LEVELS=
	IniFile.ignore();
	IniFile >> GDat.max_level;
	IniFile.getline(filename, 79);      // clear line

	for (i = 0; i < level_number; i++)    // cut all lines except the one containing the correct level filename
		IniFile.getline(filename, 79);

   IniFile.close();



	ifstream LevFile(filename);          // Level file.

//    ;music description block   
   while (LevFile.peek() != '=')  LevFile.ignore();    // TRACK=
   LevFile.ignore();
   LevFile >> L.track;   // get the number of different scenery tiles

	if (Music)                               // play the track
   {
   	stop_sound(SHOP_SOUND);
      start_sound(Sound[L.track], L.track, ON);
	}

//    ;level description block
   while (LevFile.peek() != '=')  LevFile.ignore();   // LINES=
	LevFile.ignore();

   LevFile >> L.ld_lines;    // get the number of lines in the level description.
   LevFile.ignore();
   for (i = 0; i < L.ld_lines; i++)               // get the level description
   	LevFile.getline(L.lev_descrip[i], 60);

   while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF SCENERY TILES=
   LevFile.ignore();

   LevFile >> L.num_diff_scen_tiles;   // get the number of different scenery tiles

   while (LevFile.peek() != '=')  LevFile.ignore();    // LEVEL GRAPHICS FILE=
   LevFile.ignore();

	LevFile.getline(filename, 79);   // get level graphics filename
	ifstream Gfile(filename);         // graphics file


	img = malloc(isize);       // allocate memory for image

	if (img == NULL)          // if not successful
	{
		cerr << "out of memory!";
		exit(1);    // quit
	}

	L.land_offset = GDat.next_offst;

	for (i = 0; i < L.num_diff_scen_tiles; i++)    // load in all the tiles
	{
		Gfile.read((char*)img, isize);      // get the image
		if (X.CopyToEMB(img, isize, GDat.xms_pic_handle, L.land_offset+isize*i) == 0)   // copy the image to memory
		{
			cerr << "XMS copy-to error";
			exit(1);   // quit
		}
	}

	free(img);   // free memory allocated for image

   GDat.next_offst += isize*L.num_diff_scen_tiles;

   while (LevFile.peek() != '=')  LevFile.ignore();    // LEVEL MODULE FILE=
   LevFile.ignore();
   LevFile.getline(filename, 79);   // get level module filename
   ifstream ModFile(filename);         // module file

   ParseModFile(ModFile, Gfile, EP, E);

   ModFile.close();

//----------------------------------------------------
//              input level strips

	while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF SCENERY STRIPS=
	LevFile.ignore();

	LevFile >> L.num_diff_lev_strips;   // get the number of different level strips

	lev_node_ptr ins;                  // insertion ptr for linked list

	L.lev_strip_defs = new lev_node;            // create circular-doubly-linked-list
	L.lev_strip_defs->next = L.lev_strip_defs;
	L.lev_strip_defs->prev = L.lev_strip_defs;

	L.destroyed = 0;       // set flag to zero; this flag represents the status of the level object
	L.img = malloc(isize);    // allocate enough memory for the temp tile (stored in level object)

	for (i = 0; i < L.num_diff_lev_strips; i++)      // insert all level strips into linked list
	{
		ins = new lev_node;
		for (j = 0; j < 20; j++)          // put the data in each node
			LevFile >> ins->tiles[j];
		ins->prev = L.lev_strip_defs->prev;    // now we get to link them all up!  :)
		ins->next = L.lev_strip_defs;
		ins->prev->next = ins;
		ins->next->prev = ins;
	}

//--------------------------------------------------
//                end level strip input

//--------------------------------------------------
//                load scenery list block

	while (LevFile.peek() != '=')  LevFile.ignore();   // NUMBER OF ROWS=
	LevFile.ignore();

	LevFile >> num_rows;
	L.lev_rows = num_rows*16;

	for (i = 0; i < num_rows; i++)
		LevFile >> L.lev_strips[i];

//--------------------------------------------------
//                end scenery list block


//       ;enemy list block
	int num_enemies;
   enemy_node_ptr et_ins;                  // insertion ptr for enemy types

	while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF ENEMIES=
	LevFile.ignore();
	LevFile >> num_enemies;

	en_ptr e_ins;

   E.head = new eship_node;
   E.head->next = E.head->prev = E.head;  

   for (i = 0; i < num_enemies; i++)
   {
   	e_ins = new eship_node;

		while (LevFile.peek() != '=')  LevFile.ignore();    // TYPE=
		LevFile.ignore();
		LevFile >> e_ins->type;

      et_ins = E.enemy_types->next;
      for (j = 0; j < e_ins->type; j++)
      	et_ins = et_ins->next;
      e_ins->hitpoints = et_ins->hitpoints;

		while (LevFile.peek() != '=')  LevFile.ignore();    // POSITION=
		LevFile.ignore();
		LevFile >> (e_ins->x) >> (e_ins->row);
		e_ins->y = -1000;
      e_ins->next_move = 0;
      e_ins->next_shoot = 0;

      while (LevFile.peek() != '=')  LevFile.ignore();    // PATH=
		LevFile.ignore();
		LevFile >> e_ins->path_type;
		e_ins->stage_in_path = 0;
		e_ins->ref_x = e_ins->x;
      e_ins->ref_y = 0;

		while (LevFile.peek() != '=')  LevFile.ignore();    // CARRIES=
		LevFile.ignore();
		LevFile >> e_ins->carries;

		e_ins->prev = E.head->prev;    // now we get to link them all up!  :)
		e_ins->next = E.head;
		e_ins->prev->next = e_ins;
		e_ins->next->prev = e_ins;
	}

	Gfile.close();     // close files
	LevFile.close();
}

void Destroy(EShips &E, YProjs &YP, EProjs &EP, Goodies &Gd, Level &L)
// Pre: All classes fully initialized.
// Post: All class memory deallocated.
{
	if (!L.destroyed)     // if not destroyed yet..
   {
		lev_node_ptr Ldel;    // deletion ptr

   	while (L.lev_strip_defs->next != L.lev_strip_defs)     // delete linked list
	   {
	   	Ldel = L.lev_strip_defs->next;
	      L.lev_strip_defs->next = Ldel->next;
	      delete Ldel;
	   }
	   delete L.lev_strip_defs;

	   free(L.img);          // free temporary image

      L.destroyed = 1;     // set destroyed flag
   }

	yproj_node_ptr YPdel;               // delete memory for yprojectiles
	while (YP.head->next != YP.head)
	{
		YPdel = YP.head->next;
		YP.head->next = YPdel->next;
		delete YPdel;
	}
	delete YP.head;

	eproj_node_ptr EPdel;               // delete all memory for eprojectiles
	while (EP.head->next != EP.head)
	{
		EPdel = EP.head->next;
		EP.head->next = EPdel->next;
		delete EPdel;
	}
	delete EP.head;

	etn_ptr etn_del;                // delete all memory for enemy stuff
	while (EP.eproj_types->next != EP.eproj_types)
	{
		etn_del = EP.eproj_types->next;
		EP.eproj_types->next = etn_del->next;
		delete etn_del;
	}
	delete EP.eproj_types;

	en_ptr e_del;
	while (E.head->next != E.head)
	{
		e_del = E.head->next;
		E.head->next = e_del->next;
		delete e_del;
	}
	delete E.head;

	path_node_ptr p_del;                 // delete enemy paths
	coord_node_ptr c_del;
	while (E.paths->next != E.paths)
	{
		p_del = E.paths->next;
		E.paths->next = p_del->next;
		while (p_del->coords_head->next != p_del->coords_head)
		{
			c_del = p_del->coords_head->next;
			p_del->coords_head->next = c_del->next;
			delete c_del;
		}
		delete p_del->coords_head;
		delete p_del;
	}
	delete E.paths;

	enemy_node_ptr et_del;                   // delete enemy descriptions
	wsn_ptr w_del;
	while (E.enemy_types->next != E.enemy_types)
	{
		et_del = E.enemy_types->next;
		E.enemy_types->next = et_del->next;
		while (et_del->ws_head->next != et_del->ws_head)
		{
			w_del = et_del->ws_head->next;
			et_del->ws_head->next = w_del->next;
			delete w_del;
		}
		delete et_del->ws_head;
		delete et_del;
	}
	delete E.enemy_types;

	good_node_ptr g_del;                // delete goodies
	while (Gd.head->next != Gd.head)
	{
		g_del = Gd.head->next;
		Gd.head->next = g_del->next;
		delete g_del;
	}
	delete Gd.head;
}

void CheckGd(Goodies &Gd, YShip &Y, Pilot &P)
// Pre: None.
// Post: Checked if you collided with a goody.
{
	good_node_ptr g_cur, g_del;

	g_cur = Gd.head->next;               // loop thru list
	while (g_cur != Gd.head)
	{
		if ( abs(g_cur->x - Y.x) <= (GDat.ys_xdim + GDat.ygoodies[g_cur->type].xdim)/2 &&
			  abs(g_cur->y - Y.y) <= (GDat.ys_ydim + GDat.ygoodies[g_cur->type].ydim)/2 )   // check for collision
		{
			if (SFX)
			{
				if (sound_playing(6))    // wheeeeeeee! noise
					stop_sound(6);
				start_sound(Sound[6], 6, OFF);
			}
			XP.Spark(g_cur->x, g_cur->y, 30);   // put up a happy spark

			money += GDat.ygoodies[g_cur->type].cash;              // make goody do something
			Y.shields += GDat.ygoodies[g_cur->type].hitpoints;
			if (Y.shields > 100) Y.shields = 100;
			if (GDat.ygoodies[g_cur->type].weapon >= 0)
				P.weapon_list[GDat.ygoodies[g_cur->type].weapon]++;

			g_del = g_cur;             // delete goody from list
			g_cur = g_cur->prev;
			g_cur->next = g_del->next;
			g_cur->next->prev = g_cur;
			delete g_del;
		}
		g_cur = g_cur->next;    // go on to next goody
	}
}

void InitProjs(YProjs &YP, EProjs &EP)
// Pre: None.
// Post: Initializes projectiles.
{
	YP.head = new yproj_node;
   YP.head->prev = YP.head->next = YP.head;

   EP.head = new eproj_node;
   EP.head->prev = EP.head->next = EP.head;
}

void CheckKybd(YShip &Y, YProjs &YP, EShips &E, Pilot &P)
// Check keyboard for input
{
	int i, j;
	char done;
	en_ptr target;
	char hit_head;

	if (EscFlag)
		quit = 1;

	drop(Y.next_shoot[0]);
	drop(Y.next_shoot[1]);
	drop(Y.next_shoot[2]);
	drop(Y.next_acc);
   drop(Y.next_wswitch);
   drop(Y.next_power_up);
   drop(Y.next_shield_up);
   drop(Y.next_move);

   if (Y.next_acc > 0) return;

	if (LeftFlag && !RightFlag)     // if moving left
   {
   	Y.xmove -= Y.acc;                    // move
      if (Y.xmove < -Y.velocity)              // check limit
			Y.xmove += Y.acc;
   }
   else if (RightFlag && !LeftFlag)    // etc etc
   {
   	Y.xmove += Y.acc;
		if (Y.xmove > Y.velocity)
      	Y.xmove -= Y.acc;
   }
   else
   	if (Y.xmove < 0)
      	Y.xmove += Y.acc;
      else if (Y.xmove > 0)
      	Y.xmove -= Y.acc;

   if (UpFlag && !DownFlag)
   {
   	Y.ymove -= Y.acc;
		if (Y.ymove < -Y.velocity)
      	Y.ymove += Y.acc;
   }
   else if (DownFlag && !UpFlag)
   {
   	Y.ymove += Y.acc;
      if (Y.ymove > Y.velocity)
      	Y.ymove -= Y.acc;
   }
	else
   	if (Y.ymove > 0)
      	Y.ymove -= Y.acc;
		else if (Y.ymove < 0)
      	Y.ymove += Y.acc;

	yproj_node_ptr ins;       // insertion pointer for yprojectiles

	if (CtrlFlag && Y.next_shoot[0] == 0 && Y.power >= GDat.yweapons[P.primary].power)  // try to shoot primary
	{
		Y.next_shoot[0] = GDat.yweapons[P.primary].refire;
		Y.power -= GDat.yweapons[P.primary].power;
		if (SFX)                                   // make sound
		{
			if (sound_playing(GDat.yweapons[P.primary].sound_num))
				stop_sound(GDat.yweapons[P.primary].sound_num);
			start_sound(Sound[GDat.yweapons[P.primary].sound_num], GDat.yweapons[P.primary].sound_num, OFF);
		}
		for (i = 0; i < GDat.yweapons[P.primary].num_projs_shot; i++)   // each subsystem of weapon fires
		{
			ins = new yproj_node;
			ins->x = Y.x + GDat.yweapons[P.primary].x_off[i];
			ins->y = Y.y + GDat.yweapons[P.primary].y_off[i];
			ins->type = GDat.yweapons[P.primary].proj[i];

			if (GDat.yweapons[P.primary].theta[i] > 1000)                   // compute special tracking if weapon has a auto-tracking system.
				if (E.head->next != E.head && E.head->next->y != -1000)
				{
					target = E.head->next;
					hit_head = 0;
					for (j = 0; j < Y.target_num; j++)
					{
						target = target->next;
						while (target == E.head || target->y == -1000)
						{
							if (target == E.head)
								hit_head = 1;
							target = target->next;
						}
					}
					if (hit_head)
						Y.target_num = 0;
					ins->theta = CalcAngle(Y.x, Y.y, target->x, target->y);
					Y.target_num++;
				}
				else
					ins->theta = pi_2;
			else
				ins->theta = GDat.yweapons[P.primary].theta[i] * DEG_RAD;

			ins->xmove = cos(ins->theta);      // set up values in the linked list
			ins->ymove = sin(ins->theta);
			ins->next_move = 0;
			ins->next = YP.head->next;
			ins->prev = YP.head;
			ins->next->prev = ins;
			ins->prev->next = ins;
		}
	}
	if (P.secondary != -1)   // if you own a secondary weapon
	if (CtrlFlag && Y.next_shoot[1] == 0 && Y.power >= GDat.yweapons[P.secondary].power)  // fire it
	{
		Y.next_shoot[1] = GDat.yweapons[P.secondary].refire;      // this entire segment is the same as above.
		Y.power -= GDat.yweapons[P.secondary].power;
		if (SFX)
		{
			if (sound_playing(GDat.yweapons[P.secondary].sound_num))
         	stop_sound(GDat.yweapons[P.secondary].sound_num);
	      start_sound(Sound[GDat.yweapons[P.secondary].sound_num], GDat.yweapons[P.secondary].sound_num, OFF);
		}
      for (i = 0; i < GDat.yweapons[P.secondary].num_projs_shot; i++)
      {
			ins = new yproj_node;
			ins->x = Y.x + GDat.yweapons[P.secondary].x_off[i];
			ins->y = Y.y + GDat.yweapons[P.secondary].y_off[i];
			ins->type = GDat.yweapons[P.secondary].proj[i];

			if (GDat.yweapons[P.secondary].theta[i] > 1000)
				if (E.head->next != E.head && E.head->next->y != -1000)
				{
					target = E.head->next;
               hit_head = 0;
					for (j = 0; j < Y.target_num; j++)
					{
						target = target->next;
						while (target == E.head || target->y == -1000)
                  {
                  	if (target == E.head)
                     	hit_head = 1;
							target = target->next;
                  }
					}
               if (hit_head)
               	Y.target_num = 0;
					ins->theta = CalcAngle(Y.x, Y.y, target->x, target->y);
               Y.target_num++;
				}
				else
					ins->theta = pi_2;
			else
				ins->theta = GDat.yweapons[P.secondary].theta[i] * DEG_RAD;

			ins->xmove = cos(ins->theta);
			ins->ymove = sin(ins->theta);
			ins->next_move = 0;
			ins->next = YP.head->next;
			ins->prev = YP.head;
			ins->next->prev = ins;
			ins->prev->next = ins;
		}
	}

	if (AltFlag && Y.next_wswitch <= 0 && P.secondary >= 0)   // check for switching weapons
	{
		Y.next_wswitch = WEAPON_SWITCH_DELAY;      // update counter for next weapon-switch.

		done = 0;                                         // cycle forward in list.
		for (i = 0; i < NUM_WEAPONS && !done; i++)
			if (P.weapon_list[(i+P.secondary+1)%NUM_WEAPONS] > 0 &&
				 !GDat.yweapons[(i+P.secondary+1)%NUM_WEAPONS].primary)
			{
				P.secondary = (i+P.secondary+1)%NUM_WEAPONS;
				done = 1;
			}
	}

	if (PFlag)
	{
		G.fade_out();
		G.setcolor(10);
      G.clearviewport();
		G.settextjustify(CENTER_TEXT, CENTER_TEXT);
		G.outtextxy(160, 100, "Press ESC to resume...");
		G.fade_in();
#pragma warn -eff
		while (!EscFlag) ;
		while (EscFlag) ;
#pragma warn .eff
		G.fade_out();
		brightness = 0;
	}
}

void CheckEP(EProjs &EP, YShip &Y)
// Pre: None.
// Post: Checks if you have been hit.
{
	eproj_node_ptr ep_cur, ep_del;             // traversal and deletion ptrs
	etn_ptr eproj_type;                        // eproj type pointer
	int i, j;
	int x_range, y_range;                      // range of explosions

	ep_cur = EP.head->next;                    // traverse list of enemy projectiles
	while (ep_cur != EP.head)
	{
		eproj_type = EP.eproj_types->next;      // move to the eproj type
		for (i = 0; i < ep_cur->type; i++)
			eproj_type = eproj_type->next;

		if ( abs(ep_cur->x - Y.x) <= (GDat.ys_xdim + eproj_type->xdim)/2 &&    // check for collision
			  abs(ep_cur->y - Y.y) <= (GDat.ys_ydim + eproj_type->ydim)/2 )
		{
			Y.shields -= eproj_type->hitpoints;

			if (SFX)
			{
				if (sound_playing(5))    // ka-booiiiing noise
					stop_sound(5);
				start_sound(Sound[5], 5, OFF);
			}
			XP.Spark(ep_cur->x, ep_cur->y, 10);    // spark

			ep_del = ep_cur;                       // delete enemy projectile
			ep_cur = ep_cur->prev;
			ep_cur->next = ep_del->next;
			ep_cur->next->prev = ep_cur;
			delete ep_del;

			if (Y.shields <= 0)     // if you are dead
			{
				x_range = floor((GDat.ys_xdim/2.0) / XP.xdim);   // compute range of explosions
				y_range = floor((GDat.ys_ydim/2.0) / XP.ydim);

				if (SFX)                                 // make explosion noise
				{
					if (sound_playing(2))
						stop_sound(2);
					start_sound(Sound[2], 2, OFF);
				}
				for (i = -x_range; i <= x_range; i++)               // draw all explosions in the range
					for (j = -y_range; j <= y_range; j++)
						XP.Explode(Y.x + i*XP.xdim, Y.y + j*XP.ydim);
				dead = 1;
			}
		}
		ep_cur = ep_cur->next;  // move on to next eproj
	}
}

void CheckYP(YProjs &YP, EShips &E, Goodies &Gd)
// Pre: None.
// Post: Checks if you have shot down something.
{
	yproj_node_ptr y_cur, y_del;        // the logic of this entire section parallels that
	en_ptr e_cur, e_del;                //  of the prior section, except that last time
	enemy_node_ptr e_type;              //  there was only one target, and this time there
	int i, j;                           //  are many.
	char breakout;
	int x_range, y_range;

	y_cur = YP.head->next;
	while (y_cur != YP.head)
	{
		e_cur = E.head->next;
		breakout = 0;
		while (e_cur != E.head && !breakout)
		{
      	if (e_cur->y == -1000)
         {
         	e_cur = e_cur->next;
            continue;
         }

			e_type = E.enemy_types->next;
			for (i = 0; i < e_cur->type; i++)
				e_type = e_type->next;

         if ( abs(y_cur->x - e_cur->x) <= (e_type->xdim + GDat.yprojectiles[y_cur->type].xdim)/2 &&
              abs(y_cur->y - e_cur->y) <= (e_type->ydim + GDat.yprojectiles[y_cur->type].ydim)/2 )
         {
 				e_cur->hitpoints -= GDat.yprojectiles[y_cur->type].hitpoints;

            if (SFX)
            {
					if (sound_playing(5))    // ka-booiiiing noise
               	stop_sound(5);
               start_sound(Sound[5], 5, OFF);
				}
            XP.Spark(y_cur->x, y_cur->y, 10); 

            y_del = y_cur;
            y_cur = y_cur->prev;
            y_cur->next = y_del->next;
            y_cur->next->prev = y_cur;
            delete y_del;

            breakout = 1;

            if (e_cur->hitpoints <= 0)
            {
            	x_range = floor((e_type->xdim/2.0) / XP.xdim);
               y_range = floor((e_type->ydim/2.0) / XP.ydim);

					if (SFX)
               {
               	if (sound_playing(2))
							stop_sound(2);
                  start_sound(Sound[2], 2, OFF);
               }
					money += e_type->money;
               if (e_cur->carries != -1)
			         Gd.AddGoody(e_cur->x, e_cur->y, e_cur->carries);
               for (i = -x_range; i <= x_range; i++)
               	for (j = -y_range; j <= y_range; j++)
                  	XP.Explode(e_cur->x + i*XP.xdim, e_cur->y + j*XP.ydim);

            	e_del = e_cur;
               e_cur = e_cur->prev;
               e_cur->next = e_del->next;
               e_cur->next->prev = e_cur;
            	delete e_del;
            }
         }
         e_cur = e_cur->next;
      }
		y_cur = y_cur->next;
	}
}

void CheckE(EShips &E, YShip &Y, Goodies &Gd)
// Pre: None.
// Post: Checks if you have collided with an enemy ship
{
	en_ptr e_cur, e_del;            // again, this is similar to the prior section,
	enemy_node_ptr e_type;          //  except that enemy ship collisions are checked.
	int i, j;
	int x_range, y_range;

	e_cur = E.head->next;
	while (e_cur != E.head)
	{
   	if (e_cur->y == -1000)
      {
      	e_cur = e_cur->next;
         continue;
      }
		e_type = E.enemy_types->next;
		for (i = 0; i < e_cur->type; i++)
			e_type = e_type->next;

		if ( abs(e_cur->x - Y.x) <= (GDat.ys_xdim + e_type->xdim)/2 &&
			  abs(e_cur->y - Y.y) <= (GDat.ys_ydim + e_type->ydim)/2 &&
			  e_type->speed > -1)
		{
			Y.shields -= e_cur->hitpoints;

			if (SFX)
         {
         	if (sound_playing(5))    // ka-booiiiing noise
            	stop_sound(5);
            start_sound(Sound[5], 5, OFF);
         }
			XP.Spark(e_cur->x, e_cur->y, 10);

         x_range = floor((e_type->xdim/2.0) / XP.xdim);
         y_range = floor((e_type->ydim/2.0) / XP.ydim);
         if (SFX)
         {
         	if (sound_playing(2))
            	stop_sound(2);
            start_sound(Sound[2], 2, OFF);
         }
         money += e_type->money;
         if (e_cur->carries != -1)
	         Gd.AddGoody(e_cur->x, e_cur->y, e_cur->carries);
         for (i = -x_range; i <= x_range; i++)
         	for (j = -y_range; j <= y_range; j++)
            	XP.Explode(e_cur->x + i*XP.xdim, e_cur->y + j*XP.ydim);

         e_del = e_cur;
         e_cur = e_cur->prev;
         e_cur->next = e_del->next;
         e_cur->next->prev = e_cur;
         delete e_del;

         if (Y.shields <= 0)
         {
         	x_range = floor((GDat.ys_xdim/2.0) / XP.xdim);
            y_range = floor((GDat.ys_ydim/2.0) / XP.ydim);

            if (SFX)
            {
            	if (sound_playing(2))
               	stop_sound(2);
               start_sound(Sound[2], 2, OFF);
            }
            for (i = -x_range; i <= x_range; i++)
            	for (j = -y_range; j <= y_range; j++)
               	XP.Explode(Y.x + i*XP.xdim, Y.y + j*XP.ydim);
            dead = 1;
         }
      }
      e_cur = e_cur->next;
   }
}

void Animate(EShips &E, EProjs &EP, YShip &Y)
// Pre: eships init.
// Post: eships animated.
{
	en_ptr cur, del;            // traversal and deletion ptrs for eships
	enemy_node_ptr ship_type;   // ship type
	wsn_ptr weapon_type;        // weapon type
	eproj_node_ptr ep_ins;      // eproj insertion
	path_node_ptr p_cur;        // path node ptr
	coord_node_ptr c_cur;       // coords of next waypoint
	void *img;                  // image
	int i;
	float dest_x, dest_y;       // destination coords
	float angle;                // angle to move on in order to get to next waypoint

	cur = E.head->next;         // traverse list of eships
	while (cur != E.head)
	{
		ship_type = E.enemy_types->next;   // get the ship type
		for (i = 0; i < cur->type; i++)
			ship_type = ship_type->next;

		if (cur->row > row+200+ship_type->ydim)    // if the ship is too far away as yet
		{
			cur = cur->next;
			continue;
		}
		if (cur->y > 200+ship_type->ydim || cur->x < -ship_type->xdim || cur->x > 320+ship_type->xdim)  // if the ship has fallen off the screen
		{
			del = cur;
			cur = cur->prev;
			cur->next = del->next;
			cur->next->prev = cur;
			delete del;
			cur = cur->next;
			continue;
		}
		if (cur->y == -1000)    // if the y-coord has not been initialized yet
			cur->y = (row+200) - cur->row;

		if (cur->next_move > 0)  // if it is not time to move yet
		{
			cur->next_move--;
			cur = cur->next;
			continue;
		}

		p_cur = E.paths->next;               // get the path
		for (i = 0; i < cur->path_type; i++)
			p_cur = p_cur->next;

		c_cur = p_cur->coords_head->next;    // get the coords of the next waypoint

		if (ship_type->speed == -1)          // if ship doesn't move..
			if (!lev_fin)
				cur->y += SPEED;     // just move it at the same speed as the ground.
			else ;
      else if (ship_type->speed < 0)
      {
      	if (!lev_fin)
				cur->y += SPEED;     // just move it at the same speed as the ground.
			else ;
         cur->x += ship_type->speed;
      }
		else if (cur->stage_in_path < p_cur->num_points)    // if the ship has exhausted the path
		{
			for (i = 0; i < cur->stage_in_path; i++)    // get to the coords of the next waypoint
				c_cur = c_cur->next;

			dest_x = cur->ref_x + c_cur->x;   // compute the destination
			dest_y = cur->ref_y + c_cur->y;

			if (dest_x > cur->x)                                        // compute the angle
				angle = atan( (dest_y-cur->y) / (dest_x-cur->x) );
			else if (dest_x < cur->x)
				angle = atan( (dest_y-cur->y) / (dest_x-cur->x) ) + PI;
			else
				if (dest_y > cur->y)
					angle = PI/2.0;
				else
					angle = -PI/2.0;

			cur->y += ship_type->speed * sin(angle);      // move the ship on
			cur->x += ship_type->speed * cos(angle);

			if ( abs(cur->y - dest_y) < ship_type->speed &&    // if the ship has reached the waypoint
				  abs(cur->x - dest_x) < ship_type->speed)
				cur->stage_in_path++;
		}

      mem_size = (ship_type->isize+1)/2 * 2;
		img = malloc(mem_size);            // get mem

		if (X.CopyFromEMB(GDat.xms_pic_handle, ship_type->offset, ship_type->isize, img) == 0)   // AND image
		{
			cerr << "XMS copy-from error (AND image)";
			exit(1);
		}
		G.putimage(cur->x-ship_type->xdim/2, cur->y-ship_type->ydim/2, img, AND_PUT, Vscr_G);    // put image

		if (X.CopyFromEMB(GDat.xms_pic_handle, ship_type->offset+mem_size, ship_type->isize, img) == 0)   // XOR image
		{
			cerr << "XMS copy-from error (XOR image)";
			exit(1);
		}
		G.putimage(cur->x-ship_type->xdim/2, cur->y-ship_type->ydim/2, img, XOR_PUT, Vscr_G);    // put image

		free(img);    // free mem

		if (cur->next_shoot <= 0 && ship_type->num_projs_fired > 0)  // if time to shoot, then shoot
		{
			cur->next_shoot = ship_type->refire;      // update counter for next shot
			if (SFX)    // make firing sound
			{
				if (sound_playing(ship_type->sound_num))
					stop_sound(ship_type->sound_num);
				start_sound(Sound[ship_type->sound_num], ship_type->sound_num, OFF);
			}

			weapon_type = ship_type->ws_head->next;   // figure out which weapons the eship fires
			for (i = 0; i < ship_type->num_projs_fired; i++)
			{
				ep_ins = new eproj_node;     // insert a new eproj
				ep_ins->type = weapon_type->type;
				ep_ins->x = cur->x + weapon_type->x_off;
				ep_ins->y = cur->y + weapon_type->y_off;
				if (weapon_type->theta > 1000)
					ep_ins->theta = CalcAngle(cur->x, cur->y, Y.x, Y.y);
				else
					ep_ins->theta = weapon_type->theta * DEG_RAD;
				ep_ins->ymove = sin(ep_ins->theta);
				ep_ins->xmove = cos(ep_ins->theta);
				ep_ins->next_move = 0;

				ep_ins->prev = EP.head->prev;
				ep_ins->next = EP.head;
				ep_ins->prev->next = ep_ins;
				ep_ins->next->prev = ep_ins;

				weapon_type = weapon_type->next;
			}
		}
		else
			cur->next_shoot--;   // one time interval has passed

		cur = cur->next;   // move on
	}

	if (E.head->next == E.head)  // if no enemies left, then the enemies are finished.
		e_fin = 1;
}

void ParseModFile(ifstream &LevFile, ifstream &Gfile, EProjs &EP, EShips &E)
// parse the .MOD file to load enemy information
{
	int explode;                     // explode flag
	void *img;
	int i, j;

   // this is the same kind of documentation as the load-level function

	//   ;projectile description block
	while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF PROJECTILES=
	LevFile.ignore();
	LevFile >> EP.num_types;   // get the number of different eprojs

	etn_ptr ep_ins;                  // insertion ptr for enemy proj types

	EP.eproj_types = new eproj_type_node;            // create circular-doubly-linked-list
	EP.eproj_types->next = EP.eproj_types;
	EP.eproj_types->prev = EP.eproj_types;

	for (i = 0; i < EP.num_types; i++)      // insert all eprojs into linked list
	{
		ep_ins = new eproj_type_node;

		while (LevFile.peek() != '=')  LevFile.ignore();    // XDIM=
		LevFile.ignore();
		LevFile >> ep_ins->xdim;

		while (LevFile.peek() != '=')  LevFile.ignore();    // YDIM=
		LevFile.ignore();
		LevFile >> ep_ins->ydim;

		ep_ins->isize = G.imagesize(1, 1, ep_ins->xdim, ep_ins->ydim);
      mem_size = (ep_ins->isize+1)/2 * 2;
		img = malloc(mem_size);
		Gfile.read((char*)img, ep_ins->isize);      // AND mask
		ep_ins->offset = GDat.next_offst;
		GDat.next_offst += mem_size;
		if (X.CopyToEMB(img, ep_ins->isize, GDat.xms_pic_handle, ep_ins->offset) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying eprojectile AND image)";
			exit(1);
		}
		Gfile.read((char*)img, ep_ins->isize);      // XOR mask
		GDat.next_offst += mem_size;
		if (X.CopyToEMB(img, ep_ins->isize, GDat.xms_pic_handle, ep_ins->offset+mem_size) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying eprojectile XOR image)";
			exit(1);
		}
		free(img);

		while (LevFile.peek() != '=')  LevFile.ignore();    // SPEED=
		LevFile.ignore();
		LevFile >> ep_ins->speed;

		while (LevFile.peek() != '=')  LevFile.ignore();    // EXPLODE=
		LevFile.ignore();
		LevFile >> explode;

		while (LevFile.peek() != '=')  LevFile.ignore();    // EXPLOSION RADIUS=
		LevFile.ignore();
		LevFile >> ep_ins->radius;
		if (!explode)
			ep_ins->radius = 0;

		while (LevFile.peek() != '=')  LevFile.ignore();    // HITPOINTS=
		LevFile.ignore();
		LevFile >> ep_ins->hitpoints;

		ep_ins->prev = EP.eproj_types->prev;    // now we get to link them all up!  :)
		ep_ins->next = EP.eproj_types;
		ep_ins->prev->next = ep_ins;
		ep_ins->next->prev = ep_ins;
	}

//      ;path description block
   while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF PATHS=
	LevFile.ignore();
	LevFile >> E.num_paths;

	path_node_ptr p_ins;                  // insertion ptr for enemy path types
	coord_node_ptr c_ins;                 // insertion ptr for enemy coordinates

	E.paths = new path_node;            // create circular-doubly-linked-list
	E.paths->next = E.paths;
	E.paths->prev = E.paths;

	for (i = 0; i < E.num_paths; i++)      // insert all eprojs into linked list
	{
		p_ins = new path_node;

		while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF POINTS=
		LevFile.ignore();
		LevFile >> p_ins->num_points;

		p_ins->coords_head = new coord_node;
		p_ins->coords_head->prev = p_ins->coords_head->next = p_ins->coords_head;

		for (j = 0; j < p_ins->num_points; j++)
		{
			c_ins = new coord_node;

			while (LevFile.peek() != '=')  LevFile.ignore();    // COORDINATES=
			LevFile.ignore();
			LevFile >> (c_ins->x) >> (c_ins->y);

			c_ins->prev = p_ins->coords_head->prev;    // now we get to link them all up!  :)
			c_ins->next = p_ins->coords_head;
			c_ins->prev->next = c_ins;
			c_ins->next->prev = c_ins;
		}

		p_ins->prev = E.paths->prev;    // now we get to link them all up!  :)
		p_ins->next = E.paths;
		p_ins->prev->next = p_ins;
		p_ins->next->prev = p_ins;
	}

//       ;enemy description block
	while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF ENEMIES=
	LevFile.ignore();
	LevFile >> E.num_enemy_types;   // get the number of different enemy types

	enemy_node_ptr et_ins;                  // insertion ptr for enemy types
	wsn_ptr w_ins;

	E.enemy_types = new enemy_node;            // create circular-doubly-linked-list
	E.enemy_types->next = E.enemy_types;
	E.enemy_types->prev = E.enemy_types;

	for (i = 0; i < E.num_enemy_types; i++)      // insert all enemy types into linked list
	{
		et_ins = new enemy_node;

		while (LevFile.peek() != '=')  LevFile.ignore();    // XDIM=
		LevFile.ignore();
		LevFile >> et_ins->xdim;

		while (LevFile.peek() != '=')  LevFile.ignore();    // YDIM=
		LevFile.ignore();
		LevFile >> et_ins->ydim;

      et_ins->isize = G.imagesize(1, 1, et_ins->xdim, et_ins->ydim);
      mem_size = (et_ins->isize+1)/2 * 2;                                                    
		img = malloc(mem_size);
		Gfile.read((char*)img, et_ins->isize);      // AND mask
		et_ins->offset = GDat.next_offst;
		GDat.next_offst += mem_size;
		if (X.CopyToEMB(img, et_ins->isize, GDat.xms_pic_handle, et_ins->offset) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying enemy AND image)";
			exit(1);
		}
		Gfile.read((char*)img, et_ins->isize);      // XOR mask
		GDat.next_offst += mem_size;
		if (X.CopyToEMB(img, et_ins->isize, GDat.xms_pic_handle, et_ins->offset+mem_size) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying enemy XOR image)";
			exit(1);
		}
		free(img);

      while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF PROJECTILES FIRED=
		LevFile.ignore();
		LevFile >> et_ins->num_projs_fired;

      et_ins->ws_head = new weapon_system_node;
      et_ins->ws_head->next = et_ins->ws_head;
      et_ins->ws_head->prev = et_ins->ws_head;

      for (j = 0; j < et_ins->num_projs_fired; j++)
      {
			w_ins = new weapon_system_node;

         while (LevFile.peek() != '=')  LevFile.ignore();    // TYPE=
			LevFile.ignore();
			LevFile >> w_ins->type;

         while (LevFile.peek() != '=')  LevFile.ignore();    // X OFF=
			LevFile.ignore();
			LevFile >> w_ins->x_off;

         while (LevFile.peek() != '=')  LevFile.ignore();    // Y OFF=
			LevFile.ignore();
			LevFile >> w_ins->y_off;

         while (LevFile.peek() != '=')  LevFile.ignore();    // THETA=
			LevFile.ignore();
			LevFile >> w_ins->theta;

         w_ins->prev = et_ins->ws_head->prev;    // now we get to link them all up!  :)
			w_ins->next = et_ins->ws_head;
			w_ins->prev->next = w_ins;
			w_ins->next->prev = w_ins;
      }

      while (LevFile.peek() != '=')  LevFile.ignore();    // REFIRE=
		LevFile.ignore();
		LevFile >> et_ins->refire;

      while (LevFile.peek() != '=')  LevFile.ignore();    // SOUND NUM=
		LevFile.ignore();
		LevFile >> et_ins->sound_num;

      while (LevFile.peek() != '=')  LevFile.ignore();    // HITPOINTS=
		LevFile.ignore();
		LevFile >> et_ins->hitpoints;

      while (LevFile.peek() != '=')  LevFile.ignore();    // SPEED=
		LevFile.ignore();
		LevFile >> et_ins->speed;

      while (LevFile.peek() != '=')  LevFile.ignore();    // MONEY=
		LevFile.ignore();
		LevFile >> et_ins->money;

		et_ins->prev = E.enemy_types->prev;    // now we get to link them all up!  :)
		et_ins->next = E.enemy_types;
		et_ins->prev->next = et_ins;
		et_ins->next->prev = et_ins;
	}
}
