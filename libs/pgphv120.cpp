/* VERSION INFO:

   PGphV100:  Basic 320x200x256 graphics.  No text functions.
              Implemented virtual screens.

   PGphV110:  Implemented text.

   PGphV120:  Implements screen transition effects.
              Effects are:
                1. Fade to black.
                2. Fade in from black.
                3. Fade to black and replace with virtual screen.
					 4. Dissolve to virtual screen.
                5. Bars draw in virtual screen.
                6. Screen slides away, shredded.
*/

#include <dos.h>
#include <mem.h>
#include <stdlib.h>
#include <math.h>
#include <alloc.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <iostream.h>

#include "PGphV120.H"

const int CHAR_X_MOVE = 4;
const int CHAR_Y_MOVE = 10;
const int CHAR_X_DIST = 3;
const int CHAR_Y_DIST = 5;
const int FADE_PRC = 2;
const int ANIM_WAIT = 25;
const int tail_length = 3;
const int BLINK_RATE = 50;
const int NUM_ANGLES = 360;
float sin_lookup[NUM_ANGLES];
float cos_lookup[NUM_ANGLES];

inline min(int x, int y) { return x < y ? x : y; }
inline int sgn(float a)
{
	if (a > 0) return 1;
   if (a < 0) return -1;
	return 0;
}
inline int round(float x)
{
	return int(floor(x+0.5));
}



GraphObject::GraphObject(void)
{
	int i;
   
	_p_graphobject_drawcolor = 1;
   _p_graphobject_fillcolor = 1;
   _p_graphobject_backcolor = 0;
   _p_graphobject_gmodeon = 0;
   _p_graphobject_xpos = 0;
   _p_graphobject_ypos = 0;
   _p_graphobject_hjust = 0;   // left_text
   _p_graphobject_vjust = 2;   // top_text
   _p_graphobject_bright = 100;

   for (i = 0; i < NUM_ANGLES; i++)
	{
		sin_lookup[i] = sin(float(i)/NUM_ANGLES*2*3.14159);
		cos_lookup[i] = cos(float(i)/NUM_ANGLES*2*3.14159);
	}
}

void GraphObject::initgraph(void)
{
	int i;

	asm {
   	mov ax, 0x0013
     	int 0x10
	}
   _p_graphobject_drawcolor = 15;
   _p_graphobject_fillcolor = 15;
   _p_graphobject_backcolor = 0;
   _p_graphobject_gmodeon = 1;
	for (i = 0; i < 256; i++)
   	getrgbpalette(i, &_p_graphobject_palette[i][0],
      					  &_p_graphobject_palette[i][1],
							  &_p_graphobject_palette[i][2]);
}

void GraphObject::closegraph(void)
{
	asm {
   	mov ax, 0x0003
     	int 0x10
	}
   _p_graphobject_gmodeon = 0;
}

void GraphObject::clearviewport(void)
{
	_fmemset(MK_FP(VGAaddr, 0), _p_graphobject_backcolor, 64000U);
}

void GraphObject::waitretrace(void)
{
	while ((inp(0x3DA) & 8) == 8)
   	;
   do
   	;
   while (! ((inp(0x3DA) & 8) == 8));
}

void GraphObject::moveto(int x, int y)
{
	_p_graphobject_xpos = x;
   _p_graphobject_ypos = y;
}

int GraphObject::getx(void)
{
	return _p_graphobject_xpos;
}

int GraphObject::gety(void)
{
	return _p_graphobject_ypos;
}

void GraphObject::putpixel(int x, int y, char color)
{
   if (x < 0) return;
   if (x >= 320) return;
   if (y < 0) return;
   if (y >= 200) return;

	unsigned VGA = VGAaddr;

	asm {
   	push    ds
    	push    es
    	mov     ax, [VGA]
    	mov     es, ax
    	mov     bx, [x]
    	mov     dx, [y]
    	push    bx                      //; and this again for later
    	mov     bx, dx                  //; bx = dx
    	mov     dh, dl                  //; dx = dx * 256
    	xor     dl, dl
    	shl     bx, 1
    	shl     bx, 1
    	shl     bx, 1
    	shl     bx, 1
    	shl     bx, 1
    	shl     bx, 1                   //; bx = bx * 64
    	add     dx, bx                  //; dx = dx + bx (ie y*320)
    	pop     bx                      //; get back our x
    	add     bx, dx                  //; finalise location
    	mov     di, bx
    	//; es:di = where to go
    	xor     al,al
    	mov     ah, [color]
    	mov     es:[di], ah
    	pop     es
    	pop     ds
   }
}

char GraphObject::getpixel(int x, int y)
{
	if (x < 0) return 0;
   if (x >= 320) return 0;
   if (y < 0) return 0;
   if (y >= 200) return 0;

   return peek(0xA000, x+y*320);
}

void GraphObject::getpixel(int x, int y, char *color)
{
	if (x < 0) { *color = 0; return; }
   if (x >= 320) { *color = 0; return; }
   if (y < 0) { *color = 0; return; }
   if (y >= 200) { *color = 0; return; }

   *color = peek(0xA000, x+y*320);
}

void GraphObject::setrgbpalette(char color, char red, char green, char blue)
  // This sets the Red, Green and Blue values of a certain color
{
	asm {
   	mov    dx, 0x3c8
      mov    al, color
      out    dx, al
      inc    dx
      mov    al, red
      out    dx, al
      mov    al, green
      out    dx, al
      mov    al, blue
      out    dx, al
	}
}

void GraphObject::getrgbpalette(char color, char *red, char *green, char *blue)
{
	char rr, gg, bb;

   asm {
   	mov    dx, 0x3c7
      mov    al, color
      out    dx, al

      add    dx, 2

      in     al, dx
      mov    [rr], al
      in     al, dx
      mov    [gg], al
      in     al, dx
      mov    [bb], al
	}
   *red = rr;
   *green = gg;
   *blue = bb;
}

void GraphObject::settextjustify(int h, int v)
{
	_p_graphobject_hjust = h;
   _p_graphobject_vjust = v;
}

void GraphObject::gettextjustify(int *h, int *v)
{
	*h = _p_graphobject_hjust;
   *v = _p_graphobject_vjust;
}

void GraphObject::swap(unsigned dest, unsigned source)
{                    /*
	asm {
   	push    ds
    	mov     ax, [dest]
    	mov     es, ax
    	mov     ax, [source]
    	mov     ds, ax
    	xor     si, si
    	xor     di, di
    	mov     cx, 32000
    	rep     movsw
    	pop     ds
  	}                   */

   _fmemcpy(MK_FP(dest, 0), MK_FP(source, 0), 64000U);
}

