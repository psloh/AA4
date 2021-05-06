#ifndef __AA4YPROJ_H
#define __AA4YPROJ_H

#include "Aa4Class.H"
#include "PGphV120.H"
#include "Aa4Game.H"

struct yproj_node;
typedef yproj_node *yproj_node_ptr;
struct yproj_node
{
	char type;
   float x, y;  // local coords
   float theta;  // angle headed
	int next_move;
   float xmove, ymove;  // increments
   yproj_node_ptr prev, next;
};

class YProjs
{
	private:
   	yproj_node_ptr head;

   public:
   	YProjs(void);
      ~YProjs(void);
      void Animate(void);


      friend void InitProjs(YProjs &YP, EProjs &EP);
      friend void CheckYP(YProjs &YP, EShips &E, Goodies &Gd);
      friend void CheckKybd(YShip &Y, YProjs &YP, EShips &E, Pilot &P);
      friend void Destroy(EShips &E, YProjs &YP, EProjs &EP, Goodies &Gd, Level &L);
};

#endif
