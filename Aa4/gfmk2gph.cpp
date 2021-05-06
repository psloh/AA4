#include <fstream.h>
#include <stdlib.h>

int main(void)
{
	char filename[301];

	cout << "Input file (GrafMk2 format): ";
   cin.getline(filename, 300);
	ifstream inf(filename);

   cout << "Output file (GPH format): ";
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

   outf.write((char*)and, isize);
   outf.write((char*)xor, isize);
	outf.close();

   cout << "-----------------------------------" << endl;
   cout << " XDIM: " << xdim << endl;
   cout << " YDIM: " << ydim << endl;
	cout << "-----------------------------------" << endl;

   return 0;
}
