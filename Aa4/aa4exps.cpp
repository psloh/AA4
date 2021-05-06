#include <alloc.h>        // memory
#include <stdlib.h>       // other stuff
#include <iostream.h>     // cerr

#include "PGphV120.H"     // my graphics
#include "PxmsV111.H"     // xms

#include "Aa4Game.H"      // classes
#include "Aa4Exps.H"
#include "Aa4GDat.H"

extern GraphObject G;     // graphics object
extern XMSObject X;       // xms object
extern GameData GDat;     // game data
extern int mem_size;  // temp mem counter
extern VirtualScreen Vscr_G;  // extern vscr

Explosions::Explosions(void)  // constructor
{
	return;
}

Explosions::~Explosions(void)  // destructor
{
	return;
}

void Explosions::Init(void)    // init linked list
{
	head = new exp_node;
	head->prev = head->next = head;
}

void Explosions::Destruct(void)    // delete linked list
{
	exp_node_ptr del;

	while (head->next != head)
	{
		del = head->next;
		head->next = del->next;
		delete del;
	}
	delete head;
}

void Explosions::Animate(void)
// animate explosions
{
	exp_node_ptr cur, del;    // current and deletion
	exp_pic_ptr picture;      // pointer to the picture of explosion
	int i;      // counter
	void *img;  // image

	cur = head->next;     // traverse linked list of explosions
	while (cur != head)
	{
		if (cur->stage == -1)   // if the explosion is just sparks
		{
			for (i = 0; i < NUM_SPARKS; i++)       // display sparks
				G.putpixel(cur->x - cur->dist + random(7)-3, cur->y - cur->dist + random(7)-3, random(32)+64, Vscr_G);
			for (i = 0; i < NUM_SPARKS; i++)
				G.putpixel(cur->x + cur->dist + random(7)-3, cur->y - cur->dist + random(7)-3, random(32)+64, Vscr_G);
			for (i = 0; i < NUM_SPARKS; i++)
				G.putpixel(cur->x - cur->dist + random(7)-3, cur->y + cur->dist + random(7)-3, random(32)+64, Vscr_G);
			for (i = 0; i < NUM_SPARKS; i++)
				G.putpixel(cur->x + cur->dist + random(7)-3, cur->y + cur->dist + random(7)-3, random(32)+64, Vscr_G);
			cur->dist_left--;         // less distance to go.
			cur->dist += SPARK_MOVE;   // move it.

			if (cur->dist_left <= 0)     // spark should be deleted...
			{
				del = cur;
				cur = cur->prev;
				cur->next = del->next;
				cur->next->prev = cur;
				delete del;
			}
		}
		else    // this is a real explosion
		{
			picture = stages->next;     // get the picture for the current explosion stage.
			for (i = 0; i < cur->stage; i++)
				picture = picture->next;

			cur->stage++;   // go on to the next stage of explosion

         mem_size = (picture->isize+1)/2 * 2;
			img = malloc(mem_size);   // get mem for explosion

			if (X.CopyFromEMB(GDat.xms_pic_handle, picture->offset, picture->isize, img) == 0)   // copy AND image
			{
				cerr << "XMS copy-from error (AND image)";
				exit(1);
			}
			G.putimage(cur->x-picture->xdim/2, cur->y-picture->ydim/2, img, AND_PUT, Vscr_G);  // put image on virtual screen

			if (X.CopyFromEMB(GDat.xms_pic_handle, picture->offset+mem_size, picture->isize, img) == 0)   // copy XOR image
			{
				cerr << "XMS copy-from error (XOR image)";
				exit(1);
			}
			G.putimage(cur->x-picture->xdim/2, cur->y-picture->ydim/2, img, XOR_PUT, Vscr_G);  // put image on virtual screen

			free(img);   // free memory allocated for image.

			if (cur->stage >= num_stages)   // if explosion expired.
			{
				del = cur;
				cur = cur->prev;
				cur->next = del->next;
				cur->next->prev = cur;
				delete del;
			}
		}

		cur = cur->next;   // go on to next element in linked-list
	}
}

void Explosions::Explode(int x, int y)
// start an explosion at (x, y)
{
	exp_node_ptr ins;            // insertion pointer

	ins = new exp_node;          // get memory for insertion
	ins->x = x;           // set the values for an explosion
	ins->y = y;
	ins->stage = 0;
	ins->next_move = 0;

	ins->prev = head->prev;   // link up the list
	ins->next = head;
	ins->prev->next = ins;
	ins->next->prev = ins;
}

void Explosions::Spark(int x, int y, int range)
// start a spark at (x, y) with range [range].
{
	exp_node_ptr ins;   // insertion pointer

	ins = new exp_node;   // get mem
	ins->x = x;                      // set values for sparks
	ins->y = y;
	ins->stage = -1;
	ins->dist_left = range;
	ins->dist = 0;
	ins->next_move = 0;

	ins->prev = head->prev;    // link it up
	ins->next = head;
	ins->prev->next = ins;
	ins->next->prev = ins;
}
