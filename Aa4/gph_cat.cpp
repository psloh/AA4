#include <io.h>
#include <fcntl.h>
#include <fstream.h>
#include <iostream.h>

int main(void)
{
	char filename[301];
   char buf[2];
   int a, b;
   int ln_a, ln_b;
   int i;

	cout << "File B will be put on to the end of File A and saved as File C." << endl;
   cout << endl;
   cout << "File A: ";
   cin.getline(filename, 300);
   a = open(filename, O_RDONLY);
   ln_a = filelength(a);
   close(a);
   ifstream A(filename);
   cout << "File B: ";
   cin.getline(filename, 300);
   b = open(filename, O_RDONLY);
   ln_b = filelength(b);
   close(b);
   ifstream B(filename);
   cout << "File C: ";
   cin.getline(filename, 300);
   ofstream tmp(filename);
	for (i = 0; i < ln_a; i++)
   {
   	A.read(buf, 1);
      tmp.write(buf, 1);
   }
   for (i = 0; i < ln_b; i++)
   {
   	B.read(buf, 1);
      tmp.write(buf, 1);
   }
   tmp.close();
   A.close();
   B.close();

   return 0;
}
