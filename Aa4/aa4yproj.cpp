#include <alloc.h>        // mem allocation
#include <iostream.h>     // cerr
#include <stdlib.h>       // exit()

#include "PGphV120.H"     // my graphics
#include "PxmsV111.H"     // my tweaked xms

#include "Aa4GDat.H"      // game data

#include "Aa4Yproj.H"     // header for this module




extern GraphObject G;    // external graphics object
extern XMSObject X;      // extern xms object
extern GameData GDat;    // game data
extern int mem_size;     // temp mem counter
extern VirtualScreen Vscr_G;  // extern vscr


YProjs::YProjs(void)
// Pre: None.
// Post: None.
{
	return;
}

YProjs::~YProjs(void)
// Pre: None.
// Post: None.
{
	return;
}

void YProjs::Animate(void)
// Pre: yprojs init.
// Post: yprojs animated.
{
	yproj_node_ptr cur, del;    // traversal and deletion ptrs
	void *img;

	cur = head->next;      // traverse whole list
	while (cur != head)
	{
		cur->y -= GDat.yprojectiles[cur->type].speed * cur->ymove;  // move the projectile
		cur->x += GDat.yprojectiles[cur->type].speed * cur->xmove;
      mem_size = (GDat.yprojectiles[cur->type].isize+1)/2 * 2;
		img = malloc(mem_size);  // get mem

		if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.yprojectiles[cur->type].offset, GDat.yprojectiles[cur->type].isize, img) == 0)   // AND image
		{
			cerr << "XMS copy-from error (AND image)";
			exit(1);
		}
		G.putimage(cur->x-GDat.yprojectiles[cur->type].xdim/2, cur->y-GDat.yprojectiles[cur->type].ydim/2, img, AND_PUT, Vscr_G);  // put image

      if (X.CopyFromEMB(GDat.xms_pic_handle, GDat.yprojectiles[cur->type].offset+mem_size, GDat.yprojectiles[cur->type].isize, img) == 0)   // XOR image
      {
      	cerr << "XMS copy-from error (XOR image)";
			exit(1);
		}
		G.putimage(cur->x-GDat.yprojectiles[cur->type].xdim/2, cur->y-GDat.yprojectiles[cur->type].ydim/2, img, XOR_PUT, Vscr_G);   // put image

		free(img);   // free mem

		if (cur->y < -GDat.yprojectiles[cur->type].ydim ||      // check if projectile went off screen
			 cur->y > GDat.yprojectiles[cur->type].ydim+200 ||
			 cur->x < -GDat.yprojectiles[cur->type].xdim ||
			 cur->x > GDat.yprojectiles[cur->type].xdim+320)
		{
			del = cur;              // delete from linked list
			cur = cur->prev;
			cur->next = del->next;
			cur->next->prev = cur;
			delete del;
      }

	 	cur = cur->next;    // move on to next item
	}
}
