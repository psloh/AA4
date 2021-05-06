#include <iostream.h>     // cout
#include <conio.h>        // getch()
#include <stdlib.h>       // exit()

#include "PxmsV111.H"       // my adapted XMS unit

#include "Aa4xms.H"    // this module's header file
#include "Aa4GDat.H"   // game data



extern XMSObject X;          // externally declared xms object
extern GameData GDat;   // xms handle for pictures


void WriteInfo(void)
// Pre: Text mode.
// Post: XMS data output.
{
	cout << "- - - - - - - - - - - - - -\n";   /*
	cout << "XMS driver version:   " << X.GetXMSVersionStr() << "\n";
	cout << "XMS driver internal:  " << X.GetXMSRevisionStr() << "\n";  */
	cout << "Free XMS memory:      " << X.GetFreeXMS() << "k" << "\n";
	cout << "Largest XMS block:    " << X.GetLargestXMSBlock() << "k" << "\n";
	cout << "- - - - - - - - - - - - - -\n";
}

void Install_xms(void)
// Pre: None.
// Post: XMS driver installed.
{
	if (X.XMSInst())                           // if driver installed
	{
   	X.InitXMS();                            // init it.
		cout << "XMS driver is installed\n";
   }
   else
	{
		cout << "No XMS driver installed!\n";
		exit(1);    // quit
	}
   WriteInfo();                               // output xms data
   if (X.GetFreeXMS() == 0)       // if not enough xms
	{
   	cout << "Not enough XMS available!  Terminating!\n";
		exit(1);
	}


	if (X.AllocateEMB(&GDat.xms_pic_handle, 100) == 0)   // try to allocate the 100 KB picture block
   {
   	cout << "Insufficient free XMS.\n";
   	exit(1);
   }
}

void Uninstall_xms(void)
// Pre: XMS installed.
// Post: XMS freed.
{
	X.FreeEMB(&GDat.xms_pic_handle);
}
