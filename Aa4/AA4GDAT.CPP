#include <fstream.h>    // files
#include <alloc.h>      // allocating memory
#include <stdlib.h>     // exit()

#include "PxmsV111.H"   // xms library

#include "Aa4GDat.H"    // Game data
#include "Aa4Exps.H"    // Explosions
#include "Aa4Shop.H"    // Shop

extern GraphObject G;   // external graphics object
extern XMSObject X;     // external xms object
int mem_size;    // mem counter
char shop_file[20], store_file[20];           // filenames

void LoadGameData(GameData &GDat, Explosions &XP)
// load game data
{
	ifstream IniFile("AA4.INI");   // ini file
	char filename[301];
	void *img;      // image
	int i, j;       // counters
	int explode;    // temp explode variable.

	GDat.ys_pic_offst = 0;   // set XMS offset of your ship's picture to be zero.

/* In the next code segment, the Inifile, Gamefile, and Graphicsfile will be
	parsed.  The little tags after the  while..ignore statements tell what the
	input means.  [i.e. what is being read from the files]  */

	while (IniFile.peek() != '=')  IniFile.ignore();      // GAME DAT=
	IniFile.ignore();
	IniFile.getline(filename, 300);

	ifstream GamFile(filename);
	while (IniFile.peek() != '=')  IniFile.ignore();      // NUM LEVELS=
	IniFile.ignore();
	IniFile >> GDat.max_level;
	IniFile.close();

// ---------------------------  GAME FILE  --------------------------------
//            ;general
	while (GamFile.peek() != '=')  GamFile.ignore();   // SPEED MODE=
	GamFile.ignore();
	GamFile >> GDat.speed_mode;

//            ;yship
	while (GamFile.peek() != '=')  GamFile.ignore();   // XDIM=
	GamFile.ignore();
	GamFile >> GDat.ys_xdim;      // get xdim of ship

	while (GamFile.peek() != '=')  GamFile.ignore();   // YDIM=
	GamFile.ignore();
	GamFile >> GDat.ys_ydim;      // get ydim of ship

	while (GamFile.peek() != '=')  GamFile.ignore();   // GRAPHICS FILE=
	GamFile.ignore();
	GamFile.getline(filename, 299);    // get game.dat graphics file name.

	ifstream Gfile(filename);   // open graphics file
	GDat.ys_isize = G.imagesize(1, 1, GDat.ys_xdim, GDat.ys_ydim);      // compute image size for yship
   mem_size = (GDat.ys_isize+1)/2 * 2;
	img = malloc(mem_size);    // allocate the memory

	Gfile.read((char*)img, GDat.ys_isize);         // AND mask
	if (X.CopyToEMB(img, GDat.ys_isize, GDat.xms_pic_handle, GDat.ys_pic_offst) == 0)   // copy AND mask into xms.
   {
   	cerr << "XMS copy-to error (copying yship)";
 		exit(1);
   }

	Gfile.read((char*)img, GDat.ys_isize);         // XOR mask
	if (X.CopyToEMB(img, GDat.ys_isize, GDat.xms_pic_handle, GDat.ys_pic_offst+mem_size) == 0)   // copy XOR mask into xms.
	{
		cerr << "XMS copy-to error (copying yship)";
		exit(1);
	}

	GDat.next_offst = GDat.ys_pic_offst + 2*mem_size;   // update status of memory management routines

	free(img);             // free allocated low-memory for image

//             ;projectiles
	while (GamFile.peek() != '=')   GamFile.ignore();   // NUMBER OF PROJECTILES=
	GamFile.ignore();
	GamFile >> GDat.num_yprojs;

	for (i = 0; i < GDat.num_yprojs; i++)
	{
		while (GamFile.peek() != '=')   GamFile.ignore();   // XDIM=
		GamFile.ignore();
		GamFile >> GDat.yprojectiles[i].xdim;

		while (GamFile.peek() != '=')   GamFile.ignore();   // YDIM=
		GamFile.ignore();
		GamFile >> GDat.yprojectiles[i].ydim;

		GDat.yprojectiles[i].isize = G.imagesize(1, 1, GDat.yprojectiles[i].xdim, GDat.yprojectiles[i].ydim);
      mem_size = (GDat.yprojectiles[i].isize+1)/2 * 2;
		img = malloc(mem_size);
		Gfile.read((char*)img, GDat.yprojectiles[i].isize);      // AND mask
		GDat.yprojectiles[i].offset = GDat.next_offst;   // update status of memory management routines
		GDat.next_offst += mem_size;   // update status of memory management routines
		if (X.CopyToEMB(img, GDat.yprojectiles[i].isize, GDat.xms_pic_handle, GDat.yprojectiles[i].offset) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying projectile AND image)";
			exit(1);
		}
		Gfile.read((char*)img, GDat.yprojectiles[i].isize);      // XOR mask
		GDat.next_offst += mem_size;   // update status of memory management routines
		if (X.CopyToEMB(img, GDat.yprojectiles[i].isize, GDat.xms_pic_handle, GDat.yprojectiles[i].offset+mem_size) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying projectile XOR image)";
			exit(1);
		}
		free(img);

		while (GamFile.peek() != '=')   GamFile.ignore();   // SPEED=
		GamFile.ignore();
		GamFile >> GDat.yprojectiles[i].speed;

		while (GamFile.peek() != '=')   GamFile.ignore();   // EXPLODE=
		GamFile.ignore();
		GamFile >> explode;

		while (GamFile.peek() != '=')   GamFile.ignore();   // EXPLOSION RADIUS=
		GamFile.ignore();
		GamFile >> GDat.yprojectiles[i].radius;
		if (explode)
			GDat.yprojectiles[i].radius = 0;

		while (GamFile.peek() != '=')   GamFile.ignore();   // HITPOINTS=
		GamFile.ignore();
		GamFile >> GDat.yprojectiles[i].hitpoints;
	}

