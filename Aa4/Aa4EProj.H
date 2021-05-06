#ifndef __AA4EPROJ_H
#define __AA4EPROJ_H

#include <fstream.h>

#include "PGphV120.H"

#include "Aa4Class.H"

struct eproj_node;
typedef eproj_node *eproj_node_ptr;
struct eproj_node
{
	int type;
	float x, y;  // local coords
	float theta;  // angle headed
	int next_move;  // number of iterations to next move
   float xmove, ymove;  // incremental movements
   eproj_node_ptr prev, next;
};

struct eproj_type_node;
typedef eproj_type_node *etn_ptr;
struct eproj_type_node
{
	int xdim, ydim;   // xms stuff
	int isize;
	long offset;

   int speed;

	int radius;
	int hitpoints;
	etn_ptr prev, next;
};


class EProjs
{
	private:
   	int num_types;   // types of eprojs
      etn_ptr eproj_types;
      eproj_node_ptr head;

   public:
		EProjs(void);
		~EProjs(void);
		void Animate(void);

		friend void Animate(EShips &E, EProjs &EP, YShip &Y);
      friend void InitProjs(YProjs &YP, EProjs &EP);
      friend void CheckEP(EProjs &EP, YShip &Y);
      friend void LoadLevel(Level &L, EShips &E, EProjs &EP, int level_number);
      friend void ParseModFile(ifstream &LevFile, ifstream &Gfile, EProjs &EP, EShips &E);
		friend void Destroy(EShips &E, YProjs &YP, EProjs &EP, Goodies &Gd, Level &L);
};
#endif

