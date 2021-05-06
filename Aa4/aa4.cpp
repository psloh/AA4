#include <stdlib.h>            // for randomize
#include <string.h>            // for checking command-line arguments

#include "PGphV120.H"          // my graphics
#include "PxmsV111.H"          // my modified xms
extern "C"
{
	#include "detect.h"         // ethan's sound
	#include "smix.h"
}

#include "Aa4Menus.H"          // maim menu
#include "Aa4Snds.H"           // sounds
#include "Aa4Kybd.H"           // keyboard
#include "Aa4xms.H"            // xms
#include "Aa4Scrns.H"          // screens


extern SOUND *Sound[];     // in aa4snds.cpp
extern char Music, SFX;     // music and sound-effects flags
GraphObject G;                   // graph object
XMSObject X;                     // xms object
VirtualScreen Vscr_G;      // virtual screen
char no_sound;            // no-sound flag


int main(int argc, char *argv[])
{
	no_sound = 0;

	if (argc == 2)
   	if (!strcmpi(argv[1], "-s"))
	   {
	   	no_sound = 1;
	      Music = SFX = 0;
	   }

	randomize();                  // initializes the random-number generator

   StartInfo();

	Install_xms();                // install xms
	InstallKyb();                 // install keyboard

   if (!no_sound)
		InstallSounds();              // install sounds

	Menu();                       // show menu

   EndingInfo();

   if (Music)
		stop_sound(SHOP_SOUND);                // stop background music
   if (!no_sound)
		UnInstSounds();               // uninstall sounds

	Uninstall_xms();              // uninstall xms

	return 0;
}