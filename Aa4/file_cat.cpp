#include <fstream.h>
#include <iostream.h>
#include <io.h>
#include <fcntl.h>

int main(void)
{
	char filename[301];
   int a, ln_a;
   int i;
   char buf[3];

   cout << "This program will parse the given file." << endl;
   cout << "It will put all the indicated files together into one" << endl;
   cout << "large output file." << endl;
   cout << endl;

   cout << "File to parse: ";
   cin.getline(filename, 300);
   ifstream parse(filename);
   cout << "Output file: ";
   cin.getline(filename, 300);
   ofstream outf(filename);

   ifstream temp_in;

   while (parse)
   {
   	parse.getline(filename, 300);
      if (strlen(filename) < 3)
      	break;

      a = open(filename, O_RDONLY);
	   ln_a = filelength(a);
	   close(a);

      temp_in.open(filename, ios::in);
      for (i = 0; i < ln_a; i++)
	   {
	   	temp_in.read(buf, 1);
	      outf.write(buf, 1);
	   }
      temp_in.close();
   }

   outf.close();
   parse.close();
}

