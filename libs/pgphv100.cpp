#include <dos.h>
#include <mem.h>
#include <stdlib.h>
#include <graphics.h>
#include <math.h>
#include <alloc.h>

#include "PGphV100.H"

inline min(int x, int y) { return x < y ? x : y; }
inline int sgn(float a)
{
	if (a > 0) return 1;
   if (a < 0) return -1;
	return 0;
}
inline int round(float x)
{
	if (x - floor(x) >= 0.5) return ceil(x);
   return floor(x);
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
	setmem(MK_FP(VGAaddr, 0), 64000, _p_graphobject_backcolor);
   #pragma warn .sig
   #pragma warn .cln
}

void GraphObject::waitretrace(void)
  // This waits until you are in a Verticle Retrace ... this means that all
  // screen manipulation you do only appears on screen in the next verticle
  // retrace ... this removes most of the "fuzz" that you see on the screen
  // when changing the pallette. It unfortunately slows down your program
  // by "synching" your program with your monitor card ... it does mean
  // that the program will run at almost the same speed on different
  // speeds of computers which have similar monitors. In our SilkyDemo,
  // we used a WaitRetrace, and it therefore runs at the same (fairly
  // fast) speed when Turbo is on or off.
{
	asm {
    	mov dx, 0x3DA
l1:
    	in al, dx
    	and al, 0x08
    	jnz l1
l2:
    	in al, dx
    	and al, 0x08
    	jz  l2
	}
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

void GraphObject::setrgbpalette(char color, unsigned r, unsigned g, unsigned b)
  // This sets the Red, Green and Blue values of a certain color
{
	asm {
   	mov    dx, 0x3c8
      mov    al, [color]
      out    dx, al
      inc    dx
      mov    al, [r]
      out    dx, al
      mov    al, [g]
      out    dx, al
      mov    al, [b]
      out    dx, al
	}
}

void GraphObject::getrgbpalette(char color, unsigned *r, unsigned *g, unsigned *b)
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
   *r = rr;
   *g = gg;
   *b = bb;
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
   setmem(MK_FP(VGAaddr, m+ys*320), a+m>319?320-m:a+1, _p_graphobject_drawcolor);
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
void GraphObject::makevirtual(void **VScr)
{
   #pragma warn -cln
	*VScr = farmalloc(64000);
   #pragma warn .cln
}

void GraphObject::clearvirtual(void **Vscr)
{
	farfree(*Vscr);
}

void GraphObject::clearviewport(void *VScr)
{
	#pragma warn -cln
   #pragma warn -sig
	setmem(MK_FP(FP_SEG(VScr), 0), 64000, _p_graphobject_backcolor);
   #pragma warn .sig
   #pragma warn .cln
}

void GraphObject::putpixel(int x, int y, char color, void *VScr)
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

char GraphObject::getpixel(int x, int y, void *VScr)
{
	if (x < 0) return 0;
   if (x >= 320) return 0;
   if (y < 0) return 0;
   if (y >= 200) return 0;

   return peek(FP_SEG(VScr), x+y*320);
}

void GraphObject::getpixel(int x, int y, char *color, void *VScr)
{
	if (x < 0) { *color = 0; return; }
   if (x >= 320) { *color = 0; return; }
   if (y < 0) { *color = 0; return; }
   if (y >= 200) { *color = 0; return; }

   *color = peek(FP_SEG(VScr), x+y*320);
}

void GraphObject::putimage(int x, int y, void *image, char mode, void *VScr)
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

void GraphObject::getimage(int xs, int ys, int xe, int ye, void *image, void *VScr)
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

void GraphObject::hline(int xs, int ys, int xe, int ye, void *VScr)
{
	if (ys != ye) return;
   int m = min(xs, xe);
	int a = abs(xs-xe);
   setmem(MK_FP(FP_SEG(VScr), m+ys*320), a+m>319?320-m:a+1, _p_graphobject_drawcolor);
}

void GraphObject::vline(int xs, int ys, int xe, int ye, void *VScr)
{
	if (xs != xe) return;

   int start = min(ys, ye);
   int end = start + abs(ys-ye);
	for (int i = start; i <= end; i++)
   	putpixel(xs, i, _p_graphobject_drawcolor, VScr);
}

void GraphObject::line(int xs, int ys, int xe, int ye, void *VScr)
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

void GraphObject::rectangle(int xs, int ys, int xe, int ye, void *VScr)
{
	hline(xs, ys, xe, ys, VScr);
   hline(xs, ye, xe, ye, VScr);
   vline(xs, ys, xs, ye, VScr);
   vline(xe, ys, xe, ye, VScr);
}

void GraphObject::bar(int xs, int ys, int xe, int ye, void *VScr)
{
	int ymin = min(ys, ye);
   int ymax = ymin + abs(ye-ys);

   char oldcolor = GraphObject::getcolor();
   GraphObject::setcolor(_p_graphobject_fillcolor);
	for (int row = ymin; row <= ymax; row++)
   	hline(xs, row, xe, row, VScr);
   GraphObject::setcolor(oldcolor);
}
