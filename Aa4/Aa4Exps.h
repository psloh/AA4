#ifndef __AA4EXPS_H
#define __AA4EXPS_H

#include "PGphV120.H"

#include "Aa4GDat.H"


struct exp_pic;
typedef exp_pic *exp_pic_ptr;
struct exp_pic
{
	int xdim, ydim;
   int isize;
   long offset;

   exp_pic_ptr prev, next;
};


struct exp_node;
typedef exp_node *exp_node_ptr;
struct exp_node
{
	int x, y;
   int stage;                      // stage = -1 ==> sparks
   int dist_left;                  // for sparks
   int dist;
   int next_move;
   exp_node_ptr prev, next;
};


class Explosions
{
	private:
  		exp_node_ptr head;
      exp_pic_ptr stages;

   public:
   	int xdim, ydim;     // spacing of explosions
      int num_stages;

   	Explosions(void);
      ~Explosions(void);
      void Animate(void);
      void Init(void);
      void Destruct(void);

      void Explode(int x, int y);
      void Spark(int x, int y, int range);

      friend void LoadGameData(GameData &GDat, Explosions &XP);
};


#endif
