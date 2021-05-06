#ifndef __AA4GAME_H
#define __AA4GAME_H

#include "PGphV120.H"

const int NUM_WEAPONS = 15;
const int NUM_YPROJS = 30;
const int NUM_PROJECTILES_PER_WEAP = 10;
const int NUM_YGOODIES = 20;
const int SPEED = 1;
const int NUM_SPARKS = 3;
const int SPARK_MOVE = 1;
const int GOODY_MOVE_PIX = 2;
const int WEAPON_SWITCH_DELAY = 5;


struct Pilot
{
	char name[80];
   long credits;
   int level;     // next level to fly
   char weapon_list[NUM_WEAPONS];   // contains 1 or 0, depending on whether or not the weapon is carried.
   int primary;     // primary weapon
   int secondary;   // secondary weapon
	int shields;   // shield level
   long ID;         // id number
};


void NewGame(void);
void LoadGame(void);
void Game(Pilot &P);   

#endif
