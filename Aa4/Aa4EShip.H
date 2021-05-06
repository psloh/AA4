#ifndef __AA4ESHIP_H
#define __AA4ESHIP_H

#include <fstream.h>
#include "PGphV120.H"
#include "Aa4Class.H"
#include "Aa4Game.H"

struct coord_node;
typedef coord_node *coord_node_ptr;
struct coord_node
{
	int x, y;     // relative coords... entrance to screen is (0, 0)
   coord_node_ptr prev, next;
};

struct path_node;
typedef path_node *path_node_ptr;
struct path_node
{
	int num_points;
	coord_node_ptr coords_head;
   path_node_ptr prev, next;
};

struct weapon_system_node;
typedef weapon_system_node *wsn_ptr;
struct weapon_system_node
{
	int type;
   int x_off, y_off;
	float theta;
	wsn_ptr prev, next;
};

struct enemy_node;
typedef enemy_node *enemy_node_ptr;
struct enemy_node
{
	int xdim, ydim;    // xms stuff
   int isize;
   long offset;

   int num_projs_fired;
   wsn_ptr ws_head;
   int refire;

   int sound_num;
	int hitpoints;
	int speed;      // pix moved for each refresh
   long money;
	enemy_node_ptr prev, next;
};

struct eship_node;
typedef eship_node *en_ptr;
struct eship_node
{
	int type;
	float x, y;
	long row;

   int hitpoints;

   int path_type;
   int stage_in_path;
   float ref_x, ref_y;

   int carries;    // goody carried
   int next_move, next_shoot;
	en_ptr prev, next;
};


class EShips
{
	private:
		en_ptr head;
      int num_paths;
      path_node_ptr paths;
      int num_enemy_types;
      enemy_node_ptr enemy_types;

   public:
   	EShips(void);
		~EShips(void);

		friend void Animate(EShips &E, EProjs &EP, YShip &Y);      
		friend void LoadLevel(Level &L, EShips &E, EProjs &EP, int level_number);
      friend void ParseModFile(ifstream &LevFile, ifstream &Gfile, EProjs &EP, EShips &E);
      friend void CheckYP(YProjs &YP, EShips &E, Goodies &Gd);
      friend void CheckE(EShips &E, YShip &Y, Goodies &Gd);
		friend void Destroy(EShips &E, YProjs &YP, EProjs &EP, Goodies &Gd, Level &L);
      friend void CheckKybd(YShip &Y, YProjs &YP, EShips &E, Pilot &P);
};
#endif