void GraphObject::putimage(int x, int y, void *image, char mode)
{
	int xdim, ydim;
   char *bitmap;

   xdim = *((int*)image);
   ydim = *((int*)image + 1);
   bitmap = (char*)((int*)image + 2);
   int num = xdim * ydim;
   char color;
   int i;

   switch (mode)
   {
   	case COPY_PUT : for (i = 0; i < num; i++)
   							putpixel(x+i%xdim, y+i/xdim, *(bitmap+i));
                      break;
      case AND_PUT  : for (i = 0; i < num; i++)
      					 {
                      	color = getpixel(x+i%xdim, y+i/xdim);
                        putpixel(x+i%xdim, y+i/xdim, color & *(bitmap+i));
                      }
                      break;
      case XOR_PUT  : for (i = 0; i < num; i++)
      					 {
                      	color = getpixel(x+i%xdim, y+i/xdim);
                        putpixel(x+i%xdim, y+i/xdim, color ^ *(bitmap+i));
                      }
                      break;
      case NOT_PUT  : for (i = 0; i < num; i++)
   							putpixel(x+i%xdim, y+i/xdim, !(*(bitmap+i)) );
                      break;
      case OR_PUT   : for (i = 0; i < num; i++)
      					 {
                      	color = getpixel(x+i%xdim, y+i/xdim);
                        putpixel(x+i%xdim, y+i/xdim, ((color) | (*(bitmap+i))));
                      }
                      break;
      default  	  : ;
   }
}

unsigned GraphObject::imagesize(int xs, int ys, int xe, int ye)
{
	return ( 2*sizeof(int) + (abs(xe-xs)+1)*(abs(ye-ys)+1)*sizeof(char) );
}

void GraphObject::getimage(int xs, int ys, int xe, int ye, void *image)
{
	int xdim = abs(xe-xs) + 1;
   int ydim = abs(ye-ys) + 1;

	*((int*)image) = xdim;
   *((int*)image + 1) = ydim;

   char *bitmap = (char*)((int*)image + 2);
   int num = xdim * ydim;

   for (int i = 0; i < num; i++)
   	*(bitmap+i) = getpixel(xs+i%xdim, ys+i/xdim);
}

void GraphObject::setcolor(char color)
{
	_p_graphobject_drawcolor = color;
}

void GraphObject::getcolor(char *color)
{
	*color = _p_graphobject_drawcolor;
}

char GraphObject::getcolor(void)
{
	return _p_graphobject_drawcolor;
}

void GraphObject::setbkcolor(char color)
{
	_p_graphobject_backcolor = color;
}

void GraphObject::getbkcolor(char *color)
{
	*color = _p_graphobject_backcolor;
}

char GraphObject::getbkcolor(void)
{
	return _p_graphobject_backcolor;
}

void GraphObject::setfillcolor(char color)
{
	_p_graphobject_fillcolor = color;
}

void GraphObject::getfillcolor(char *color)
{
	*color = _p_graphobject_fillcolor;
}

char GraphObject::getfillcolor(void)
{
	return _p_graphobject_fillcolor;
}

void GraphObject::hline(int xs, int ys, int xe, int ye)
{
	if (ys != ye) return;
   if (ys < 0 || ys >= 200) return;

   int m = min(xs, xe);
   int t;
   if (xs != m)
   { t = xs;   xs = xe;   xe = t; }
	if (xs < 0)     xs = 0;
   if (xe >= 320)  xe = 319;
   _fmemset(MK_FP(VGAaddr, xs+ys*320), _p_graphobject_drawcolor, xe-xs+1);
}

void GraphObject::vline(int xs, int ys, int xe, int ye)
{
	if (xs != xe) return;

   int start = min(ys, ye);
   int end = start + abs(ys-ye);
	for (int i = start; i <= end; i++)
   	putpixel(xs, i, _p_graphobject_drawcolor);
}

void GraphObject::line(int xs, int ys, int xe, int ye)
{
	float u, s, v, d1x, d1y, d2x, d2y, m, n;
   int i;
   int round_m, round_d1x, round_d2x, round_d1y, round_d2y;

   u = xe - xs;
   v = ye - ys;
   d1x = sgn(u);
   round_d1x = round(d1x);
   d1y = sgn(v);
   round_d1y = round(d1y);
   d2x = sgn(u);
   d2y = 0;
   m = abs(u);
   n = abs(v);
   if (m <= n)
   {
   	d2x = 0;
      d2y = sgn(v);
      m = abs(v);
      n = abs(u);
	}
   round_d2x = round(d2x);
   round_d2y = round(d2y);
   round_m = round(m);
   s = floor(m/2);
   for (i = 0; i <= round_m; i++)
   {
   	putpixel(xs, ys, _p_graphobject_drawcolor);
      s += n;
      if (s >= m)
      {
      	s = s - m;
         xs += round_d1x;
         ys += round_d1y;
		}
		else
      {
      	xs += round_d2x;
         ys += round_d2y;
      }
	}
}

void GraphObject::rectangle(int xs, int ys, int xe, int ye)
{
	hline(xs, ys, xe, ys);
   hline(xs, ye, xe, ye);
   vline(xs, ys, xs, ye);
   vline(xe, ys, xe, ye);
}

void GraphObject::bar(int xs, int ys, int xe, int ye)
{
	int ymin = min(ys, ye);
   int ymax = ymin + abs(ye-ys);

   char oldcolor = GraphObject::getcolor();
   GraphObject::setcolor(_p_graphobject_fillcolor);
	for (int row = ymin; row <= ymax; row++)
   	hline(xs, row, xe, row);
   GraphObject::setcolor(oldcolor);
}


//  Begin virtual screen stuff
void GraphObject::makevirtual(VirtualScreen *VScr)
{
	*VScr = malloc(64000U);
   if (*VScr == NULL)
   {
   	cerr << "Not enough memory for virtual screen";
      exit(1);
   }
}

void GraphObject::clearvirtual(VirtualScreen *VScr)
{
	free(*VScr);
}

void GraphObject::clearviewport(VirtualScreen VScr)
{
	_fmemset(MK_FP(FP_SEG(VScr), 0), _p_graphobject_backcolor, 64000U);
}

void GraphObject::putpixel(int x, int y, char color, VirtualScreen VScr)
{
   if (x < 0) return;
   if (x >= 320) return;
   if (y < 0) return;
   if (y >= 200) return;

	unsigned Vseg = FP_SEG(VScr);

	asm {
   	push    ds
    	push    es
    	mov     ax, [Vseg]
    	mov     es, ax
    	mov     bx, [x]
    	mov     dx, [y]
    	push    bx                      //; and this again for later
    	mov     bx, dx                  //; bx = dx
    	mov     dh, dl                  //; dx = dx * 256
    	xor     dl, dl
    	shl     bx, 1
    	shl     bx, 1
    	shl     bx, 1
    	shl     bx, 1
    	shl     bx, 1
    	shl     bx, 1                   //; bx = bx * 64
    	add     dx, bx                  //; dx = dx + bx (ie y*320)
    	pop     bx                      //; get back our x
    	add     bx, dx                  //; finalise location
    	mov     di, bx
    	//; es:di = where to go
    	xor     al,al
    	mov     ah, [color]
    	mov     es:[di], ah
    	pop     es
    	pop     ds
   }
}

char GraphObject::getpixel(int x, int y, VirtualScreen VScr)
{
	if (x < 0) return 0;
   if (x >= 320) return 0;
   if (y < 0) return 0;
   if (y >= 200) return 0;

   return peek(FP_SEG(VScr), x+y*320);
}

