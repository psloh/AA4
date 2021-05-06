#include <fstream.h>
#include <iostream.h>
#include <alloc.h>

#include "PGphV120.H"


int main(void)
{
	GraphObject G;
	char filename[301];
   int i, j;

	cout << "This will take in a .PIC file and convert all reds to blues." << endl;
   cout << "Source file: ";
   cin.getline(filename, 300);
   ifstream inf(filename);
   cout << "Destination file: ";
   cin.getline(filename, 300);
   ofstream outf(filename);

   int xdim, ydim, isize;
   void *xor, *and;

   inf.read((char*)&xdim, sizeof(int));
   inf.read((char*)&ydim, sizeof(int));
   inf.read((char*)&isize, sizeof(int));

   xor = malloc(isize);
   and = malloc(isize);

   inf.read((char*)xor, isize);
   inf.read((char*)and, isize);
   inf.close();

   G.initgraph();
   G.clearviewport();
   G.putimage(0, 0, xor, COPY_PUT);

   for (i = 0; i < xdim; i++)
   	for (j = 0; j < ydim; j++)
      {
      	if (G.getpixel(i, j) >= 32 || G.getpixel(i, j) < 16 && G.getpixel(i, j) != 0)
         	G.putpixel(i, j, G.getpixel(i, j)-3);
      }

   G.getimage(0, 0, xdim-1, ydim-1, xor);

   outf.write((char*)&xdim, sizeof(int));
   outf.write((char*)&ydim, sizeof(int));
   outf.write((char*)&isize, sizeof(int));
   outf.write((char*)xor, isize);
   outf.write((char*)and, isize);
   outf.close();

   free(xor);
   free(and);

   G.closegraph();

   return 0;
}

