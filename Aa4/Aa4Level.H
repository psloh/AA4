#ifndef __AA4LEVEL_H
#define __AA4LEVEL_H

#include "Aa4Class.H"

#include "PGphV120.H"

struct lev_node;
typedef lev_node *lev_node_ptr;
struct lev_node
{
	int tiles[20];    // horizontally, left->right
   lev_node_ptr prev, next;
};

class Level
{
	private:
   	int lev_strips[400];
      long lev_rows;
      int num_diff_lev_strips;
      lev_node_ptr lev_strip_defs;
      char lev_descrip[25][61];    // level description block
      int ld_lines;
      int num_diff_scen_tiles;
      char destroyed;
      int track;

		void *img;

      long land_offset;

   public:
   	Level(void);
      ~Level(void);
      void Animate(void);
      void ShowInfo(int level_no);
      void ResetMusic(void);

      friend void LoadLevel(Level &L, EShips &E, EProjs &EP, int level_number);
		friend void Destroy(EShips &E, YProjs &YP, EProjs &EP, Goodies &Gd, Level &L);
}; 
#endif      