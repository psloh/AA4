extern "C"
{
	// loader.h starts here:
	// ** C prototype header to MIDPAK and DIGPAK loader functions.
	int LoadDigPak(char far *fname); // Load digpak driver, returns' true if loaded.
	int InitDigPak(void); // Initialize Digpak true, if successfull.
	int DeInitDigPak(void); // Uninitialize DigPak.
	int UnLoadDigPak(void); // unload digpak driver from memory.

	int LoadMidPak(char far *fname,char far *adv,char far *ad); // Load MidPak driver, returns' true if loaded.
	int InitMidPak(void); // Initialize MidPak true, if successfull.
	int DeInitMidPak(void); // Uninitialize MidPak.
	int UnLoadMidPak(void); // unload MidPak driver from memory.

	// midpak.h starts here:
	extern short	cdecl far CheckMidiIn(void); // Returns 1 if MIDPAK is installed, 0 if not.
	extern short	cdecl far DigPakAvailable(void);	// Returns 1 if DIGPAK is installed, 0 if not.

	/****************************************************************************/
	/* These flags are returned by the XMIDI registration call. 		    */
	/****************************************************************************/
	#define FAILURE_TO_REGISTER 0 	// Xmidi file registration failed.
	#define REGISTERED_RESIDENT 1   // Resident driver holds midi file now.
					// The application can throw away the memory
					// if it wants to.
	#define REGISTERED_APPLICATION 2  // Driver didn't have a big enough buffer
					  // area reserved to hold the audio data
					  // so the application is responsible for
					  // keeping the memory for this sequence
					  // while it is registered.

	extern short	cdecl far PlaySequence(short seqnum); // Play a particular sequence number from
					  // the currently registered xmidi file.

	#define NEXT_CALLBACK -1	// Activation on next callback.

	extern short	cdecl far SegueSequence(short seqnum,short activate);
					// Switch sequence to this sequence when next
					// callback trigger event is hit with the
					// event number equal to activate.	If activate
					// is set to -1 then the next event causes the
					// segue to occur.

	extern short	cdecl far RegisterXmidi(char	far *xmidi,long int size);
					// Registers an extended midi file for playback.
					// This call will register all sequences.

	extern short	cdecl far MidiStop(void); 	// Stop playing current sequence.

	extern long int  cdecl far ReportCallbackTrigger(void); // Low word is trigger count.
						  // High word is last event ID.

	extern void  cdecl far ResetCallbackCounter(void);	// Reset callback counter to zero.

	extern void  cdecl far ResumePlaying(void); 	// Resume playing last sequence.

	#define SEQ_STOPPED 0 		// equates for SequenceStatus()
	#define SEQ_PLAYING 1
	#define SEQ_DONE 2
	extern short	 cdecl far SequenceStatus(void);	// Report current sequence play status.

	extern short	 cdecl far RelativeVolume(short vol); // Report current volume.

	extern void  cdecl far SetRelativeVolume(short vol,short time); // Set volume, over time period.

	#define NOBUFFER 1		// No resident buffer available.
	#define FILENOTFOUND 2		// The file was not found.
	#define FILETOBIG 3 		// The file exceeds the reserved buffer size.
	#define REGISTRATIONERROR 4 	// Error registering the XMI file.

	extern short	 cdecl far RegisterXmidiFile(char far *fname); // Register by filename.

	extern void  cdecl far PollMidPak(void); // Poll MidPak for music processing.

	extern long int  cdecl far MidPakClock(void); // Return MIDPAK heartbeat count (120hz)

	extern long int far * cdecl far MidPakClockAddress(void); // Return address of midpak clock.

	extern short far *	cdecl far TriggerCountAddress(void); // Report address of trigger count.

	extern short far *	cdecl far EventIDAddress(void); // Report address of event id.

	extern short	cdecl far ReportSequenceNumber(void);

	extern short	cdecl far InitMP(short midpakseg,short advseg,char	far *ad); // Init MIDI driver.
	extern void cdecl far DeInitMP(short midpakseg); // Unload a previously loaded sound driver.

	// doscalls.h starts here:
	#define NEW_FILE 0 // MFOPEN file types, 0 open as a new file for read/write
	#define OLD_FILE 1 // file type 1, open as an old file for read/write access
	#define FILE_NOT_FOUND 0 // file handle of zero, is file not found.

	int  far cdecl mfopen(char far *filename,long int far *size,int type);
	int  far cdecl mpfopen(char far *filename,long int far *size,int type);
	/******************************************************************************/
	/* mfopen -> Does a DOS file open.  Returns a DOS file handle, which is simply*/
	/*	     an integer.  You can open a file as a new file or as an old file */
	/*	     by specifying the file type.  A return code of zero means that   */
	/*	     the file couldn't be opened.  You pass the address of a long int */
	/*	     who's contents will be filled with the total length of the file. */
	/*	     If you pass a null address then the size will not be reported.   */
	/*	     After a file open, the file pointer will always be sitting at    */
	/*	     byte position zero.					      */
	/******************************************************************************/

	int  far cdecl mfclose(int fhand);
	int  far cdecl mpfclose(int fhand);
	/******************************************************************************/
	/* mfclose -> Close a file that was opened with mfopen. 		      */
	/******************************************************************************/

	long int	far cdecl mfpos(int fhand);
	/******************************************************************************/
	/* mfpos -> report current file position of this file.			      */
	/******************************************************************************/

	long int far cdecl mfseek(int fhand,long int fpos);
	/******************************************************************************/
	/* mfseek -> seek file to this position passed.  Return code is actual file   */
	/*	     seek position achieved. (In case request went past end of file.) */
	/******************************************************************************/

	int  far cdecl mfread(void far *address,long int size,int fhand);
	/******************************************************************************/
	/* mfread -> read from file, into address, for length of size, from fhand.    */
	/*	     return code of 1, successful file read, return code of zero,     */
	/*	     file read failed.						      */
	/******************************************************************************/

	int  far cdecl mfwrite(void far *address,long int size,int fhand);
	/******************************************************************************/
	/* mfwrite -> write data to file, from address, for length of size, to fhand. */
	/*	      return code of 1, success, return code of zero, write failed.   */
	/******************************************************************************/

	char far * far cdecl fload(char far *name,long int far *siz);
	char far * far cdecl fpload(char far *name,long int far *siz);
	/******************************************************************************/
	/* fload -> allocate memory, and read entire file in.  Uses name as filename  */
	/*	    and returns the length read in, in siz.  If siz is null then siz  */
	/*	    not set.  If return code is NULL then was unable to load file.    */
	/*	    Either the file wasn't found, or there wasn't enough memory to    */
	/*	    read it in.  Otherwise return code is the address of the file     */
	/*	    read in at.  Uses MEMALLOC (provided by application program) to   */
	/*	    allocate memory, and caller must do MEMFREE when finished with    */
	/*	    this memory.						      */
	/******************************************************************************/

	char far * far cdecl floadpara(char far *name,long int far *siz,int far *segment);
	/******************************************************************************/
	/* floadpara -> a special version of fload, that reads in the file into       */
	/*		allocated memory, but forces it at a paragraph boundary.      */
	/*		The return code is still the address of allocated memory for  */
	/*		the file read, but the variable segment is loaded with the    */
	/*		actual segment boundary that the file was read in at.  This   */
	/*		is used by digplay's LoadDriver call, which loads a binary    */
	/*		image into memory, that must fall on a paragraph boundary.    */
	/******************************************************************************/

	int  far cdecl keystat(void);
	/******************************************************************************/
	/* keystat-> report DOS key status.  Zero, no key pending, 1, key pending.    */
	/******************************************************************************/

	int  far cdecl getkey(void);
	/******************************************************************************/
	/* getkey -> DOS getkey function. Returns keypress pending.  Automatically    */
	/*	     handles extended key codes, by adding 256 to them. 	      */
	/******************************************************************************/

	void far cdecl farcop(char far *dest,char far *source);
	/******************************************************************************/
	/* farcop -> string copy routine, but uses far pointers.		      */
	/******************************************************************************/

	void far cdecl farcat(char far *dest,char far *source);
	/******************************************************************************/
	/* farcat -> string concatenate routine, but with far pointers. 	      */
	/******************************************************************************/

	int far cdecl farlen(char far *string); // Return length of string.

	int far cdecl farcompare(char far *source,char far *dest); // String compare.

	void far cdecl ucase(char far *string); // Upper case a string.

	char far * far cdecl fmalloc(long int size);
	/******************************************************************************/
	/* fmalloc -> DOS memory allocation.  Works fine by itself but conflicts with */
	/*	      C compiler's far memory allocation.  DOS likes memory to be     */
	/*	      de-allocated in the order that it was originally allocated, in  */
	/*	      order for it to cleanly defragment memory pools.	These function*/
	/*	      calls are valid if you are writing a TSR or must do DOS memory  */
	/*	      allocation exclusively.					      */
	/******************************************************************************/

	void far cdecl ffree(char far *tmp);
	/******************************************************************************/
	/* ffree -> free dos allocated memory.					      */
	/******************************************************************************/

	void far cdecl writeln(char far *string);
	/******************************************************************************/
	/* writeln -> echo a string to the console.  Avoids dragging all of the printf*/
	/*	      library code, which is HUGE!				      */
	/******************************************************************************/

	// This installs a timer interupt at a particular service rate.  The callback,
	// goes to a valid C routine.  Will save and restore all registers, handle
	// chaining the interrupt vector, and sets up DS addressability and an
	// independent stack.  The stack is very small, so your C routine should use
	// stack variables at an abolute minimum.
	// Returns a far pointer to a long integer clock tick counter.
	long int far * far cdecl InstallServiceRoutine(void far *address,unsigned int divisor);
	// Puts hardware interrupt back the way it belongs.
	void far cdecl RestoreServiceRoutine(void);
	void far * far cdecl ReportTimerService(void); // report far address of
	// installed hardware interrupt.

	int far cdecl mdelete(char far *fname); // Delete a file by filename.
	int far cdecl ifexists(char far *fname);	// Does this file exist? 1 yes, 0 no.


	#define BLACK 0
	#define BLUE 1
	#define GREEN 2
	#define CYAN 3
	#define RED 4
	#define MAGENTA 5
	#define BROWN 6
	#define GRAY 7
	#define GREY 7
	#define DARK_GREY 8
	#define DARK_GRAY 8
	#define LIGHT_BLUE 9
	#define LIGHT_GREEN 10
	#define LIGHT_CYAN 11
	#define LIGHT_RED 12
	#define LIGHT_MAGENTA 13
	#define YELLOW 14
	#define WHITE 15

	#define BEHIND << 4 |

	void far cdecl tprint(int x,int y,int len,char far *string,int color);
	void far cdecl tcolor(int x,int y,int len,int color);  // Change text background color.
	void far cdecl TextCursor(int xloc,int yloc);

	void far cdecl farmov(char far *dest,char far *source,int length);

	void far cdecl MemStosb(char far *dest,int value,int length);

	int far cdecl scale(int a,int b,int c);

	void far * far cdecl MakeFP(int offset,int segment);
	int far cdecl MakeSeg(char far *address);

	void far cdecl DBUG(long int value,int xloc,int yloc);

	int far cdecl InWindows(void); // Return's true if in windows dos box.

	// detect.h starts here:
	#define TRUE 1
	#define FALSE 0

	int detect_settings(int *baseio, int *irq, int *dma, int *dma16);
	  /* Detects sound card settings using BLASTER environment variable */
	  /* Parameters:                                                    */
	  /*   baseio    Sound card base IO address                         */
	  /*   irq       Sound card IRQ                                     */
	  /*   dma       Sound card 8-bit DMA channel                       */
	  /*   dma16     Sound card 16-bit DMA channel (0 if none)          */
	  /* Returns:                                                       */
	  /*   TRUE      Sound card settings detected successfully          */
	  /*   FALSE     Sound card settings could not be detected          */

	// smix.h starts here:
	#define TRUE  1
	#define FALSE 0

	#define ON  1
	#define OFF 0

	typedef struct
   {
   	int xmshandle;
      long startofs;
      long soundsize;
   } SOUND;

	int  init_sb(int baseio, int irq, int dma, int dma16);
   /* Initializes control parameters, resets DSP and installs int. handler   */
	/*  Parameters:                                                           */
	/*   baseio    Sound card base IO address                                 */
	/*   irq       Sound card IRQ setting                                     */
	/*   dma       Sound card 8-bit DMA channel                               */
	/*   dma16     Sound card 16-bit DMA channel                              */
	/*  Returns:                                                              */
	/*   TRUE      Sound card successfully initialized                        */
	/*   FALSE     Sound card could not be initialized                        */

	void shutdown_sb(void); /* Removes interrupt handler and resets DSP       */


	void set_sampling_rate(unsigned short rate);
	  /* Overrides default sampling rate set with SAMPLING_RATE constant        */
	  /*  Parameters:                                                           */
	  /*   Rate:     New sampling rate (will be rounded by sound card)          */
	  /* This function can either be called before calling InitMixing (after    */
	  /* calling InitSB) to change the sampling rate before playback begins or  */
	  /* called during playback to change the rate dynamically.  The lowest     */
	  /* sampling rate that will work is roughly 5000 HZ.  The highest sampling */
	  /* rate that will work on all sound cards is 22050 HZ.  If you only want  */
	  /* to support the SB16, then you can use rates all the way up to 48000 HZ */


	void init_mixing(void);
	  /* Allocates internal buffers and starts digitized sound output           */

	void shutdown_mixing(void);
	  /* Deallocates internal buffers and stops digitized sound output          */


	int  init_xms(void);
	  /* Initializes extended memory driver                                     */
	  /*  Returns:                                                              */
	  /*   TRUE      Extended memory driver successfully initialized            */
	  /*   FALSE     Extended memory driver could not be initialized            */

	unsigned short getfreexms(void);
	  /* Returns amount of free extended memory (In kilobytes)                  */


	void init_sharing(void);
	  /* Allocates an EMB that all sound data will be stored in.  Using this    */
	  /* will preserve extended memory handles, which are scarce resources.     */
	  /* Call this on initialization and all sounds will automatically be       */
	  /* stored in one EMB.  You can call load_sound as usual to allocate a     */
	  /* sound, but free_sound will only deallocate the sound data structure.   */
	  /* You must call shutdown_sharing before program termination in order     */
	  /* to free all allocated extended memory.                                 */

	void shutdown_sharing(void);
	  /* Shuts down EMB sharing and frees shared EMB block                      */


	int open_sound_resource_file(char *filename, int offst);
	  /* Opens a resource file for loading sounds.  After this has been called, */
	  /* the Key parameter in the LoadSound function is used as a resource key  */
	  /* to locate the sound data in this file.                                 */
	  /*  Parameters:                                                           */
	  /*   FileName: File name of resource file                                 */
	  /*  Returns:                                                              */
	  /*   TRUE      Sound resource file exists                                 */
	  /*   FALSE     Error, sound resource file does not exist                  */


	void close_sound_resource_file(void);
	  /* Close sound resource file.  If you have called this, the Key parameter */
	  /* to the LoadSound function will act as a filename instead of a resource */
	  /* key.                                                                   */


	int load_sound(SOUND **sound, char *key);
	  /* Allocates an memory buffer and loads a sound from a file               */
	  /*  Parameters:                                                           */
	  /*   sound     Pointer to pointer to unallocated sound data structure     */
	  /*   key       If a resource file has been opened then key is a resource  */
	  /*             identifier.  Use the same ID as you used when adding the   */
	  /*             sound resource with SNDLIB.  If a resource file is not     */
	  /*             opened, then key is the filename from which to load the    */
	  /*             sound data.                                                */
	  /*  Returns:                                                              */
	  /*   TRUE      Sound loaded successfully                                  */
	  /*   FALSE     Error loading sound                                        */

	void free_sound(SOUND **sound);
	  /* Frees sound data structure and extended memory block                   */
	  /*  Parameters:                                                           */
	  /*   sound     Pointer to pointer to allocated sound data structure       */

	int start_sound(SOUND *sound, int index, int loop);
	  /* Starts playing a sound                                                 */
	  /*  Parameters:                                                           */
	  /*   sound     Pointer to sound data structure                            */
	  /*   index     A number to keep track of the sound with (Used to stop it) */
	  /*   loop      Indicates whether sound should be continuously looped      */
	  /*  Returns:                                                              */
	  /*   TRUE      Sound started successfully                                 */
	  /*   FALSE     Sound not started (no free channels)                       */

	void stop_sound(int index);
	  /* Stops playing a sound                                                  */
	  /*  Parameters:                                                           */
	  /*   index     Index of sound to stop (All with given index are stopped)  */

	int  sound_playing(int index);
	  /* Checks if a sound is still playing                                     */
	  /*  Parameters:                                                           */
	  /*   index     Index used when the sound was started                      */
	  /*  Returns:                                                              */
	  /*   TRUE      At least one sound with the specified index is playing     */
	  /*   FALSE     No sounds with the specified index are playing             */



	extern volatile long  intcount;       /* Current count of sound interrupts  */
	extern volatile int   voicecount;     /* Number of voices currently in use  */

	extern short dspversion;
	extern int   autoinit;
	extern int   sixteenbit;
}

extern int baseio, irq, dma, dma16;  // blaster environment variable
extern char SFX; // sound f-x flag
extern char Blaster_used;  // flag for whether sb was used

void init_smix_form_1(void);     // basic forms of smix processing
void shutdown_smix_form_1(void);

void init_midi_form_1(char *XMI_filename); // form to init midi
void shutdown_midi_form_1(void);  // form to shutdown midi
void start_sequence(int seq_num, char repeat);  // to play sequence... with repeats
