#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <string.h>

#include "Aa4Game.H"


int main(void)
{
	Pilot P;
   int num;
   char filename[80];
   char name[80];
   int i;

   cout << "Which save-file do you wish to edit?  (Enter a number): " << endl;
   cin >> num;
   cin.getline(name, 80);
   sprintf(filename, "SAVGAME.%03d", num);
   ifstream inf(filename);
   inf.read((char*)&P, sizeof(Pilot));
   inf.close();

   P.weapon_list[0] = 1;  // basic cannon.
   for (i = 1; i < NUM_WEAPONS; i++)
   	P.weapon_list[i] = 0;
	P.primary = 0;
   P.secondary = -1;
   P.shields = 100;
   P.ID = num;

   cout << "Name: ";
   cin.getline(name, 79);
   strcpy(P.name, name);
   cout << "Credits: ";
   cin >> P.credits;
   cout << "Level: ";
   cin >> P.level;

   ofstream outf(filename);
   outf.write((char*)&P, sizeof(Pilot));
   outf.close();

   return 0;
}
