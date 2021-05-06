#include <conio.h>
#include <ctype.h>
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <alloc.h>
#include <stdio.h>
#include <dos.h>

#include "PMseV101.H"
#include "PGphV120.H"

void ShowHelp(void);
void Use(char *filename);
void process(int &row, int &col, int &quit, int xstart, int xdim, int ydim);
void procpalette(int row, int col);
void procgrid(int row, int col, int xstart);
void chekerase(int row, int col, int xstart, int xdim, int ydim);
char exists(char *filename);
void LoadDim(char *filename, int &xdim, int &ydim);
void LoadPic(char *filename, int xstart);
void Xout(int xs, int ys, int xe, int ye);

GraphObject G;
char mse_hide = 0;

int main(int ParamCount, char *ParamStr[])

{
	if (ParamCount != 2)
   	ShowHelp();
   else
   	Use(ParamStr[1]);
      
   return 0;
}

void ShowHelp(void)
{
	clrscr();
   cout << "===================== HELP! =====================" << "\n";
   cout << "Use:          TILEMK2  <filename>                " << "\n";
   cout << "  <filename>     :            Name of output file" << "\n";
   cout << "=================================================" << "\n";
   cout << "\n\n\n";
   cout << "Press a key...";
   getch();
}

void Use(char *filename)
{
   int xdim, ydim;
   int startx;
   MouseObject M;
   int quit = 0;
   char rt=0, md=0, lt=0;
   int row, col;

   if (exists(filename)) LoadDim(filename, xdim, ydim);
   else
   {
   	cout << "X-dimension (pixels): ";
   	cin >> xdim;
   	cout << "Y-dimension (pixels): ";
   	cin >> ydim;

   	while (kbhit())
			getch();
	}

   G.initgraph();

   G.setcolor(DARKGRAY);                  // 8 X 32
   for (int x = 5; x <= 37; x += 4)
   	G.vline(x, 5, x, 133);
   for (int y = 5; y <= 133; y += 4)
   	G.hline(5, y, 37, y);
   Xout(6, 6, 8, 8);
   G.setcolor(DARKGRAY);

   G.rectangle(5, 135, 5 + xdim + 1, 135 + ydim + 1);

   G.rectangle(315, 0, 319, 4);
   Xout(316, 1, 318, 3);
   G.setcolor(DARKGRAY);

   if (5 + ydim*4 > 135)
   	startx = 10 + max(32, xdim+2);
   else
   	startx = 42;
   G.rectangle(startx, 5, startx + xdim*4, 5 + ydim*4);
   for (int y = 9; y < 5 + ydim*4; y += 4)
   	G.hline(startx, y, startx + xdim*4, y);
   for (int x = startx + 4; x < startx + xdim*4; x += 4)
   	G.vline(x, 5, x, 5 + ydim*4);

   for (int col = 1; col < 256; col++)
   {
   	G.setfillcolor(char(col));
      G.bar(col/32*4 + 6, col%32*4 + 6, col/32*4 + 8, col%32*4 + 8);
   }

   if (exists(filename)) LoadPic(filename, startx);

   G.setcolor(DARKGRAY);                  //OK button
   G.rectangle(5, 0, 13, 4);
   G.putpixel(6, 2, LIGHTGREEN);
   G.putpixel(7, 1, LIGHTGREEN);
   G.putpixel(7, 3, LIGHTGREEN);
   G.putpixel(8, 2, LIGHTGREEN);
   G.setcolor(LIGHTGREEN);
   G.vline(10, 1, 10, 3);
   G.putpixel(11, 2, LIGHTGREEN);
   G.putpixel(12, 1, LIGHTGREEN);
   G.putpixel(12, 3, LIGHTGREEN);
   G.setcolor(BLACK);

   M.ColdReset();
   M.SoftReset();
   M.VertLimit(0, 199);
   M.HorizLimit(0, 638);

	int Orow = -1, Ocol = -1, Ocolor = 0;
   char pixcolor;
   while (!quit)
   {
   	M.GetClicks(rt, md, lt, row, col);
      col = col/2;
		if (row != Orow || col != Ocol)
      {
         if (!mse_hide)
         	G.putpixel(Ocol, Orow, Ocolor);
         else
         	mse_hide = 0;
         Orow = row;
         Ocol = col;
         pixcolor = G.getpixel(Ocol, Orow);
         Ocolor = pixcolor;
         G.putpixel(Ocol, Orow, WHITE);
      }
      if (mse_hide)
      {
      	mse_hide = 0;
         Ocolor = G.getpixel(Ocol, Orow);
         G.putpixel(Ocol, Orow, WHITE);
      }
      if (lt)  process(row, col, quit, startx, xdim, ydim);
      if (rt)  chekerase(row, col, startx, xdim, ydim);
   }

   ofstream outf(filename);
   void *bitmap;

   int isize = G.imagesize(6, 136, 5 + xdim, 135 + ydim);
   bitmap = farmalloc(isize);
   G.getimage(6, 136, 5 + xdim, 135 + ydim, bitmap);
   outf.write((char*)bitmap, isize);
   farfree(bitmap);

   G.closegraph();
}

