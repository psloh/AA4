#include <iostream.h>      // cout
#include <stdlib.h>        // exit()
#include <iomanip.h>       // formatting (hex)
#include <conio.h>         // getch()

extern "C"                 // These are Ethan's sound header files
{
	#include "detect.h"
	#include "smix.h"
}

#include "Aa4Snds.H"       // Header file for this module

int baseio, irq, dma, dma16;    // internal variables of SB card
SOUND *Sound[NUMSOUNDS];        // list of loaded sounds (all in memory)
char Music = 1, SFX = 1;        // global flags about whether or not certain sound effects, etc are installed.

// exit procedure, called on quit.
void SndExitProc(void)
// Pre: About to quit.
// Post: Sound memory freed.
{
	int i;

	for (i=0; i < NUMSOUNDS; ++i)     // frees XMS allocated for sounds
		if (Sound[i] != NULL)
			free_sound(Sound+i);

	#ifdef SHAREDEMB         // if sounds are stored in the same extended memory block, then that has to be disabled.
		shutdown_sharing();
	#endif
}

void loadsounds(void)
// Pre: None.
// Post: Sounds are loaded from MUSIC.DAT and SFX.DAT into XMS
{
   if (!init_xms())                     // tries to initialize XMS
   {
   	cout << "Can not initialize extended memory\n";
      cout << "HIMEM.SYS must be installed\n";
      cout << "There will be no sounds.\n";
      cout << "Press a key...\n";
      if (getch() == 0) getch();
      Music = 0;
      SFX = 0;
      return;
   }

   cout << "Extended memory successfully initialized\n";
   cout << "Free XMS memory: " << getfreexms() << "K\n";
   if (getfreexms() < XMSREQUIRED)          // checks if there is enough XMS
   {
   	cout << "Insufficient free XMS\n";
      cout << "There will be no sounds.\n";
      cout << "Press a key...\n";
      if (getch() == 0) getch();
      Music = 0;
      SFX = 0;
      return;
   }

   cout << "Loading sounds...  ";
	#ifdef SHAREDEMB        // prepares sharing of emb's if necessary
    	init_sharing();
	#endif

   // Opens music file, with encryption level 3
   if (!open_sound_resource_file("MUSIC.DAT", 3))
   {     // if can't open file, then displays errors.
   	cout << "Can't load sound resource file\n";
		cout << "There will be no background music...\nPress a key...\n";
      if (getch() == 0) getch();
      Music = 0;
   }
   else
   {    // if no error, load sound into memory
   	load_sound(&(Sound[0]), "Background");
      load_sound(&(Sound[7]), "14");
      load_sound(&(Sound[8]), "41");
		close_sound_resource_file();   // close sound resource file
   }

   // Opens sound FX file, with encryption III (again)
   if (!open_sound_resource_file("SFX.DAT", 3))
   {   // same as above
   	cout << "Can't load sound resource file\n";
		cout << "There will be no sound effects...\nPress a key...\n";
      if (getch() == 0) getch();
      SFX = 0;
   }
   else
   {   // load all the sound effects
      load_sound(&(Sound[1]), "PHOTONGUN");
      load_sound(&(Sound[2]), "EXPLOSION");
      load_sound(&(Sound[3]), "FUSIONGUN");
      load_sound(&(Sound[4]), "MACHINE_GUN");
      load_sound(&(Sound[5]), "HIT_1");
      load_sound(&(Sound[6]), "SHIELD");
		close_sound_resource_file();
   }

   atexit(SndExitProc);    // load exit procedure into list of exit procs... called on quit.

   cout << "done!\n";
}

void freesounds(void)
// Pre: None.
// Post: All memory tied up in sounds is freed.
{
	int i;

   if (Music)     // if music loaded
   {
   	free_sound(Sound+0);   // free music
      free_sound(Sound+7);
      free_sound(Sound+8);
   }
   if (SFX)    // if sound effects loaded
		for (i = 1; i < NUMSOUNDS-2; i++)    // free sound effects from XMS
   		free_sound(Sound+i);
	#ifdef SHAREDEMB
    	shutdown_sharing();   // disable sharing of emb's
	#endif
}

void init(void)
// Pre: None.
// Post: SB card initialized for use with my program.
{
	cout << "-------------------------------------------\n";
   cout << "Sound Mixing Library v1.30 by Ethan Brodsky\n";  // credit the author
   if (!detect_settings(&baseio, &irq, &dma, &dma16))  // try to load SB card info
   {
   	cout << "\n\nInvalid or non-existant BLASTER environment variable!\n";
      cout << "There will be no sounds.\n";
      cout << "Press a key...\n";
      if (getch() == 0) getch();
      Music = 0;
      SFX = 0;
      return;
   }
   else  // it worked!
   {
   	if (!init_sb(baseio, irq, dma, dma16))   // attempt to initialize card.
      {
      	cout << "Error initializing sound card!\n";
         cout << "There will be no sounds.\n";
      	cout << "Press a key...\n";
      	if (getch() == 0) getch();
      	Music = 0;
      	SFX = 0;
      	return;
      }
   }

   cout << "BaseIO=" << hex << baseio << dec << "     "     // print info
   	  << "IRQ" << irq << "     "
        << "DMA8=" << dma << "     "
        << "DMA16=" << dma16 << "\n";

   cout << "DSP version " << setprecision(1) << (dspversion >> 8)
   	  << setprecision(2) << (dspversion & 0xFF) << ":  ";

   if (sixteenbit)
   	cout << "16-bit, ";
   else
   	cout << "8-bit, ";
   if (autoinit)
      cout << "Auto-initialized\n";
   else
      cout << "Single-cycle\n";

   init_mixing();     // initialize sound mixing
   cout << "-------------------------------------------\n";
}

void shutdown(void)
// Pre: None.
// Post: All SB stuff shut down.
{
	shutdown_mixing();
   shutdown_sb();

   cout << "\n";
}

void InstallSounds(void)
// Pre: None.
// Post: Sounds are installed.
{
	init();           // initialize this module
	if (Music || SFX)     // if there are to be sounds, load them.
   	loadsounds();
   if (Music)        // if there is background music...
   	start_sound(Sound[SHOP_SOUND], SHOP_SOUND, ON);  // Play background
   cout << "Press a key to begin... ";
   if (getch() == 0) getch();
}

void UnInstSounds(void)
// Pre: Sounds installed.
// Post: Sounds un-installed
{
	if (!Music && !SFX)       // if no sounds installed, leave
   	return;
	for (int i = 0; i < NUMSOUNDS; i++)    // deallocate all XMS used
   	if (sound_playing(i))
      	stop_sound(i);
   shutdown();     // shut down SB unit
   freesounds();    // free all memory used in sounds
}