#include <math.h>         // math
#include <alloc.h>        // allocating memory
#include <stdlib.h>       // exit()
#include <iostream.h>     // cerr

#include "PGphV120.H"     // my graphics
#include "PxmsV111.H"     // my adapted xms

#include "Aa4Eproj.H"     // header for this module
#include "Aa4GDat.H"      // game data


extern GraphObject G;     // graphics object
extern XMSObject X;       // xms object
extern GameData GDat;     // game data
extern int mem_size;      // temp mem ctr
extern VirtualScreen Vscr_G;  // extern vscr


EProjs::EProjs(void)
// Pre: None.
// Post: None.
{
	return;
}

EProjs::~EProjs(void)
// Pre: None.
// Post: Memory deallocated.
{
	return;
}

void EProjs::Animate(void)
// Pre: eprojs init.
// Post: eprojs animated.
{
	eproj_node_ptr cur, del;      // traversal and deletion pointers
	etn_ptr eproj_type;           // enemy type pointer
	void *img;    // image
	int i;   // counter

	cur = head->next;     // traverse list
	while (cur != head)
	{
		eproj_type = eproj_types->next;    // find type of enemy
		for (i = 0; i < cur->type; i++)
			eproj_type = eproj_type->next;

		cur->y -= cur->ymove*eproj_type->speed;
		cur->x += cur->xmove*eproj_type->speed;

      mem_size = (eproj_type->isize+1)/2 * 2;
		img = malloc(mem_size);   // get memory

		if (X.CopyFromEMB(GDat.xms_pic_handle, eproj_type->offset, eproj_type->isize, img) == 0)   // AND image
		{
			cerr << "XMS copy-from error (AND image)";
			exit(1);
		}
		G.putimage(cur->x-eproj_type->xdim/2, cur->y-eproj_type->ydim/2, img, AND_PUT, Vscr_G);  // put picture on screen

		if (X.CopyFromEMB(GDat.xms_pic_handle, eproj_type->offset+mem_size, eproj_type->isize, img) == 0)   // XOR image
		{
			cerr << "XMS copy-from error (XOR image)";
			exit(1);
		}
		G.putimage(cur->x-eproj_type->xdim/2, cur->y-eproj_type->ydim/2, img, XOR_PUT, Vscr_G);  // put picture on screen

		free(img);   // free memory

		if (cur->y < -eproj_type->ydim ||
			 cur->y > eproj_type->ydim+200 ||
			 cur->x < -eproj_type->xdim ||
			 cur->x > eproj_type->xdim+320)   // check for deletion
		{
			del = cur;
			cur = cur->prev;
			cur->next = del->next;
			cur->next->prev = cur;
			delete del;
		}

		cur = cur->next;   // move on to next member of list
	}
}