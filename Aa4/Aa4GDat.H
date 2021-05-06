#ifndef __AA4GDAT_H
#define __AA4GDAT_H

#include "Aa4Game.H"

struct yprojectile
{
	int xdim, ydim;   // xms image
   int isize;
   long offset;

   int speed;
	int radius;                               // explosion radius
	int hitpoints;
};

struct yweapon
{
	int xdim, ydim;          // xms image
   int isize;
   long offset;

   int lines;               // at shop
   char descrip[10][45];
	long price;
   int primary;             // is it a primary?

	int num_projs_shot;      // weapon stats
	int proj[NUM_PROJECTILES_PER_WEAP],
       x_off[NUM_PROJECTILES_PER_WEAP],
       y_off[NUM_PROJECTILES_PER_WEAP],
       theta[NUM_PROJECTILES_PER_WEAP];
	int refire;
	int power;

   int sound_num;           // sfx ID
};

struct ygoody
{
	int xdim, ydim;          // xms image
   int isize;
   long offset;

   long cash;               // +money
   int hitpoints;           // +hitpoints
   int weapon;              // +weapon
};


struct GameData
{
	int max_level;                   // maximum level

	int xms_pic_handle;              // xms stuff
	long ys_pic_offst;
	long next_offst;

   // ;general information
   int speed_mode;                  // # of waitretrace's before moving on 

	// ;ship information
	int ys_xdim, ys_ydim;            // yship image stuff
	int ys_isize;

	// ;projectiles
	int num_yprojs;
	yprojectile yprojectiles[NUM_YPROJS];

	// ;weapons
	int num_yweaps;
	yweapon yweapons[NUM_WEAPONS];

	// ;goodies
	int num_ygoodies;
	ygoody ygoodies[NUM_YGOODIES];
};

void LoadGameData(GameData &GDat);

#endif
