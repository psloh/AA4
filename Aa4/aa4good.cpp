#include <alloc.h>      // memory
#include <stdlib.h>     // exit()
#include <iostream.h>   // cerr

#include "PGphV120.H"   // my graphics
#include "PxmsV111.H"   // ethan's xms

#include "Aa4Good.H"    // header for this module
#include "Aa4Game.H"    // game constants
#include "Aa4GDat.H"    // game data


extern GraphObject G;   // graphics object
extern XMSObject X;     // xms object
extern GameData GDat;   // game data
extern int mem_size;    // temp mem counter
extern VirtualScreen Vscr_G;  // extern vscr

Goodies::Goodies(void)
// Pre: None.
// Post: None.
{
	return;
}

Goodies::~Goodies(void)
// Pre: None.
// Post: None.
{
	return;
}

void Goodies::Animate(void)
// Pre: Goodies init.
// Post: Goodies animated.
{
	good_node_ptr cur, del;     // traversal and deletion pointers
	void *img;                  // image

	cur = head->next;           // loop thru list
	while (cur != head)
	{
		cur->y += GOODY_MOVE_PIX;      // move down

      mem_size = (GDat.ygoodies[cur->type].isize+1)/2 * 2;
		img = malloc(mem_size);  // get mem

		if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.ygoodies[cur->type].offset, GDat.ygoodies[cur->type].isize, img) == 0)   // AND image
		{
			cerr << "XMS copy-from error (AND image)";
			exit(1);
		}
		G.putimage(cur->x-GDat.ygoodies[cur->type].xdim/2, cur->y-GDat.ygoodies[cur->type].ydim/2, img, AND_PUT, Vscr_G);  // put and image

		if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.ygoodies[cur->type].offset+mem_size, GDat.ygoodies[cur->type].isize, img) == 0)   // XOR image
		{
			cerr << "XMS copy-from error (XOR image)";
			exit(1);
		}
		G.putimage(cur->x-GDat.ygoodies[cur->type].xdim/2, cur->y-GDat.ygoodies[cur->type].ydim/2, img, XOR_PUT, Vscr_G);   // put xor image

		free(img);  // free mem

		if (cur->y < -GDat.ygoodies[cur->type].ydim ||       // check if it gets off screen
			 cur->y > GDat.ygoodies[cur->type].ydim+200 ||
			 cur->x < -GDat.ygoodies[cur->type].xdim ||
			 cur->x > GDat.ygoodies[cur->type].xdim+320)
		{
			del = cur;                // delete linked list
			cur = cur->prev;
			cur->next = del->next;
			cur->next->prev = cur;
			delete del;
		}

		cur = cur->next;
	}
}

void Goodies::InitGoodies(void)
// Pre: None.
// Post: Goodies init.
{
	head = new good_node;
   head->next = head->prev = head;
}

void Goodies::AddGoody(int x, int y, int type)
// insert a goody at (x,y) of type [type].
{
	good_node_ptr ins;    // insertion ptr

   ins = new good_node;     // insert it
   ins->x = x;
   ins->y = y;
   ins->type = type;
   ins->next_move = 0;
   ins->prev = head->prev;
   ins->next = head;
   ins->prev->next = ins;
   ins->next->prev = ins;
}