//             ;weapons
	while (GamFile.peek() != '=')   GamFile.ignore();   // NUMBER OF WEAPONS=
	GamFile.ignore();
	GamFile >> GDat.num_yweaps;

	for (i = 0; i < GDat.num_yweaps; i++)
	{
		while (GamFile.peek() != '=')   GamFile.ignore();   // NUM PROJS SHOT=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].num_projs_shot;

		for (j = 0; j < GDat.yweapons[i].num_projs_shot; j++)
		{
			while (GamFile.peek() != '=')   GamFile.ignore();   // PROJECTILE=
			GamFile.ignore();
			GamFile >> GDat.yweapons[i].proj[j];

			while (GamFile.peek() != '=')   GamFile.ignore();   // X OFF=
			GamFile.ignore();
			GamFile >> GDat.yweapons[i].x_off[j];

			while (GamFile.peek() != '=')   GamFile.ignore();   // Y OFF=
			GamFile.ignore();
			GamFile >> GDat.yweapons[i].y_off[j];

			while (GamFile.peek() != '=')   GamFile.ignore();   // THETA=
			GamFile.ignore();
			GamFile >> GDat.yweapons[i].theta[j];
		}

		while (GamFile.peek() != '=')   GamFile.ignore();   // REFIRE RATE=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].refire;

		while (GamFile.peek() != '=')   GamFile.ignore();   // POWER=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].power;

		while (GamFile.peek() != '=')   GamFile.ignore();   // XDIM=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].xdim;

		while (GamFile.peek() != '=')   GamFile.ignore();   // YDIM=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].ydim;

		GDat.yweapons[i].isize = G.imagesize(1, 1, GDat.yweapons[i].xdim, GDat.yweapons[i].ydim);
      mem_size = (GDat.yweapons[i].isize+1)/2 * 2;
		img = malloc(mem_size);
		Gfile.read((char*)img, GDat.yweapons[i].isize);      // AND mask
		GDat.yweapons[i].offset = GDat.next_offst;     // update status of memory management routines
		GDat.next_offst += mem_size;     // update status of memory management routines
		if (X.CopyToEMB(img, GDat.yweapons[i].isize, GDat.xms_pic_handle, GDat.yweapons[i].offset) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying weapon AND image)";
			exit(1);
		}
		Gfile.read((char*)img, GDat.yweapons[i].isize);      // XOR mask
		GDat.next_offst += mem_size;     // update status of memory management routines
		if (X.CopyToEMB(img, GDat.yweapons[i].isize, GDat.xms_pic_handle, GDat.yweapons[i].offset+mem_size) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying weapon XOR image)";
			exit(1);
		}
		free(img);

		while (GamFile.peek() != '=')   GamFile.ignore();   // LINES=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].lines;
		GamFile.ignore();

		for (j = 0; j < GDat.yweapons[i].lines; j++)
			GamFile.getline(GDat.yweapons[i].descrip[j], 44);

		while (GamFile.peek() != '=')   GamFile.ignore();   // PRICE=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].price;

		while (GamFile.peek() != '=')   GamFile.ignore();   // PRIMARY=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].primary;

		while (GamFile.peek() != '=')   GamFile.ignore();   // SOUND NUM=
		GamFile.ignore();
		GamFile >> GDat.yweapons[i].sound_num;
	}

