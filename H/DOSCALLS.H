/******************************************************************************/
/* DOSCALLS.ASM, DOSCALLS.H						      */
/* Written by John W. Ratcliff, December 1991.				      */
/******************************************************************************/
/* DOSCALLS is a linkable object module that provides many of the functions   */
/* available through the standard C libraries.	There are number of reasons to*/
/* use DOSCALLS.OBJ rather than C library function calls.		      */
/*									      */
/*	 1. Portability: These functions work the same regardless of what     */
/*			 C compiler you are using.			      */
/*									      */
/*	 2. Size:	 These functions are extremely small compared to      */
/*			 equivalent C library functions. The C library	      */
/*			 functions are general while the DOSCALLS	      */
/*			 function calls are very specific.  Accessing C       */
/*			 library function calls causes large amounts of       */
/*			 library code to get drug in that you probably	      */
/*			 don't want.                                          */
/*									      */
/*	 3. TSR capability: If you program TSR's you will find that           */
/*			 dragging in C library functions will kill you	      */
/*			 for a whole number of reasons.  Using		      */
/*			 DOSCALLS.OBJ you have no DGROUP dependencies and     */
/*			 you can easily write single segment TSR's and        */
/*			 even write both COM modules and DOS device	      */
/*			 drivers.					      */
/*									      */
/*	 4. Assembly language:	If you don't program in C and need basic      */
/*			 DOS support functions, you can just use DOSCALLS     */
/*			 for this purpose.				      */
/*									      */
/*	 5. Simplicity:  DOSCALLS don't ignore the fact that you are programming*/
/*			 on an MS-DOS machine.	They in fact simply provide   */
/*			 C callable access to INT 21h calls.  Because of this */
/*			 they are all easy to use.			      */
/******************************************************************************/
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
