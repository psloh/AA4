/*****************************************************************************/
/* MIDPAK.H 	C prototype header for API MIDPAK link layer interface				 */
/*						MIDPAK.ASM.  Because MIDPAK is a real-mode program all				 */
/*						addresses passed must be in the first 1mb of address space. 	 */
/*																																					 */
/*						See MIDPKAPI.DOC for complete MIDPAK documentation. 					 */
/*																																					 */
/*		Written by John W. Ratcliff (c) 1994																	 */
/*			 Compuserve: 70253,3237 																						 */
/*			 Genie: J.RATCLIFF3 																								 */
/*			 BBS: 1-314-939-0200																								 */
/*			 Addresss:																													 */
/*					 747 Napa Lane																									 */
/*					 St. Charles, MO 63304																					 */
/*																																					 */
/*		A $500 per product license fee applies to all commercial software 		 */
/*		products distributed with ANY MIDPAK drivers. 												 */
/*																																					 */
/*		To pay a license, simply write a check for $500 payable to						 */
/*				The Audio Solution, 747 Napa Lane, St. Charles, MO 63304					 */
/*				with a copy of your commerical product.  You will receive a signed */
/*				license agreement from The Audio Solution shortly thereafter. 		 */
/*				This license fee applies specifically to the inclusion with your	 */
/*				distribution disk any of the MIDPAK drivers from The Audio Solution*/
/*				These drivers are copyrighted works, created by me, to enhance the */
/*				use of sound and music in DOS based commercial software.	The 		 */
/*				license fees collected are used to maintain the drivers and keep	 */
/*				the BBS running.	There is a seperate license fee for the use 		 */
/*				and distribution of DIGPAK drivers. 															 */
/*																																					 */
/*				See accompaning documentation regarding license fees for DIGPAK 	 */
/*				distribution.  You would be ill-advised to distribute a commercial */
/*				product containing either DIGPAK and/or MIDPAK drivers without		 */
/*				having paid the distribution license fee.  Since your product would*/
/*				contain unlicensed copyrighted software from The Audio Solution,	 */
/*				your product could be required to be immediately removed from retail*/
/*				distribution.  I doubt this is going to be a problem.  Clearly if  */
/*				your product is enhanced by the use of these drivers, your company */
/*				can easily afford a nominal license fee of $500 in exchange for 	 */
/*				getting the use of several man-years of software engineering			 */
/*				resources.																												 */
/*****************************************************************************/
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