void GraphObject::getpixel(int x, int y, char *color, VirtualScreen VScr)
{
	if (x < 0) { *color = 0; return; }
   if (x >= 320) { *color = 0; return; }
   if (y < 0) { *color = 0; return; }
   if (y >= 200) { *color = 0; return; }

   *color = peek(FP_SEG(VScr), x+y*320);
}

void GraphObject::putimage(int x, int y, void *image, char mode, VirtualScreen VScr)
{
	int xdim, ydim;
   char *bitmap;

   xdim = *((int*)image);
   ydim = *((int*)image + 1);
   bitmap = (char*)((int*)image + 2);
   int num = xdim * ydim;
   char color;
   int i;

   switch (mode)
   {
   	case COPY_PUT : for (i = 0; i < num; i++)
   							putpixel(x+i%xdim, y+i/xdim, *(bitmap+i), VScr);
                      break;
      case AND_PUT  : for (i = 0; i < num; i++)
      					 {
                      	color = getpixel(x+i%xdim, y+i/xdim, VScr);
                        putpixel(x+i%xdim, y+i/xdim, color & *(bitmap+i), VScr);
                      }
                      break;
      case XOR_PUT  : for (i = 0; i < num; i++)
      					 {
                      	color = getpixel(x+i%xdim, y+i/xdim, VScr);
                        putpixel(x+i%xdim, y+i/xdim, color ^ *(bitmap+i), VScr);
                      }
                      break;
      case NOT_PUT  : for (i = 0; i < num; i++)
   							putpixel(x+i%xdim, y+i/xdim, !(*(bitmap+i)), VScr);
                      break;
      case OR_PUT   : for (i = 0; i < num; i++)
      					 {
                      	color = getpixel(x+i%xdim, y+i/xdim, VScr);
                        putpixel(x+i%xdim, y+i/xdim, ((color) | (*(bitmap+i))), VScr);
                      }
                      break;
      default  	  : ;
   }
}

void GraphObject::putimage_fast(int x, int y, void *image, VirtualScreen VScr)
{
	int xdim, ydim;
	char *bitmap;
	void *tmp;

	xdim = *((int*)image);
	ydim = *((int*)image + 1);
	bitmap = (char*)((int*)image + 2);

	int i, row;
	int start, end;

	if (y < 0)
	{
		start = 0;
		row = -y;
	}
	else
	{
		start = y;
		row = 0;
	}
	if (y+ydim > 200)
		end = 200;
	else
		end = y+ydim;


	for (i = start; i < end; i++)
	{
		tmp = (bitmap + row*xdim);
		_fmemcpy(MK_FP(FP_SEG(VScr), x+i*320), tmp, xdim);
      row++;
	}
}

void GraphObject::getimage(int xs, int ys, int xe, int ye, void far *image, VirtualScreen VScr)
{
	int xdim = abs(xe-xs) + 1;
   int ydim = abs(ye-ys) + 1;

	*((int*)image) = xdim;
   *((int*)image + 1) = ydim;

   char *bitmap = (char*)((int*)image + 2);
   int num = xdim * ydim;

   for (int i = 0; i < num; i++)
   	*(bitmap+i) = getpixel(xs+i%xdim, ys+i/xdim, VScr);
}

void GraphObject::hline(int xs, int ys, int xe, int ye, VirtualScreen VScr)
{
	if (ys != ye) return;
   if (ys < 0 || ys >= 200) return;

   int m = min(xs, xe);
   int t;
   if (xs != m)      { t = xs;   xs = xe;   xe = t; }
	if (xs < 0)     xs = 0;
   if (xe >= 320)  xe = 319;
   _fmemset(MK_FP(FP_SEG(VScr), xs+ys*320), _p_graphobject_drawcolor, xe-xs+1);
}

void GraphObject::vline(int xs, int ys, int xe, int ye, VirtualScreen VScr)
{
	if (xs != xe) return;

   int start = min(ys, ye);
   int end = start + abs(ys-ye);
	for (int i = start; i <= end; i++)
   	putpixel(xs, i, _p_graphobject_drawcolor, VScr);
}

void GraphObject::line(int xs, int ys, int xe, int ye, VirtualScreen VScr)
{
	float u, s, v, d1x, d1y, d2x, d2y, m, n;
   int i;
   int round_m, round_d1x, round_d2x, round_d1y, round_d2y;

   u = xe - xs;
   v = ye - ys;
   d1x = sgn(u);
   round_d1x = round(d1x);
   d1y = sgn(v);
   round_d1y = round(d1y);
   d2x = sgn(u);
   d2y = 0;
   m = abs(u);
   n = abs(v);
   if (m <= n)
   {
   	d2x = 0;
      d2y = sgn(v);
      m = abs(v);
      n = abs(u);
	}
   round_d2x = round(d2x);
   round_d2y = round(d2y);
   round_m = round(m);
   s = floor(m/2);
   for (i = 0; i <= round_m; i++)
   {
   	putpixel(xs, ys, _p_graphobject_drawcolor, VScr);
      s += n;
      if (s >= m)
      {
      	s = s - m;
         xs += round_d1x;
         ys += round_d1y;
		}
		else
      {
      	xs += round_d2x;
         ys += round_d2y;
      }
	}
}

void GraphObject::rectangle(int xs, int ys, int xe, int ye, VirtualScreen VScr)
{
	hline(xs, ys, xe, ys, VScr);
   hline(xs, ye, xe, ye, VScr);
   vline(xs, ys, xs, ye, VScr);
   vline(xe, ys, xe, ye, VScr);
}

void GraphObject::bar(int xs, int ys, int xe, int ye, VirtualScreen VScr)
{
	int ymin = min(ys, ye);
   int ymax = ymin + abs(ye-ys);

   char oldcolor = GraphObject::getcolor();
   GraphObject::setcolor(_p_graphobject_fillcolor);
	for (int row = ymin; row <= ymax; row++)
   	hline(xs, row, xe, row, VScr);
   GraphObject::setcolor(oldcolor);
}


// Text stuff

