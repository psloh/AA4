#include <conio.h>
#include <alloc.h>
#include <iostream.h>
#include <ctype.h>
#include <fstream.h>
#include <stdlib.h>
#include <dos.h>

#include "PGphV120.H"


const int INC = 5;

GraphObject G;


int main(int argc, char *argv[])
{
	if (argc != 2)
   {
   	cout << "Usage: TESTANIM <.PIC file>";
      return 0;
   }

   ifstream inf(argv[1]);
	int xdim, ydim, isize;
   void far *xor, *and, *old;
   int i, j;
   int x, y;
   char ch;
   VirtualScreen vscr;
   G.makevirtual(&vscr);
   G.clearviewport(vscr);

   randomize();

   G.initgraph();
   G.clearviewport();
   for (i = 0; i < 20; i++)
   	for (j = 0; j < 32; j++)
      {
      	G.setfillcolor(random(256));
         G.bar(j*10, i*10, j*10+9, i*10+9, vscr);
      }

   inf.read((char*)&xdim, sizeof(int));
   inf.read((char*)&ydim, sizeof(int));
   inf.read((char*)&isize, sizeof(int));
   xor = farmalloc(isize);
   and = farmalloc(isize);
   old = farmalloc(isize);
   inf.read((char*)xor, isize);
   inf.read((char*)and, isize);

   x = 160;
   y = 100;
   G.getimage(x-xdim/2, y-ydim/2, x-xdim/2+xdim-1, y-ydim/2+ydim-1, old, vscr);
   G.putimage(x-xdim/2, y-ydim/2, and, AND_PUT, vscr);
   G.putimage(x-xdim/2, y-ydim/2, xor, XOR_PUT, vscr);
   ch = 'b';
   do
   {
   	G.waitretrace();
      G.swap(VGAaddr, FP_SEG(vscr));
   	while (!kbhit()) ;
      G.putimage(x-xdim/2, y-ydim/2, old, COPY_PUT, vscr);
      ch = getch();
      ch = toupper(ch);
      switch (ch)
      {
      	case 'J' : x -= INC;
         			  break;
         case 'K' : y += INC;
         			  break;
         case 'L' : x += INC;
         			  break;
         case 'I' : y -= INC;
         default  : ;
      }

      G.getimage(x-xdim/2, y-ydim/2, x-xdim/2+xdim-1, y-ydim/2+ydim-1, old, vscr);
	   G.putimage(x-xdim/2, y-ydim/2, and, AND_PUT, vscr);
	   G.putimage(x-xdim/2, y-ydim/2, xor, XOR_PUT, vscr);
   } while (ch != 'Q');

   farfree(xor);
   farfree(and);
   farfree(old);
   G.clearvirtual(&vscr);
   G.closegraph();

   return 0;
}
