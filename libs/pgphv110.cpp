#include <dos.h>
#include <mem.h>
#include <stdlib.h>
#include <graphics.h>
#include <math.h>
#include <alloc.h>
#include <string.h>
#include <ctype.h>

#include "PGphV110.H"

const int CHAR_X_MOVE = 4;

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

void GraphObject::initgraph(void)
{
	asm {
   	mov ax, 0x0013
     	int 0x10
	}
   _p_graphobject_drawcolor = 15;
   _p_graphobject_fillcolor = 15;
   _p_graphobject_backcolor = 0;
   _p_graphobject_gmodeon = 1;
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
	#pragma warn -cln
   #pragma warn -sig
	_fmemset(MK_FP(VGAaddr, 0), _p_graphobject_backcolor, 64000);
   #pragma warn .sig
   #pragma warn .cln
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

void GraphObject::swap(unsigned dest, unsigned source)
{
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
  	}
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
   int m = min(xs, xe);
   int a = abs(xs-xe);
   _fmemset(MK_FP(VGAaddr, m+ys*320), _p_graphobject_drawcolor, a+m>319?320-m:a+1);
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
void GraphObject::makevirtual(void far **VScr)
{
   #pragma warn -cln
	*VScr = farmalloc(64000);
   #pragma warn .cln
}

void GraphObject::clearvirtual(void far **VScr)
{
	farfree(*VScr);
}

void GraphObject::clearviewport(void far *VScr)
{
	#pragma warn -cln
   #pragma warn -sig
	_fmemset(MK_FP(FP_SEG(VScr), 0), _p_graphobject_backcolor, 64000);
   #pragma warn .sig
   #pragma warn .cln
}

void GraphObject::putpixel(int x, int y, char color, void far *VScr)
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

char GraphObject::getpixel(int x, int y, void far *VScr)
{
	if (x < 0) return 0;
   if (x >= 320) return 0;
   if (y < 0) return 0;
   if (y >= 200) return 0;

   return peek(FP_SEG(VScr), x+y*320);
}

void GraphObject::getpixel(int x, int y, char *color, void far *VScr)
{
	if (x < 0) { *color = 0; return; }
   if (x >= 320) { *color = 0; return; }
   if (y < 0) { *color = 0; return; }
   if (y >= 200) { *color = 0; return; }

   *color = peek(FP_SEG(VScr), x+y*320);
}

void GraphObject::putimage(int x, int y, void *image, char mode, void far *VScr)
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
      default  	  : ;
   }
}

void GraphObject::getimage(int xs, int ys, int xe, int ye, void *image, void far *VScr)
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

void GraphObject::hline(int xs, int ys, int xe, int ye, void far *VScr)
{
	if (ys != ye) return;
   int m = min(xs, xe);
	int a = abs(xs-xe);
   _fmemset(MK_FP(FP_SEG(VScr), m+ys*320), _p_graphobject_drawcolor, a+m>319?320-m:a+1);
}

void GraphObject::vline(int xs, int ys, int xe, int ye, void far *VScr)
{
	if (xs != xe) return;

   int start = min(ys, ye);
   int end = start + abs(ys-ye);
	for (int i = start; i <= end; i++)
   	putpixel(xs, i, _p_graphobject_drawcolor, VScr);
}

void GraphObject::line(int xs, int ys, int xe, int ye, void far *VScr)
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

void GraphObject::rectangle(int xs, int ys, int xe, int ye, void far *VScr)
{
	hline(xs, ys, xe, ys, VScr);
   hline(xs, ye, xe, ye, VScr);
   vline(xs, ys, xs, ye, VScr);
   vline(xe, ys, xe, ye, VScr);
}

void GraphObject::bar(int xs, int ys, int xe, int ye, void far *VScr)
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

void GraphObject::outtextxy(int x, int y, char *string)
{
	int len, i;
   len = strlen(string);

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
         case ' ' : _p_graphobject_DrawSPC(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '-' : _p_graphobject_DrawMNS(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '>' : _p_graphobject_DrawGRE(x, y, MK_FP(VGAaddr, 0));
         			  break;
         case '<' : _p_graphobject_DrawLES(x, y, MK_FP(VGAaddr, 0));
         			  break;
         default  : ;
      }
      x += CHAR_X_MOVE;
   }
}

void GraphObject::outtext(char *string)
{
	outtextxy(_p_graphobject_xpos, _p_graphobject_ypos, string);
}

void GraphObject::outtextxy(int x, int y, char *string, void far *VScr)
{
	int len, i;
   len = strlen(string);

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
         case ' ' : _p_graphobject_DrawSPC(x, y, VScr);
         			  break;
         case '-' : _p_graphobject_DrawMNS(x, y, VScr);
         			  break;
         case '>' : _p_graphobject_DrawGRE(x, y, VScr);
         			  break;
         case '<' : _p_graphobject_DrawLES(x, y, VScr);
         			  break;
         default  : ;
      }
      x += CHAR_X_MOVE;
   }
}

void GraphObject::outtext(char *string, void far *VScr)
{
	outtextxy(_p_graphobject_xpos, _p_graphobject_ypos, string, VScr);
}