void process(int &row, int &col, int &quit, int xstart, int xdim, int ydim)
{
	if (row >= 0 && row <= 4 && col >= 5 && col <= 13)
   	quit = 1;
   else if (row >= 5 && row <= 133 && col >= 5 && col <= 37)
   	procpalette(row, col);
   else if (row >= 5 && row <= 5 + ydim*4 && col >= xstart && col <= xstart + xdim*4)
		procgrid(row, col, xstart);
}

void procpalette(int row, int col)
{
	if ((col % 4 != 1) && (row % 4 != 1))
	{
      int color;

      color = (row - 5)/4 + (col - 5)/4*32;
      G.setcolor(color);
      G.setfillcolor(G.getcolor());
    	G.bar(316, 1, 318, 3);
      if (G.getcolor() == BLACK)
      	Xout(316, 1, 318, 3);
   }
}

void procgrid(int row, int col, int xstart)
{
	if (row % 4 == 1 || (col-xstart) % 4 == 0)
   	return;

   int x, y;
   x = (col - xstart)/4 + 1;
   y = (row - 5)/4 + 1;

   G.setfillcolor(G.getcolor());
	mse_hide = 1;
   G.bar(xstart + (x-1)*4 + 1, 5 + (y-1)*4 + 1, xstart + x*4 - 1, 5 + y*4 - 1);
	if (G.getcolor() == 0)
   	Xout(xstart + (x-1)*4 + 1, 5 + (y-1)*4 + 1, xstart + x*4 - 1, 5 + y*4 - 1);
	G.putpixel(5 + x, 135 + y, G.getcolor());
}

void chekerase(int row, int col, int xstart, int xdim, int ydim)
{
	if (row >= 5 && row <= 5 + ydim*4 && col >= xstart && col <= xstart + xdim*4 && row%4 != 1 && (col-xstart)%4 != 0)
	{
   	int x, y;
   	x = (col - xstart)/4 + 1;
   	y = (row - 5)/4 + 1;

      mse_hide = 1;
   	G.setfillcolor(BLACK);
   	G.bar(xstart + (x-1)*4 + 1, 5 + (y-1)*4 + 1, xstart + x*4 - 1, 5 + y*4 - 1);
		G.putpixel(5 + x, 135 + y, BLACK);
   }
}

char exists(char *filename)
{
	FILE *testfile;

   if ((testfile = fopen(filename, "rb")) != NULL)
   {
   	fclose(testfile);
      return true;
   }
   else return false;
}

void LoadDim(char *filename, int &xdim, int &ydim)
{
	xdim = 16;
   ydim = 16;
}

void LoadPic(char *filename, int xstart)
{
	ifstream inf(filename);
   void *XORmask;
   int isize = G.imagesize(0, 0, 15, 15);
   int xdim=16, ydim=16;

   XORmask = farmalloc(isize);
   inf.read((char*)XORmask, isize);
   inf.close();

   G.putimage(6, 136, XORmask, COPY_PUT); //fix

   for (int i = 6; i <= 5 + xdim; i++)
   	for (int j = 136; j <= 135 + ydim; j++)
      {
      	G.setfillcolor(G.getpixel(i, j));
         G.bar(xstart + 1 + (i - 6)*4, 6 + (j - 136)*4,
               xstart - 1 + (i - 5)*4, 4 + (j - 135)*4);
      }

   farfree(XORmask);
}

void Xout(int xs, int ys, int xe, int ye)
{
   G.putpixel(xs, ys, LIGHTGRAY);
   G.putpixel(xs+1, ys+1, LIGHTGRAY);
   G.putpixel(xs, ys+2, LIGHTGRAY);
   G.putpixel(xe, ye, LIGHTGRAY);
   G.putpixel(xe, ys, LIGHTGRAY);
}