//             ;explosions
	while (GamFile.peek() != '=')   GamFile.ignore();   // SPACING X=
	GamFile.ignore();
	GamFile >> XP.xdim;

	while (GamFile.peek() != '=')   GamFile.ignore();   // SPACING Y=
	GamFile.ignore();
	GamFile >> XP.ydim;

	while (GamFile.peek() != '=')   GamFile.ignore();   // NUMBER OF STAGES=
	GamFile.ignore();
	GamFile >> XP.num_stages;

	exp_pic_ptr xp_ins;
	XP.stages = new exp_pic;
	XP.stages->next = XP.stages->prev = XP.stages;

	for (i = 0; i < XP.num_stages; i++)
	{
		xp_ins = new exp_pic;

		while (GamFile.peek() != '=')   GamFile.ignore();   // XDIM=
		GamFile.ignore();
		GamFile >> xp_ins->xdim;

		while (GamFile.peek() != '=')   GamFile.ignore();   // YDIM=
		GamFile.ignore();
		GamFile >> xp_ins->ydim;

		xp_ins->isize = G.imagesize(1, 1, xp_ins->xdim, xp_ins->ydim);
      mem_size = (xp_ins->isize+1)/2 * 2;
		img = malloc(mem_size);
		Gfile.read((char*)img, xp_ins->isize);      // AND mask
		xp_ins->offset = GDat.next_offst;     // update status of memory management routines
		GDat.next_offst += mem_size;     // update status of memory management routines
		if (X.CopyToEMB(img, xp_ins->isize, GDat.xms_pic_handle, xp_ins->offset) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying explosion AND image)";
			exit(1);
		}
		Gfile.read((char*)img, xp_ins->isize);      // XOR mask
		GDat.next_offst += mem_size;     // update status of memory management routines
		if (X.CopyToEMB(img, xp_ins->isize, GDat.xms_pic_handle, xp_ins->offset+mem_size) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying explosion XOR image)";
			exit(1);
		}
		free(img);

      xp_ins->prev = XP.stages->prev;
      xp_ins->next = XP.stages;
      xp_ins->prev->next = xp_ins;
      xp_ins->next->prev = xp_ins;
	}

//             ;goodies
	while (GamFile.peek() != '=')   GamFile.ignore();   // NUMBER OF GOODIES=
	GamFile.ignore();
	GamFile >> GDat.num_ygoodies;

	for (i = 0; i < GDat.num_ygoodies; i++)
	{
		while (GamFile.peek() != '=')   GamFile.ignore();   // CASH=
		GamFile.ignore();
		GamFile >> GDat.ygoodies[i].cash;

		while (GamFile.peek() != '=')   GamFile.ignore();   // SHIELD=
		GamFile.ignore();
		GamFile >> GDat.ygoodies[i].hitpoints;

		while (GamFile.peek() != '=')   GamFile.ignore();   // WEAPON=
		GamFile.ignore();
		GamFile >> GDat.ygoodies[i].weapon;

		while (GamFile.peek() != '=')   GamFile.ignore();   // XDIM=
		GamFile.ignore();
		GamFile >> GDat.ygoodies[i].xdim;

		while (GamFile.peek() != '=')   GamFile.ignore();   // YDIM=
		GamFile.ignore();
		GamFile >> GDat.ygoodies[i].ydim;

		GDat.ygoodies[i].isize = G.imagesize(1, 1, GDat.ygoodies[i].xdim, GDat.ygoodies[i].ydim);
      mem_size = (GDat.ygoodies[i].isize+1)/2 * 2;
		img = malloc(mem_size);
		Gfile.read((char*)img, GDat.ygoodies[i].isize);      // AND mask
		GDat.ygoodies[i].offset = GDat.next_offst;   // update status of memory management routines
		GDat.next_offst += mem_size;   // update status of memory management routines
		if (X.CopyToEMB(img, GDat.ygoodies[i].isize, GDat.xms_pic_handle, GDat.ygoodies[i].offset) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying goody AND image)";
			exit(1);
		}
		Gfile.read((char*)img, GDat.ygoodies[i].isize);      // XOR mask
		GDat.next_offst += mem_size;   // update status of memory management routines
		if (X.CopyToEMB(img, GDat.ygoodies[i].isize, GDat.xms_pic_handle, GDat.ygoodies[i].offset+mem_size) == 0)   // copy image into xms.
		{
			cerr << "XMS copy-to error (copying goody XOR image)";
			exit(1);
		}
		free(img);
	}

//     ;shop definition file
   while (GamFile.peek() != '=')  GamFile.ignore();   // SHOP DEF FILE=
	GamFile.ignore();
	GamFile.getline(shop_file, 19);    // get shop.po graphics file name.

//     ;store def file
	while (GamFile.peek() != '=')  GamFile.ignore();   // STORE DEF FILE=
	GamFile.ignore();
	GamFile.getline(store_file, 19);    // get store.po graphics file name.

	Gfile.close();         // close graphics file
	GamFile.close();       // close game.dat file
}


