#ifndef __AA4GOOD_H
#define __AA4GOOD_H

#include "Aa4Class.H"
#include "Aa4Game.H"

struct good_node;
typedef good_node *good_node_ptr;
struct good_node
{
	char type;
   int x, y;    // screen coords, not global coords
   int next_move;        // number of iterations to next move
   good_node_ptr prev, next;
};

class Goodies
{
	private:
   	good_node_ptr head;

   public:
   	Goodies(void);
      ~Goodies(void);
      void InitGoodies(void);
      void Animate(void);
      void AddGoody(int x, int y, int type);

      friend void CheckGd(Goodies &Gd, YShip &Y, Pilot &P);
      friend void Destroy(EShips &E, YProjs &YP, EProjs &EP, Goodies &Gd, Level &L);
};
#endif
