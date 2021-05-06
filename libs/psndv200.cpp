#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip.h>
#include <conio.h>
#include <dos.h>

#include "PSndV200.H"


int baseio, irq, dma, dma16;  // blaster environment variable
char SFX, Music;  // sound f-x flag and music flag
char midi_repeat_mode;  // flag for midi repeat

int seq_repeat_number;  //sequence to repeat

void interrupt ( *old_timer_handler)(...);   // old timer interrupt


static char Blaster_used, MIDI_used;


void init_smix_form_1(void)
{
	cout << "-------------------------------------------\n";
   cout << "Sound Mixing Library v1.30 by Ethan Brodsky\n";
   if (!detect_settings(&baseio, &irq, &dma, &dma16))
   {
   	cout << "\n\nInvalid or non-existant BLASTER environment variable!\n";
      cout << "There will be no sound effects.\n";
      cout << "Press a key...\n";
      if (getch() == 0) getch();
      SFX = Blaster_used = 0;
      return;
   }
   else
   {
   	if (!init_sb(baseio, irq, dma, dma16))
      {
      	cout << "Error initializing sound card!\n";
         cout << "There will be no sound effects.\n";
      	cout << "Press a key...\n";
      	if (getch() == 0) getch();
      	SFX = Blaster_used = 0;
      	return;
      }
   }

   cout << "BaseIO=" << hex << baseio << dec << "     "
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

   init_mixing();
   cout << "\n";

   SFX = Blaster_used = 1;
}

void shutdown_smix_form_1(void)
{
	if (Blaster_used)
   {
		shutdown_mixing();
   	shutdown_sb();
   	cout << "\n";
   }
}


void interrupt midi_timer_handler(...)
{
   // call the old routine
	old_timer_handler();

   if (midi_repeat_mode)                    // if repeating stuff...
   	if (SequenceStatus() == SEQ_DONE)
      	PlaySequence(seq_repeat_number);
}


void init_midi_form_1(char *XMI_filename)
{
	FILE *testfile;  // finds directory of drivers
   char path[80], path1[80], path2[80], path3[80];
   long siz;
   char *fname;    // name of MIDI file --- stored in memory

   MIDI_used = 1;                      // set flags
   midi_repeat_mode = 1;
   seq_repeat_number = -1;

   old_timer_handler = getvect(0x1C);  // install midi timer handler
   setvect(0x1C, midi_timer_handler);

   Music = 1;  // so far there is music

   if ((testfile = fopen("MIDPAK.COM", "r")) != NULL)
   {
   	strcpy(path, "");
      fclose(testfile);
   }
   else if ((testfile = fopen("h:\\poshen\\sounds\\libs\\midi\\MIDPAK.COM", "r")) != NULL)
   {
   	strcpy(path, "h:\\poshen\\sounds\\libs\\midi\\");
      fclose(testfile);
   }
   else
   {
   	Music = 0;
      cout << "Error!  MIDPAK drivers not found.  There will be no music.\n";
   }

   strcpy(path1, path);      strcat(path1, "MIDPAK.COM");
   strcpy(path2, path);      strcat(path2, "MIDPAK.ADV");
   strcpy(path3, path);      strcat(path3, "MIDPAK.AD");

	if (LoadMidPak(path1, path2, path3))  // load all MIDI drivers
	{
   	cout << "Loaded MIDPAK.COM MIDPAK.ADV and MIDPAK.AD into Low Mem\n";
		if (InitMidPak())  // initialize MidPak.
		{
			cout << "MIDPAK driver initialized.\n";
			fname = fload(XMI_filename, &siz);  // load XMI file into memory
			if (fname)  // if file was loaded properly
			{
				cout << "Loaded XMI file " << siz << " bytes long.\n";
				RegisterXmidi(fname, siz);   // load MIDI file into memory
            Music = 0;
			}
		}
		else   // could not intialize midi
      {
			cout << "Failed to initialize MIDPAK driver.\n";
         Music = 0;
         MIDI_used = 0;
      }
	}
   else
   {
   	cout << "MIDI drivers not found!\n";
      MIDI_used = 0;
   }
}

void shutdown_midi_form_1(void)
{
	if (SequenceStatus() == SEQ_PLAYING)  // if MIDI still playing
   	MidiStop();   // stop it

   setvect(0x1C, old_timer_handler);    // restore old interrupts

	UnLoadMidPak();   // de-init midpak
}

void start_sequence(int seq_num, char repeat)
{
	if (repeat)
   {
   	midi_repeat_mode = 1;
      seq_repeat_number = seq_num;
   }

   PlaySequence(seq_num);
}