void GraphObject::_p_graphobject_DrawA(int x, int y, void far *Screen)
{
	putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   vline(x, y+1, x, y+4, Screen);
   vline(x+2, y+1, x+2, y+4, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawB(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
	putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawC(int x, int y, void far *Screen)
{
	vline(x, y+1, x, y+3, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawD(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   vline(x+2, y+1, x+2, y+3, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawE(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   hline(x+1, y, x+2, y, Screen);
   hline(x+1, y+4, x+2, y+4, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawF(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   hline(x+1, y, x+2, y, Screen);
   hline(x+1, y+2, x+2, y+2, Screen);
}

void GraphObject::_p_graphobject_DrawG(int x, int y, void far *Screen)
{
	hline(x+1, y, x+2, y, Screen);
   vline(x, y+1, x, y+3, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
   vline(x+2, y+2, x+2, y+3, Screen);
}

void GraphObject::_p_graphobject_DrawH(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   vline(x+2, y, x+2, y+4, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawI(int x, int y, void far *Screen)
{
	hline(x, y, x+2, y, Screen);
   hline(x, y+4, x+2, y+4, Screen);
   vline(x+1, y+1, x+1, y+3, Screen);
}

void GraphObject::_p_graphobject_DrawJ(int x, int y, void far *Screen)
{
	hline(x, y, x+2, y, Screen);
   vline(x+2, y+1, x+2, y+3, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawK(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   vline(x+2, y, x+2, y+1, Screen);
   vline(x+2, y+3, x+2, y+4, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawL(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   hline(x+1, y+4, x+2, y+4, Screen);
}

void GraphObject::_p_graphobject_DrawM(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   vline(x+2, y, x+2, y+4, Screen);
   putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawN(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   vline(x+2, y+1, x+2, y+4, Screen);
}

void GraphObject::_p_graphobject_DrawO(int x, int y, void far *Screen)
{
	vline(x, y+1, x, y+3, Screen);
   vline(x+2, y+1, x+2, y+3, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawP(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawQ(int x, int y, void far *Screen)
{
	vline(x, y+1, x, y+3, Screen);
   vline(x+2, y+1, x+2, y+2, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
   hline(x+1, y+4, x+2, y+4, Screen);
}

void GraphObject::_p_graphobject_DrawR(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   vline(x+2, y+3, x+2, y+4, Screen);
}

void GraphObject::_p_graphobject_DrawS(int x, int y, void far *Screen)
{
	hline(x+1, y, x+2, y, Screen);
   hline(x, y+4, x+1, y+4, Screen);
   putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawT(int x, int y, void far *Screen)
{
	hline(x, y, x+2, y, Screen);
   vline(x+1, y+1, x+1, y+4, Screen);
}

void GraphObject::_p_graphobject_DrawU(int x, int y, void far *Screen)
{
	vline(x, y, x, y+3, Screen);
   hline(x+1, y+4, x+2, y+4, Screen);
   vline(x+2, y, x+2, y+3, Screen);
}

void GraphObject::_p_graphobject_DrawV(int x, int y, void far *Screen)
{
	vline(x, y, x, y+3, Screen);
   hline(x, y+4, x+1, y+4, Screen);
   vline(x+2, y, x+2, y+3, Screen);
}

void GraphObject::_p_graphobject_DrawW(int x, int y, void far *Screen)
{
	vline(x, y, x, y+4, Screen);
   vline(x+2, y, x+2, y+4, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawX(int x, int y, void far *Screen)
{
	vline(x, y, x, y+1, Screen);
   vline(x, y+3, x, y+4, Screen);
   vline(x+2, y, x+2, y+1, Screen);
   vline(x+2, y+3, x+2, y+4, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawY(int x, int y, void far *Screen)
{
	vline(x, y, x, y+1, Screen);
   vline(x+2, y, x+2, y+1, Screen);
   vline(x+1, y+2, x+1, y+4, Screen);
}

void GraphObject::_p_graphobject_DrawZ(int x, int y, void far *Screen)
{
	hline(x, y, x+2, y, Screen);
   hline(x, y+4, x+2, y+4, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawPRD(int x, int y, void far *Screen)
{
	putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawQUES(int x, int y, void far *Screen)
{
	hline(x, y, x+1, y, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawEXC(int x, int y, void far *Screen)
{
	vline(x+1, y, x+1, y+2, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawCMA(int x, int y, void far *Screen)
{
	putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+5, _p_graphobject_drawcolor, Screen);
}

#pragma warn -par
void GraphObject::_p_graphobject_DrawSPC(int x, int y, void far *Screen)
{
	return;
}
#pragma warn .par

void GraphObject::_p_graphobject_Draw1(int x, int y, void far *Screen)
{
	putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   hline(x, y+1, x+1, y+1, Screen);
   vline(x+1, y+2, x+1, y+3, Screen);
   hline(x, y+4, x+2, y+4, Screen);
}

void GraphObject::_p_graphobject_Draw2(int x, int y, void far *Screen)
{
	hline(x, y, x+1, y, Screen);
   hline(x, y+4, x+2, y+4, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_Draw3(int x, int y, void far *Screen)
{
	hline(x, y, x+1, y, Screen);
   hline(x, y+4, x+1, y+4, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_Draw4(int x, int y, void far *Screen)
{
	hline(x, y+2, x+2, y+2, Screen);
   vline(x, y, x, y+1, Screen);
   vline(x+2, y, x+2, y+1, Screen);
   vline(x+2, y+3, x+2, y+4, Screen);
}

void GraphObject::_p_graphobject_Draw5(int x, int y, void far *Screen)
{
	hline(x, y, x+2, y, Screen);
   putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   hline(x, y+2, x+1, y+2, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
   hline(x, y+4, x+1, y+4, Screen);
}

void GraphObject::_p_graphobject_Draw6(int x, int y, void far *Screen)
{
	putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   hline(x, y+2, x+1, y+2, Screen);
   putpixel(x, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_Draw7(int x, int y, void far *Screen)
{
	hline(x, y, x+2, y, Screen);
   putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   vline(x+2, y+1, x+2, y+2, Screen);
   vline(x+1, y+3, x+1, y+4, Screen);
}

void GraphObject::_p_graphobject_Draw8(int x, int y, void far *Screen)
{
	putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_Draw9(int x, int y, void far *Screen)
{
	putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   hline(x+1, y+2, x+2, y+2, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_Draw0(int x, int y, void far *Screen)
{
	hline(x+1, y, x+2, y, Screen);
   hline(x, y+4, x+1, y+4, Screen);
   vline(x, y+1, x, y+3, Screen);
   vline(x+2, y+1, x+2, y+3, Screen);
}

void GraphObject::_p_graphobject_DrawMNS(int x, int y, void far *Screen)
{
	hline(x, y+2, x+2, y+2, Screen);
}

void GraphObject::_p_graphobject_DrawGRE(int x, int y, void far *Screen)
{
	putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawLES(int x, int y, void far *Screen)
{
	putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawQTE(int x, int y, void far *Screen)
{
	putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawPRC(int x, int y, void far *Screen)
{
	putpixel(x, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawCAR(int x, int y, void far *Screen)
{
	putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawLpar(int x, int y, void far *Screen)
{
	putpixel(x+2, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+4, _p_graphobject_drawcolor, Screen);
   vline(x+1, y+1, x+1, y+3, Screen);
}

void GraphObject::_p_graphobject_DrawRpar(int x, int y, void far *Screen)
{
 	putpixel(x, y, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+4, _p_graphobject_drawcolor, Screen);
   vline(x+1, y+1, x+1, y+3, Screen);
}

void GraphObject::_p_graphobject_DrawUND(int x, int y, void far *Screen)
{
	hline(x, y+4, x+2, y+4, Screen);
}

void GraphObject::_p_graphobject_DrawEQL(int x, int y, void far *Screen)
{
	hline(x, y+1, x+2, y+1, Screen);
   hline(x, y+3, x+2, y+3, Screen);
}

void GraphObject::_p_graphobject_DrawPLUS(int x, int y, void far *Screen)
{
	hline(x, y+2, x+2, y+2, Screen);
   putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawLbrk(int x, int y, void far *Screen)
{
	hline(x+1, y, x+2, y, Screen);
   hline(x+1, y+4, x+2, y+4, Screen);
   putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawRbrk(int x, int y, void far *Screen)
{
	hline(x, y, x+1, y, Screen);
   hline(x, y+4, x+1, y+4, Screen);
   putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawLbrc(int x, int y, void far *Screen)
{
	hline(x+1, y, x+2, y, Screen);
   hline(x+1, y+4, x+2, y+4, Screen);
	putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawRbrc(int x, int y, void far *Screen)
{
	hline(x, y, x+1, y, Screen);
   hline(x, y+4, x+1, y+4, Screen);
	putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawPIPE(int x, int y, void far *Screen)
{
	putpixel(x+1, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawBSLH(int x, int y, void far *Screen)
{
	putpixel(x, y, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawFSLH(int x, int y, void far *Screen)
{
	putpixel(x+2, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+2, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawCLN(int x, int y, void far *Screen)
{
	putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawSCLN(int x, int y, void far *Screen)
{
 	putpixel(x+1, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+1, y+3, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+4, _p_graphobject_drawcolor, Screen);
}

void GraphObject::_p_graphobject_DrawDQTE(int x, int y, void far *Screen)
{
	putpixel(x, y, _p_graphobject_drawcolor, Screen);
   putpixel(x, y+1, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y, _p_graphobject_drawcolor, Screen);
   putpixel(x+2, y+1, _p_graphobject_drawcolor, Screen);
}

void GraphObject::outtextxy(int x, int y, char *string)
{
	int len, i;
   len = strlen(string);
   int tw, th;

   tw = CHAR_X_DIST + CHAR_X_MOVE * (len-1);
   th = CHAR_Y_DIST;

   switch (_p_graphobject_hjust)
   {
   	case LEFT_TEXT : break;
      case CENTER_TEXT : x -= (tw/2);
                         break;
      case RIGHT_TEXT : x -= tw;
                        break;
      default : ;
   }

   switch (_p_graphobject_vjust)
   {
   	case TOP_TEXT : break;
      case CENTER_TEXT : y -= (th/2);
                         break;
      case BOTTOM_TEXT : y -= th;
                         break;
      default : ;
   }

   for (i = 0; i < len; i++)
   {
   	switch (toupper(string[i]))
      {
      	case 'A' : _p_graphobject_DrawA(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'B' : _p_graphobject_DrawB(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'C' : _p_graphobject_DrawC(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'D' : _p_graphobject_DrawD(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'E' : _p_graphobject_DrawE(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'F' : _p_graphobject_DrawF(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'G' : _p_graphobject_DrawG(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'H' : _p_graphobject_DrawH(x, y, MK_FP(VGAaddr, 0));
         		     break;
         case 'I' : _p_graphobject_DrawI(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'J' : _p_graphobject_DrawJ(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'K' : _p_graphobject_DrawK(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'L' : _p_graphobject_DrawL(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'M' : _p_graphobject_DrawM(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'N' : _p_graphobject_DrawN(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'O' : _p_graphobject_DrawO(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'P' : _p_graphobject_DrawP(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'Q' : _p_graphobject_DrawQ(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'R' : _p_graphobject_DrawR(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'S' : _p_graphobject_DrawS(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'T' : _p_graphobject_DrawT(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'U' : _p_graphobject_DrawU(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'V' : _p_graphobject_DrawV(x, y, MK_FP(VGAaddr, 0));
         		     break;
         case 'W' : _p_graphobject_DrawW(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'X' : _p_graphobject_DrawX(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'Y' : _p_graphobject_DrawY(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 'Z' : _p_graphobject_DrawZ(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '1' : _p_graphobject_Draw1(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '2' : _p_graphobject_Draw2(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '3' : _p_graphobject_Draw3(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '4' : _p_graphobject_Draw4(x, y, MK_FP(VGAaddr, 0));
         		     break;
         case '5' : _p_graphobject_Draw5(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '6' : _p_graphobject_Draw6(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '7' : _p_graphobject_Draw7(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '8' : _p_graphobject_Draw8(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '9' : _p_graphobject_Draw9(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '0' : _p_graphobject_Draw0(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '.' : _p_graphobject_DrawPRD(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case ',' : _p_graphobject_DrawCMA(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '?' : _p_graphobject_DrawQUES(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '!' : _p_graphobject_DrawEXC(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case ' ' : _p_graphobject_DrawSPC(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '-' : _p_graphobject_DrawMNS(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '>' : _p_graphobject_DrawGRE(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '<' : _p_graphobject_DrawLES(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '`' :
         case '\'' : _p_graphobject_DrawQTE(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case 37 : _p_graphobject_DrawPRC(x, y, MK_FP(VGAaddr, 0));   // percent
         			  break;
         case '^' : _p_graphobject_DrawCAR(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '(' : _p_graphobject_DrawLpar(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case ')' : _p_graphobject_DrawRpar(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '_' : _p_graphobject_DrawUND(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '=' : _p_graphobject_DrawEQL(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '+' : _p_graphobject_DrawPLUS(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '[' : _p_graphobject_DrawLbrk(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case ']' : _p_graphobject_DrawRbrk(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '{' : _p_graphobject_DrawLbrc(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '}' : _p_graphobject_DrawRbrc(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '|' : _p_graphobject_DrawPIPE(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '\\' : _p_graphobject_DrawBSLH(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '/' : _p_graphobject_DrawFSLH(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case ':' : _p_graphobject_DrawCLN(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case ';' : _p_graphobject_DrawSCLN(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '\"' : _p_graphobject_DrawDQTE(x, y, MK_FP(VGAaddr, 0));
         			  break;
         default  : ;
      }
      x += CHAR_X_MOVE;
   }
}

void GraphObject::outtext(char *string)
{
	outtextxy(_p_graphobject_xpos, _p_graphobject_ypos, string);
   int tw;
   int len=strlen(string);

   tw = CHAR_X_DIST + CHAR_X_MOVE * (len-1);

   switch (_p_graphobject_hjust)
   {
   	case LEFT_TEXT : _p_graphobject_xpos += CHAR_X_MOVE*len;
      				     break;
      case CENTER_TEXT : _p_graphobject_xpos -= (tw/2);
                         _p_graphobject_xpos += CHAR_X_MOVE*len;
                         break;
      case RIGHT_TEXT : _p_graphobject_xpos -= tw;
                        _p_graphobject_xpos += CHAR_X_MOVE*len;
                        break;
      default : ;
   }
}

void GraphObject::outtextxy(int x, int y, char *string, VirtualScreen VScr)
{
	int len, i;
   len = strlen(string);
   int tw, th;

   tw = CHAR_X_DIST + CHAR_X_MOVE * (len-1);
   th = CHAR_Y_DIST;

   switch (_p_graphobject_hjust)
   {
   	case LEFT_TEXT : break;
      case CENTER_TEXT : x -= (tw/2);
                         break;
      case RIGHT_TEXT : x -= tw;
                        break;
      default : ;
   }

   switch (_p_graphobject_vjust)
   {
   	case TOP_TEXT : break;
      case CENTER_TEXT : y -= (th/2);
                         break;
      case BOTTOM_TEXT : y -= th;
                         break;
      default : ;
   }

   for (i = 0; i < len; i++)
   {
   	switch (toupper(string[i]))
      {
      	case 'A' : _p_graphobject_DrawA(x, y, VScr);
         			  break;
         case 'B' : _p_graphobject_DrawB(x, y, VScr);
         			  break;
         case 'C' : _p_graphobject_DrawC(x, y, VScr);
         			  break;
         case 'D' : _p_graphobject_DrawD(x, y, VScr);
         			  break;
         case 'E' : _p_graphobject_DrawE(x, y, VScr);
         			  break;
         case 'F' : _p_graphobject_DrawF(x, y, VScr);
         			  break;
         case 'G' : _p_graphobject_DrawG(x, y, VScr);
         			  break;
         case 'H' : _p_graphobject_DrawH(x, y, VScr);
         		     break;
         case 'I' : _p_graphobject_DrawI(x, y, VScr);
         			  break;
         case 'J' : _p_graphobject_DrawJ(x, y, VScr);
         			  break;
         case 'K' : _p_graphobject_DrawK(x, y, VScr);
         			  break;
         case 'L' : _p_graphobject_DrawL(x, y, VScr);
         			  break;
         case 'M' : _p_graphobject_DrawM(x, y, VScr);
         			  break;
         case 'N' : _p_graphobject_DrawN(x, y, VScr);
         			  break;
         case 'O' : _p_graphobject_DrawO(x, y, VScr);
         			  break;
         case 'P' : _p_graphobject_DrawP(x, y, VScr);
         			  break;
         case 'Q' : _p_graphobject_DrawQ(x, y, VScr);
         			  break;
         case 'R' : _p_graphobject_DrawR(x, y, VScr);
         			  break;
         case 'S' : _p_graphobject_DrawS(x, y, VScr);
         			  break;
         case 'T' : _p_graphobject_DrawT(x, y, VScr);
         			  break;
         case 'U' : _p_graphobject_DrawU(x, y, VScr);
         			  break;
         case 'V' : _p_graphobject_DrawV(x, y, VScr);
         		     break;
         case 'W' : _p_graphobject_DrawW(x, y, VScr);
         			  break;
         case 'X' : _p_graphobject_DrawX(x, y, VScr);
         			  break;
         case 'Y' : _p_graphobject_DrawY(x, y, VScr);
         			  break;
         case 'Z' : _p_graphobject_DrawZ(x, y, VScr);
         			  break;
         case '1' : _p_graphobject_Draw1(x, y, VScr);
         			  break;
         case '2' : _p_graphobject_Draw2(x, y, VScr);
         			  break;
         case '3' : _p_graphobject_Draw3(x, y, VScr);
         			  break;
         case '4' : _p_graphobject_Draw4(x, y, VScr);
         		     break;
         case '5' : _p_graphobject_Draw5(x, y, VScr);
         			  break;
         case '6' : _p_graphobject_Draw6(x, y, VScr);
         			  break;
         case '7' : _p_graphobject_Draw7(x, y, VScr);
         			  break;
         case '8' : _p_graphobject_Draw8(x, y, VScr);
         			  break;
         case '9' : _p_graphobject_Draw9(x, y, VScr);
         			  break;
         case '0' : _p_graphobject_Draw0(x, y, VScr);
         			  break;
         case '.' : _p_graphobject_DrawPRD(x, y, VScr);
         			  break;
         case ',' : _p_graphobject_DrawCMA(x, y, VScr);
         			  break;
         case '?' : _p_graphobject_DrawQUES(x, y, VScr);
         			  break;
         case '!' : _p_graphobject_DrawEXC(x, y, VScr);
         			  break;
         case ' ' : _p_graphobject_DrawSPC(x, y, VScr);
         			  break;
         case '-' : _p_graphobject_DrawMNS(x, y, VScr);
         			  break;
         case '>' : _p_graphobject_DrawGRE(x, y, VScr);
         			  break;
         case '<' : _p_graphobject_DrawLES(x, y, VScr);
         			  break;
         case '`' :
         case '\'' : _p_graphobject_DrawQTE(x, y, VScr);
         			  break;
         case 37 : _p_graphobject_DrawPRC(x, y, VScr);   // percent
         			  break;
         case '^' : _p_graphobject_DrawCAR(x, y, VScr);
         			  break;
         case '(' : _p_graphobject_DrawLpar(x, y, VScr);
         			  break;
         case ')' : _p_graphobject_DrawRpar(x, y, VScr);
         			  break;
         case '_' : _p_graphobject_DrawUND(x, y, VScr);
         			  break;
         case '=' : _p_graphobject_DrawEQL(x, y, VScr);
         			  break;
         case '+' : _p_graphobject_DrawPLUS(x, y, VScr);
         			  break;
         case '[' : _p_graphobject_DrawLbrk(x, y, VScr);
         			  break;
         case ']' : _p_graphobject_DrawRbrk(x, y, VScr);
         			  break;
         case '{' : _p_graphobject_DrawLbrc(x, y, VScr);
         			  break;
         case '}' : _p_graphobject_DrawRbrc(x, y, VScr);
         			  break;
         case '|' : _p_graphobject_DrawPIPE(x, y, VScr);
         			  break;
         case '\\' : _p_graphobject_DrawBSLH(x, y, VScr);
         			  break;
         case '/' : _p_graphobject_DrawFSLH(x, y, VScr);
         			  break;
         case ':' : _p_graphobject_DrawCLN(x, y, VScr);
         			  break;
         case ';' : _p_graphobject_DrawSCLN(x, y, VScr);
         			  break;
         case '\"' : _p_graphobject_DrawDQTE(x, y, VScr);
         			  break;
         default  : ;
      }
      x += CHAR_X_MOVE;
   }
}

void GraphObject::outtext(char *string, VirtualScreen VScr)
{
	outtextxy(_p_graphobject_xpos, _p_graphobject_ypos, string, VScr);
   int tw;
   int len=strlen(string);

   tw = CHAR_X_DIST + CHAR_X_MOVE * (len-1);

   switch (_p_graphobject_hjust)
   {
   	case LEFT_TEXT : _p_graphobject_xpos += CHAR_X_MOVE*len;
      				     break;
      case CENTER_TEXT : _p_graphobject_xpos -= (tw/2);
                         _p_graphobject_xpos += CHAR_X_MOVE*len;
                         break;
      case RIGHT_TEXT : _p_graphobject_xpos -= tw;
                        _p_graphobject_xpos += CHAR_X_MOVE*len;
                        break;
      default : ;
   }
}

void GraphObject::write(int x, int y, int col, int tail_col, char *string, int delay_time)
{
   int i, j;           //loop counters
   int cols = strlen(string);
   char tmpstr[2] = {'\0', '\0'};

   settextjustify(LEFT_TEXT, TOP_TEXT);
   for (i = 0; i < cols; i++) //Draws an entire line
   {
		tmpstr[0] = string[i];
      setcolor(tail_col);
      outtextxy(x, y, tmpstr);
      delay(delay_time);      //pauses for the specified amount of time
      if (i >= tail_length)
      {
      	tmpstr[0] = string[i-tail_length];
         setcolor(col);
         outtextxy(x - CHAR_X_MOVE * tail_length, y, tmpstr);
      }

      x += CHAR_X_MOVE; //Increments the x position by the specified amount
   }

   setcolor(col);
   x -= CHAR_X_MOVE * tail_length;
   for (j = max(i - tail_length, 0); j < cols; j++)
   {
   	tmpstr[0] = string[j];
      outtextxy(x, y, tmpstr);
      x += CHAR_X_MOVE;
      delay(delay_time);
   }
}

void GraphObject::write(int x, int y, int col, int tail_col, char *string, int delay_time, char &stop_flag)
{
   int i, j;           //loop counters
   int cols = strlen(string);
   char tmpstr[2] = {'\0', '\0'};

   for (i = 0; i < cols; i++) //Draws an entire line
   {
		tmpstr[0] = string[i];
      setcolor(tail_col);
      outtextxy(x, y, tmpstr);
      if (stop_flag)
      	return;
      delay(delay_time);      //pauses for the specified amount of time
      if (stop_flag)
      	return;
      if (i >= tail_length)
      {
      	tmpstr[0] = string[i-tail_length];
         setcolor(col);
         outtextxy(x - CHAR_X_MOVE * tail_length, y, tmpstr);
      }

      x += CHAR_X_MOVE; //Increments the x position by the specified amount
   }

   setcolor(col);
   x -= CHAR_X_MOVE * tail_length;
   for (j = max(i - tail_length, 0); j < cols; j++)
   {
   	tmpstr[0] = string[j];
      outtextxy(x, y, tmpstr);
      x += CHAR_X_MOVE;
      delay(delay_time);
   }
}

void GraphObject::DrawCrsr(int x_cur, int y_cur)
{
   hline(x_cur, y_cur+CHAR_Y_DIST-1, x_cur+CHAR_X_DIST-1, y_cur+CHAR_Y_DIST-1);
}

void GraphObject::intext(int x_s, int y_s, int x_e, int y_e, char color, char *string)
{
	int x_cur=x_s, y_cur=(y_s+y_e-CHAR_Y_DIST)/2;
	char ch;
   char blink_on=0;
   char swit=1;
   struct time t;
   long last_switch, cur_time;
   int letter_pos=0;
   int old_fill_color, old_draw_color, old_hjust, old_vjust;
   char tmpstr[2] = {'\0', '\0'};
   char done=0;
   int max_chars=(x_e-x_s)/CHAR_X_MOVE;

   old_fill_color = getfillcolor();
   old_draw_color = getcolor();
   gettextjustify(&old_hjust, &old_vjust);
   setfillcolor(getbkcolor());
   settextjustify(LEFT_TEXT, TOP_TEXT);


   do
   {
   	while (!kbhit())
      	if (swit)
         {
         	if (blink_on)
              	setcolor(getbkcolor());
            else
            	setcolor(color);
            if (letter_pos < max_chars)
	            DrawCrsr(x_cur, y_cur);

         	blink_on = !blink_on;

            if (blink_on)
              	setcolor(getbkcolor());
            else
            	setcolor(color);
            if (letter_pos < max_chars)
	            DrawCrsr(x_cur, y_cur);
            swit = 0;

            gettime(&t);
            last_switch = 360000L*t.ti_hour + 6000*t.ti_min + 100*t.ti_sec + t.ti_hund;
         }
         else
         {
         	gettime(&t);
				cur_time = 360000L*t.ti_hour + 6000*t.ti_min + 100*t.ti_sec + t.ti_hund;
            if (cur_time >= last_switch + BLINK_RATE)
            	swit = 1;
         }

      ch = getch();
      if (ch == 0)
      {
      	ch = getch();
         if (ch == 75 && letter_pos > 0)
         {
         	setcolor(getbkcolor());
            if (letter_pos < max_chars)
	            DrawCrsr(x_cur, y_cur);
         	x_cur -= CHAR_X_MOVE;
            letter_pos--;
            tmpstr[0] = string[letter_pos];
				outtextxy(x_cur, y_cur, tmpstr);
         }
      }
      else if (ch == 8 && letter_pos > 0)   // backspace
      {
      	setcolor(getbkcolor());
         if (letter_pos < max_chars)
	         DrawCrsr(x_cur, y_cur);
      	x_cur -= CHAR_X_MOVE;
         letter_pos--;
			tmpstr[0] = string[letter_pos];
			outtextxy(x_cur, y_cur, tmpstr);
      }
		else if (

		(ch >= 32 && ch <= 34 ||
       ch == 37 ||
       ch >= 39 && ch <= 41 ||
       ch >= 43 && ch <= 63 ||
       ch >= 65 && ch <= 125)
       
      && letter_pos < max_chars)

      {
         setcolor(getbkcolor());
         DrawCrsr(x_cur, y_cur);
         tmpstr[0] = ch;
         string[letter_pos++] = ch;
         setcolor(color);
			outtextxy(x_cur, y_cur, tmpstr);
         x_cur += CHAR_X_MOVE;
      }
      else if (ch == 13)  // enter
      {
      	while (kbhit())
         	getch();
         string[letter_pos] = '\0';
         setcolor(getbkcolor());
			DrawCrsr(x_cur, y_cur);
         done = 1;
      }

      ch = 1;
   } while (!done);

	setfillcolor(old_fill_color);
	setcolor(old_draw_color);
	settextjustify(old_hjust, old_vjust);
}

void GraphObject::ellipse(float x, float y, float start, float end, float x_rad, float y_rad)
{
	int i;

	for (i = start*NUM_ANGLES/360; i <= end*NUM_ANGLES/360-1; i++)
		line(x + x_rad*cos_lookup[i%NUM_ANGLES], y - y_rad*sin_lookup[i%NUM_ANGLES], x + x_rad*cos_lookup[(i+1)%NUM_ANGLES], y - y_rad*sin_lookup[(i+1)%NUM_ANGLES]);
}

void GraphObject::ellipse(float x, float y, float start, float end, float x_rad, float y_rad, VirtualScreen VScr)
{
	int i;

	for (i = start*NUM_ANGLES/360; i <= end*NUM_ANGLES/360-1; i++)
		line(x + x_rad*cos_lookup[i%NUM_ANGLES], y - y_rad*sin_lookup[i%NUM_ANGLES], x + x_rad*cos_lookup[(i+1)%NUM_ANGLES], y - y_rad*sin_lookup[(i+1)%NUM_ANGLES], VScr);
}

void GraphObject::h_floodfill(int x, int y, char border)
{
	if (getpixel(x, y) == border)   return;
   
	int xs, xe;
	int i;
   int y_point = y;

   for ( ; y > 0; y--)     // go up
   {
   	if (getpixel(x, y) == border) break;

   	for (i = x; i >= 0 && getpixel(i, y) != border; i--)  ;  // go left

	   xs = i+1;

	   for (i = x; i < 320 && getpixel(i, y) != border; i++)  ;  // go right

		xe = i-1;

	   _fmemset(MK_FP(VGAaddr, xs+y*320), _p_graphobject_fillcolor, xe-xs+1);
   }

   for (y = y_point+1; y < 200; y++)
   {
   	if (getpixel(x, y) == border) break;

   	for (i = x; i >= 0 && getpixel(i, y) != border; i--)  ;  // go left

	   xs = i+1;

	   for (i = x; i < 320 && getpixel(i, y) != border; i++)  ;  // go right

		xe = i-1;

	   _fmemset(MK_FP(VGAaddr, xs+y*320), _p_graphobject_fillcolor, xe-xs+1);
   }
}


void GraphObject::h_floodfill(int x, int y, char border, VirtualScreen VScr)
{
	if (getpixel(x, y, VScr) == border)   return;

	int xs, xe;
	int i;
   int y_point = y;

   for ( ; y > 0; y--)     // go up
   {
   	if (getpixel(x, y, VScr) == border) break;

   	for (i = x; i >= 0 && getpixel(i, y, VScr) != border; i--)  ;  // go left

	   xs = i+1;

	   for (i = x; i < 320 && getpixel(i, y, VScr) != border; i++)  ;  // go right

		xe = i-1;

	   _fmemset(MK_FP(FP_SEG(VScr), xs+y*320), _p_graphobject_fillcolor, xe-xs+1);
   }

   for (y = y_point+1; y < 200; y++)
   {
   	if (getpixel(x, y, VScr) == border) break;

   	for (i = x; i >= 0 && getpixel(i, y, VScr) != border; i--)  ;  // go left

	   xs = i+1;

	   for (i = x; i < 320 && getpixel(i, y, VScr) != border; i++)  ;  // go right

		xe = i-1;

	   _fmemset(MK_FP(FP_SEG(VScr), xs+y*320), _p_graphobject_fillcolor, xe-xs+1);
   }
}


// Screen effects

void GraphObject::fade_out(void)
{
	int i, j;
	float multiplier;

	for (i = floor(_p_graphobject_bright/FADE_PRC)*FADE_PRC; i >= 0; i -= FADE_PRC)
	{
   	multiplier = i/100.0;
      waitretrace();
   	for (j = 0; j < 256; j++)
      	setrgbpalette(j, multiplier*_p_graphobject_palette[j][0],
                          multiplier*_p_graphobject_palette[j][1],
                          multiplier*_p_graphobject_palette[j][2]);
   }

   _p_graphobject_bright = 0;
}

void GraphObject::fade_to(float percent)
  // all colors go to <percent>% intensity
{
	int i;
	float multiplier = percent/100.0;

   waitretrace();
	for (i = 0; i < 256; i++)
   	setrgbpalette(i, multiplier*_p_graphobject_palette[i][0],
                       multiplier*_p_graphobject_palette[i][1],
                       multiplier*_p_graphobject_palette[i][2]);

   _p_graphobject_bright = percent;
}

void GraphObject::fade_in(void)
{
	int i, j;
   float multiplier;

   for (i = ceil(_p_graphobject_bright/FADE_PRC)*FADE_PRC; i <= 100; i += FADE_PRC)
   {
   	multiplier = i/100.0;
      waitretrace();
   	for (j = 0; j < 256; j++)
      	setrgbpalette(j, multiplier*_p_graphobject_palette[j][0],
                          multiplier*_p_graphobject_palette[j][1],
                          multiplier*_p_graphobject_palette[j][2]);
   }
   _p_graphobject_bright = 100;
}

void GraphObject::scroll_up(int rows)
{
	int i;
	for (i = 0; i < rows; i++)
   {
   	waitretrace();
	   _fmemcpy(MK_FP(VGAaddr, 0), MK_FP(VGAaddr, 320), 63680U);
   }
}

void GraphObject::scroll_up(int rows, char &stop_flag)
{
	int i;
	for (i = 0; i < rows; i++)
   {
   	if (stop_flag)
      	return;
   	waitretrace();
      if (stop_flag)
      	return;
	   _fmemcpy(MK_FP(VGAaddr, 0), MK_FP(VGAaddr, 320), 63680U);
   }
}

void GraphObject::fade_into(VirtualScreen VScr)
  // fades into virtual screen
{
	fade_out();
   GraphObject::swap(VGAaddr, FP_SEG(VScr));
   fade_in();
}

void GraphObject::dissolve(VirtualScreen VScr)
   // dissolves into virtual screen
{
	int i;

   for (i = 100; i >= 50; i -= FADE_PRC)
   {
   	fade_to(i);
      delay(ANIM_WAIT);
   }
   GraphObject::swap(VGAaddr, FP_SEG(VScr));
   for (i = 50; i <= 100; i += FADE_PRC)
   {
   	fade_to(i);
      delay(ANIM_WAIT);
   }
}

void GraphObject::bars_wipe(VirtualScreen VScr)
  // transition with bars
{
	int row, col;
   float ratio = 200.0/320.0;
   int i;
   char pixel;

   for (col = 0; col <= 320; col++)
   {
   	row = ratio*col;
      setfillcolor(28);
      bar(0, row, 319, row+1);
      bar(col, 0, col+1, 199);
      setfillcolor(29);
      bar(col-5, row, col-4, row+1);
      bar(col, row-5, col+1, row-4);
      bar(col+4, row, col+5, row+1);
      bar(col, row+5, col+1, row+4);
		setfillcolor(30);
      bar(col-3, row, col-2, row+1);
      bar(col+2, row, col+3, row+1);
      bar(col, row-3, col+1, row-2);
      bar(col, row+3, col+1, row+2);
		setfillcolor(31);
      bar(col-1, row, col+1, row+1);
      bar(col, row-1, col+1, row+1);

      for (i = 0; i < row; i++)
      {
      	pixel = getpixel(col-1, i, VScr);
         putpixel(col-1, i, pixel);
      }
      for (i = 0; i < col; i++)
      {
      	pixel = getpixel(i, row-1, VScr);
         putpixel(i, row-1, pixel);
      }
      pixel = getpixel(col-1, row-1, VScr);
      putpixel(col-1, row-1, pixel);

      delay(ANIM_WAIT/10);
   }
}

void GraphObject::shred_clear(void)
{
	void far *v2;
   void far *VirScr;
   void far *bitmap;
   int i, j;
   const int SPEED = 5;

   bitmap = malloc(imagesize(0, 0, 319, 199));
	makevirtual(&v2);
   makevirtual(&VirScr);

   setcolor(_p_graphobject_backcolor);
   for (i = 0; i < 320; i += SPEED)
   {
   	clearviewport(VirScr);

   	getimage(0, 0, 319, 199, bitmap);
      putimage(0, 0, bitmap, COPY_PUT, v2);
      for (j = 0; j < 200; j += 2)
      	hline(0, j, 319, j, v2);
      getimage(0, 0, 319, 199, bitmap, v2);
      putimage(-SPEED, 0, bitmap, COPY_PUT, VirScr);      /*

      getimage(0, 0, 319, 199, bitmap);
      putimage(0, 0, bitmap, COPY_PUT, v2);
      for (j = 1; j < 200; j += 2)
      	hline(0, j, 319, j, v2);
      getimage(0, 0, 319, 199, bitmap, v2);
      putimage(SPEED, 0, bitmap, OR_PUT, VirScr);           */

      waitretrace();
      GraphObject::swap(VGAaddr, FP_SEG(VirScr));
   }

   clearvirtual(&v2);
   clearvirtual(&VirScr);
   farfree(bitmap);
}
