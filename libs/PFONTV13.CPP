//========================== Implementation ==============================
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <iostream.h>
#include <stdlib.h>
#include <new.h>
#include <math.h>
#include <string.h>

#include ".\PFONTV13.H"


//~~~~~~~~~~~~~~~~~~~ Beginning of FontObject ~~~~~~~~~~~~~~~~~~~~~~


//************************ PsClearScr ******************************

void FontObject::ClrScr(unsigned color)

{
   setfillstyle(SOLID_FILL, color); //sets the fill color to the one passed
   setcolor(color);                 //sets the draw color to the one passed
   bar(0, 0, 639, 479);             //draws a huge rectangle around the screen
   movetextcursor(2, 2);            //moves text cursor to (2, 2)
}

//////////////////////////////////////////////////////////////////////
//            CHARACTER DRAWING ROUTINES START HERE                 //
//            ALL CHARS ARE DRAWN WITH LINES AND/OR                 //
//            ARCS.                                                 //
//////////////////////////////////////////////////////////////////////

void FontObject::Draw1(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX), x + size, floor(y + size * YDIVX));
     line(x + size / 2, y, x + size / 2, floor(y + size * YDIVX));
     line(x, floor(y + size * YDIVX / 6), x + size / 2, y);
}

void FontObject::Draw2(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(x + size / 2, y + size / 2, floor(330 - 2 * (atan(2 * YDIVX - 1))), 180, size / 2);
     moveto(x, floor(y + size * YDIVX));
     linerel(floor((size*(YDIVX-1/2)*sin(2*atan(2))))+floor(size/7)-1,floor((size*(YDIVX-1/2)*cos(2 * atan(2)))));
     line(x, floor(y + size * YDIVX), x + size, floor(y + size * YDIVX));
}

void FontObject::Draw3(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 2, floor(y + size * YDIVX / 4), 270, 180, size / 2, floor(size * YDIVX / 4));
     ellipse(x + size / 2, floor(y + size * 3 * YDIVX / 4), 180, 90, size / 2, floor(size * YDIVX / 4));
}

void FontObject::Draw4(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX / 2), x + size, floor(y + size * YDIVX / 2));
     line(x + 3 * size / 4, y, x + 3 * size / 4, floor(y + size * YDIVX));
     line(x + 3 * size / 4, y, x, floor(y + size * YDIVX / 2));
}

void FontObject::Draw5(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 2, floor(y + size * 3 * YDIVX / 4), 180, 90, size / 2, floor(size * YDIVX / 4));
     line(x, y, x, floor(y + size * YDIVX / 2));
     line(x, y, x + size, y);
     line(x, floor(y + size * YDIVX / 2), x + size / 2, floor(y + size * YDIVX / 2));
}

void FontObject::Draw6(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 2, floor(y + size * 3 * YDIVX / 4), 0, 360, size / 2, floor(size * YDIVX / 4));
     arc(floor(x + size / 2), floor(y + size / 2), 90, 180, floor(size / 2));
     line(x, floor(y + size * YDIVX / 4), x, floor(y + 3 * size * YDIVX / 4));
}

void FontObject::Draw7(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x + size, y);
     line(x, floor(y + size * YDIVX), x + size, y);
}

void FontObject::Draw8(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 2, floor(y + size * YDIVX / 4), 0, 360, size / 2, floor(size * YDIVX / 4));
     ellipse(x + size / 2, floor(y + size * 3 * YDIVX / 4), 0, 360, size / 2, floor(size * YDIVX / 4));
}

void FontObject::Draw9(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 2, floor(y + size * YDIVX / 4), 0, 360, size / 2, floor(size * YDIVX / 4));
     line(x + size / 2, floor(y + size * YDIVX), x + size, floor(y + size * YDIVX / 4));
}

void FontObject::Draw0(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 2, floor(y + size * YDIVX / 2), 0, 360, size / 2, floor(size * YDIVX / 2));
}

void FontObject::DrawSPC(int x, int y, unsigned size)

{
   setfillstyle(SOLID_FILL, 0);
   bar(x, y, x + size, y + floor(size * YDIVX));
}

//********************** DrawEXC ***************************

void FontObject::DrawEXC(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x + size / 2, y, x + size / 2, floor(y + 2 * size * YDIVX / 3));
     putpixel(x + size / 2, floor(y + size * YDIVX), color);
}

//********************* Draw# **************************

void FontObject::DrawNUM(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX / 3), x +  size, floor(y + size * YDIVX / 3));
     line(x, floor(y + 2 * size * YDIVX / 3), x +  size, floor(y + 2 * size * YDIVX / 3));
     line(x + size / 3, y, x + size / 3, floor(y + size * YDIVX));
     line(x + 2 * size / 3, y, x + 2 * size / 3, floor(y + size * YDIVX));
}

//************************ DrawQUES ******************************

void FontObject::DrawQUES(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size / 2), floor(y + size / 2), 0, 180, floor(size / 2));
     ellipse(x + 3 * size / 4, y + size / 2, 270, 360, size / 4, floor(size * YDIVX / 2 - size / 2));
     ellipse(x+3*size / 4, y+size / 2+2*floor(size*YDIVX/2-size/2), 90, 180, size / 4, floor(size*YDIVX/2-size/2));
     line(x + size / 2, y + size / 2 + 2 * floor(size*YDIVX/2-size/2), x + size / 2, floor(y + float(5) / float(6) * size * YDIVX));
     putpixel(x + size / 2, floor(y + size * YDIVX), color);
}

//*********************** DrawDLR *****************************

void FontObject::DrawDLR(int x, int y, unsigned size, unsigned color)

{
     DrawS(x + 1, y + 1, size - 2, color);
     line(x + size / 2, y, x + size / 2, floor(y + size * YDIVX));
}

//************************ DrawPRC ****************************

void FontObject::DrawPRC(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x + size, y, x, floor(y + size * YDIVX));
     arc(x + size / 8, y + size / 8, 0, 360, size / 8);
     arc(x + 7 * size / 8, floor(y + size * YDIVX - size / 8), 0, 360, size / 8);
}

//************************ DrawBSH ******************************

void FontObject::DrawBSH(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x + size, floor(y + size * YDIVX));
}

//************************ Draw" ************************************

void FontObject::DrawODBQT(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 4, y + size / 8, size / 8, size / 8);
     fillellipse(x + 3 * size / 4, y + size / 8, size / 8, size / 8);
     ellipse(x + 3 * size / 8 - 1, y + size / 8 + 1, 180, 270, size / 8, floor(size * YDIVX / 6));
     ellipse(x + 7 * size / 8 - 1, y + size / 8 + 1, 180, 270, size / 8, floor(size * YDIVX / 6));
}

void FontObject::DrawCDBQT(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 4, y + size / 6 + 1 + floor(size * YDIVX / 6) - size / 8, size / 8, size / 8);
     fillellipse(x + 3 * size / 4, y + size / 6 + 1 + floor(size * YDIVX / 6) - size / 8, size / 8, size / 8);
     ellipse(x + size / 4, y + size / 6, 330, 90, size / 8, floor(size * YDIVX / 6));
     ellipse(x + 3 * size / 4, y + size / 8, 330, 90, size / 8, floor(size * YDIVX / 6));
}

void FontObject::DrawOSIQT(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 2, y + size / 8, size / 8, size / 8);
     ellipse(x + 5 * size / 8 - 1, y + size / 8 + 1, 180, 270, size / 8, floor(size * YDIVX / 6));
}

void FontObject::DrawCSIQT(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 2, y + size / 6 + 1 + floor(size * YDIVX / 6) - size / 8, size / 8, size / 8);
     ellipse(x + size / 2, y + size / 8, 330, 90, size / 8, floor(size * YDIVX / 6));
}

//****************************  (..) ****************************

void FontObject::DrawOPR(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + 3 * size / 4, floor(y + size / 2 * YDIVX), 90, 270, size / 4, floor(size / 2 * YDIVX));
}

void FontObject::DrawCPR(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + 3 * size / 4, floor(y + size / 2 * YDIVX), 270, 90, size / 4, floor(size / 2 * YDIVX));
}

//***************************** Draw *, X *********************************

void FontObject::DrawAST(int x, int y, unsigned size, unsigned color)

{
   setcolor(color);

   line(x, y + floor(size * YDIVX / 2), x + size, y + floor(size * YDIVX / 2));
   line(x + size / 4, y + floor(size * YDIVX / 2 - sqrt(3) / 4 * size), x + 3 * size / 4, y + floor(size * YDIVX / 2 + sqrt(3) / 4 * size));
   line(x + size / 4, y + floor(size * YDIVX / 2 + sqrt(3) / 4 * size), x + 3 * size / 4, y + floor(size * YDIVX / 2 - sqrt(3) / 4 * size));
}

void FontObject::DrawTIMES(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x + size / 8, floor(y + size / 2 * YDIVX - 3 * size / 8), x + 7 * size / 8, floor(y + size/2*YDIVX+3*size/8));
     line(x + 7 * size / 8, floor(y + size / 2 * YDIVX - 3 * size / 8), x + size / 8, floor(y + size/2*YDIVX+3*size/8));
}

//*********************** DrawPLS *******************************

void FontObject::DrawPLS(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX / 2), x + size, floor(y + size * YDIVX / 2));
     line(x + size / 2, floor(y + size * YDIVX / 2 - size / 2), x + size / 2, floor(y + size * YDIVX / 2 + size / 2));
}

//*********************** DrawCMA *******************************

void FontObject::DrawCMA(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 2, floor(y + size * YDIVX - float(size) / float(6)) - 1, size / 8, size / 8);
     ellipse(x + 5 * size / 8 - 1, floor(y + size * YDIVX + float(size) / float(6) - 1) - 1, 270, 90, size / 8, floor(size * YDIVX / 6));
}

//************************* DrawMNS *****************************

void FontObject::DrawMNS(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX / 2), x + size, floor(y + size * YDIVX / 2));
}

//********************** DrawPRD **************************

void FontObject::DrawPRD(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 2, floor(y + size * YDIVX - size / 8), size / 8, size / 8);
}

//********************** Draw /,  / ******************************

void FontObject::DrawSLSH(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX), x + size, y);
}

void FontObject::DrawDIV(int x, int y, unsigned size, unsigned color)

{
     DrawMNS(x, y, size, color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 2, floor(y + 3/4 * size * YDIVX), size / 8, size / 8);
     fillellipse(x + size / 2, floor(y + 1/4 * size * YDIVX), size / 8, size / 8);
}

//******************* Draw CLN, SMICLN ********************************

void FontObject::DrawCLN(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 2, floor(y + float(3)/float(4) * size * YDIVX), size / 8, size / 8);
     fillellipse(x + size / 2, floor(y + float(1)/float(4) * size * YDIVX), size / 8, size / 8);
}

void FontObject::DrawSMICLN(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     setfillstyle(SOLID_FILL, color);
     fillellipse(x + size / 2, floor(y + float(1)/float(4) * size * YDIVX), size / 8, size / 8);
     DrawCMA(x, floor(y - size * YDIVX / 4 + size / 8), size, color);
}

//****************** DrawLES ***********************

void FontObject::DrawLES(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x + size, floor(y + size * YDIVX / 4), x, floor(y + size * YDIVX / 2));
     line(x + size, floor(y + 3 * size * YDIVX / 4), x, floor(y + size * YDIVX / 2));
}

void FontObject::DrawGRE(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX / 4), x + size, floor(y + size * YDIVX / 2));
     line(x, floor(y + 3 * size * YDIVX / 4), x + size, floor(y + size * YDIVX / 2));
}

void FontObject::DrawLESOREQ(int x, int y, unsigned size, unsigned color)

{
     DrawLES(x, y, size, color);
     line(x + size, floor(y + 3 * size * YDIVX / 4 + 1), x, floor(y + size * YDIVX / 2 + 1));
}

void FontObject::DrawGREOREQ(int x, int y, unsigned size, unsigned color)

{
     DrawGRE(x, y, size, color);
     line(x, floor(y + 3 * size * YDIVX / 4 + 1), x + size, floor(y + size * YDIVX / 2 + 1));
}

void FontObject::DrawEQU(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX * 1 / 3), x + size, floor(y + size * YDIVX * 1 / 3));
     line(x, floor(y + size * YDIVX * 2 / 3), x + size, floor(y + size * YDIVX * 2 / 3));
}

void FontObject::DrawSIM(int x, int y, unsigned size, unsigned color)

            //   \~

{
     setcolor(color);
     ellipse(x + size / 4, floor(y + size * YDIVX / 2), 180, 360, size / 4, floor(size * YDIVX / 8));
     ellipse(x + 3 * size / 4, floor(y + size * YDIVX / 2), 0, 180, size / 4, floor(size * YDIVX / 8));
}

void FontObject::DrawAPPROX(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 4, floor(y + size * YDIVX / 2 - 1), 180, 360, size / 4, floor(size * YDIVX / 8));
     ellipse(x + 3 * size / 4, floor(y + size * YDIVX / 2 - 1), 0, 180, size / 4, floor(size * YDIVX / 8));
     ellipse(x + size / 4, floor(y + size * YDIVX / 2 + 1), 180, 360, size / 4, floor(size * YDIVX / 8));
     ellipse(x + 3 * size / 4, floor(y + size * YDIVX / 2 + 1), 0, 180, size / 4, floor(size * YDIVX / 8));
}

//****************** Draw Brackets *******************

void FontObject::DrawOBR(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x + 3 * size / 8, y, x + 3 * size / 8, floor(y + size * YDIVX));
     line(x + 3 * size / 8, y, x + 5 * size / 8, y);
     line(x + 3 * size / 8, floor(y + size * YDIVX), x + 5 * size / 8, floor(y + size * YDIVX));
}

void FontObject::DrawCBR(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x + 5 * size / 8, y, x + 5 * size / 8, floor(y + size * YDIVX));
     line(x + 3 * size / 8, y, x + 5 * size / 8, y);
     line(x + 3 * size / 8, floor(y + size * YDIVX), x + 5 * size / 8, floor(y + size * YDIVX));
}

//******************** Draw Carat ***********************

void FontObject::DrawCAR(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x + size / 2, y, x + size, floor(y + size * YDIVX * 3/8));
     line(x + size / 2, y, x, floor(y + size * YDIVX * 3/8));
}

//******************* Draw Underscore *************************

void FontObject::DrawUND(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, floor(y + size * YDIVX), x + size, floor(y + size * YDIVX));
}

//********************* Draw Braces **************************

void FontObject::DrawOBRACE(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + 5 * size / 8, floor(y + size * YDIVX / 4), 90, 180, size / 8, floor(size * YDIVX / 4));
     ellipse(x + 5 * size / 8, floor(y + 3 * size * YDIVX / 4), 180, 270, size / 8, floor(size * YDIVX / 4));
     arc(x + 3 * size / 8, floor(y + size * YDIVX / 2 - size / 8), 270, 360, size / 8);
     arc(x + 3 * size / 8, floor(y + size * YDIVX / 2 + size / 8), 0, 90, size / 8);
     line(x + size / 2, floor(y + size * YDIVX / 4), x + size / 2, floor(y + size * YDIVX / 2 - size / 8));
     line(x + size / 2, floor(y + 3 * size * YDIVX / 4), x + size / 2, floor(y + size * YDIVX / 2 + size / 8));
}

void FontObject::DrawCBRACE(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + 3 * size / 8, floor(y + size * YDIVX / 4), 0, 90, size / 8, floor(size * YDIVX / 4));
     ellipse(x + 3 * size / 8, floor(y + 3 * size * YDIVX / 4), 270, 360, size / 8, floor(size * YDIVX / 4));
     arc(x + 5 * size / 8, floor(y + size * YDIVX / 2 - size / 8), 180, 270, size / 8);
     arc(x + 5 * size / 8, floor(y + size * YDIVX / 2 + size / 8), 90, 180, size / 8);
     line(x + size / 2, floor(y + size * YDIVX / 4), x + size / 2, floor(y + size * YDIVX / 2 - size / 8));
     line(x + size / 2, floor(y + 3 * size * YDIVX / 4), x + size / 2, floor(y + size * YDIVX / 2 + size / 8));
}

//********************* Draw Tilde ***************************

void FontObject::DrawTIL(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 4, floor(y + size * YDIVX / 8), 180, 360, size / 4, floor(size * YDIVX / 8));
     ellipse(x + 3 * size / 4, floor(y + size * YDIVX / 8), 0, 180, size / 4, floor(size * YDIVX / 8));
}

//********************** DrawA ***************************

void FontObject::DrawA(int x, int y, unsigned size, unsigned color)

{
   setcolor(color);
   arc(floor(x + size / 2), floor(y + size / 2), 0, 180, floor(size / 2));
   line(x, floor(y + size / 2), x, floor(y + YDIVX * size));
   line(x + size, floor(y + size / 2), x + size, floor(y + YDIVX * size));
   line(x, floor(y + YDIVX * size / 2), x + size, floor(y + YDIVX * size / 2));
}

//********************** DrawB *****************************

void FontObject::DrawB(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size - (size * YDIVX / 4)), floor(y + (size * YDIVX / 4)), 270, 90, floor(size * YDIVX / 4));
     arc(floor(x + size - (size * YDIVX / 4)), floor(y + 3 * (size * YDIVX / 4)), 270, 90, floor(size * YDIVX / 4));
     line(x, y, x, floor(y + size * YDIVX));
     line(x, y, floor(x + size - (size * YDIVX / 4)), y);
     line(x, floor(y + YDIVX * size / 2), floor(x + size - (size * YDIVX / 4)), floor(y + YDIVX * size / 2));
     line(x, floor(y + size * YDIVX), floor(x + size - (size * YDIVX / 4)), floor(y + size * YDIVX));
}

//******************* DrawC ******************************

void FontObject::DrawC(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size / 2), floor(y + size / 2), 30, 180, floor(size / 2));
     arc(floor(x + size / 2), floor(y + size * YDIVX - size / 2), 180, 330, floor(size / 2));
     line(x, y + size / 2, x, floor(y + size * YDIVX - size / 2));
}

//******************* DrawD ******************************

void FontObject::DrawD(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size - (size * YDIVX / 4)), floor(y + (size * YDIVX / 4)), 0, 90, floor(size * YDIVX / 4));
     arc(floor(x + size - (size * YDIVX / 4)), floor(y + 3 * (size * YDIVX / 4)), 270, 0, floor(size * YDIVX / 4));
     line(floor(x+size), floor(y+size*YDIVX/4), floor(x+size), floor(y+YDIVX*size-size*YDIVX/4));
     line(x, y, x, floor(y + size * YDIVX));
     line(x, y, floor(x + size - (size * YDIVX / 4)), y);
     line(x, floor(y + size * YDIVX), floor(x + size - (size * YDIVX / 4)), floor(y + size * YDIVX));
}

//************************* DrawE ******************************

void FontObject::DrawE(int x, int y, unsigned size, unsigned color)

{
   setcolor(color);
   line(x, y, x, floor(y + size * YDIVX));
   line(x, y, x + size, y);
   line(x, floor(y + size * YDIVX / 2), floor(x + float(5) / float(6) * size), floor(y + size * YDIVX / 2));
   line(x, floor(y + size * YDIVX), x + size, floor(y + size * YDIVX));
}

//************************ DrawF *****************************

void FontObject::DrawF(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x, floor(y + size * YDIVX));
     line(x, y, x + size, y);
     line(x, floor(y + size * YDIVX / 2), floor(x + float(5) / float(6) * size), floor(y + size * YDIVX / 2));
}

//**************************** DrawG *********************************

void FontObject::DrawG(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size / 2), floor(y + size / 2), 30, 180, floor(size / 2));
     arc(floor(x + size / 2), floor(y + size * YDIVX - size / 2), 180, 360, floor(size / 2));
     line(x, y + size / 2, x, floor(y + size * YDIVX - size / 2));
     line(x + size, floor(y + size * YDIVX - size / 2), x + size, floor(y + size * YDIVX / 2));
     line(x + size / 2, floor(y + YDIVX * size / 2), x + size, floor(y + YDIVX * size / 2));
}

//*************************** DrawH ***********************************

void FontObject::DrawH(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x, floor(y + size * YDIVX));
     line(x + size, y, x + size, floor(y + size * YDIVX));
     line(x, floor(y + size * YDIVX / 2), floor(x + size), floor(y + size * YDIVX / 2));
}

//************************ DrawI ***************************

void FontObject::DrawI(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x + size, y);
     line(x, floor(y + size * YDIVX), x + size, floor(y + size * YDIVX));
     line(x + size / 2, y, x + size / 2, floor(y + size * YDIVX));
}

//********************** DrawJ *************************

void FontObject::DrawJ(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size / 2), floor(y + size * YDIVX - size / 2), 180, 360, floor(size / 2));
     line(x + size, y, x + size, floor(y + size * YDIVX - size / 2));
     line(x, floor(y + size * YDIVX / 2), x, floor(y + size * YDIVX - size / 2));
}

//********************* DrawK **************************

void FontObject::DrawK(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x, floor(y + size * YDIVX));
     line(x, floor(y + size * YDIVX / 2), x + size, y);
     line(x, floor(y + size * YDIVX / 2), x + size, floor(y + size * YDIVX));
}

//******************** DrawL ***************************

void FontObject::DrawL(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x, floor(y + size * YDIVX));
     line(x, floor(y + size * YDIVX), x + size, floor(y + size * YDIVX));
}

//******************** DrawM *****************************

void FontObject::DrawM(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x, floor(y + size * YDIVX));
     line(x + size, y, x + size, floor(y + size * YDIVX));
     line(x, y, x + size / 2, floor(y + size * YDIVX / 2));
     line(x + size, y, x + size / 2, floor(y + size * YDIVX / 2));
}

//************************ DrawN *******************************

void FontObject::DrawN(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x, floor(y + size * YDIVX));
     line(x + size, y, x + size, floor(y + size * YDIVX));
     line(x, y, x + size, floor(y + size * YDIVX));
}

//************************ DrawO **********************************

void FontObject::DrawO(int x, int y, unsigned size, unsigned color)

{
   setcolor(color);
   arc(floor(x + size / 2), floor(y + size / 2), 0, 180, floor(size / 2));
   arc(floor(x + size / 2), floor(y + size * YDIVX - size / 2), 180, 360, floor(size / 2));
   line(x, y + size / 2, x, floor(y + size * YDIVX - size / 2));
   line(x + size, y + size / 2, x + size, floor(y + size * YDIVX - size / 2));
}

//******************* DrawP ***************************

void FontObject::DrawP(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size - (size * YDIVX / 4)), floor(y + (size * YDIVX / 4)), 270, 90, floor(size * YDIVX / 4));
     line(x, y, x, floor(y + size * YDIVX));
     line(x, y, floor(x + size - (size * YDIVX / 4)), y);
     line(x, floor(y + YDIVX * size / 2), floor(x + size - (size * YDIVX / 4)), floor(y + YDIVX * size / 2));
}

//******************* DrawQ *****************************

void FontObject::DrawQ(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size / 2), floor(y + size / 2), 0, 180, floor(size / 2));
     arc(floor(x + size / 2), floor(y + size * YDIVX - size / 2), 180, 360, floor(size / 2));
     line(x, y + size / 2, x, floor(y + size * YDIVX - size / 2));
     line(x + size, y + size / 2, x + size, floor(y + size * YDIVX - size / 2));
     line(floor(x + size / 2), floor(y + size * YDIVX - size / 2), x + size, floor(y + size * YDIVX));
}

//******************* DrawR ***************************

void FontObject::DrawR(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size - (size * YDIVX / 4)), floor(y + (size * YDIVX / 4)), 270, 90, floor(size * YDIVX / 4));
     line(x, y, x, floor(y + size * YDIVX));
     line(x, y, floor(x + size - (size * YDIVX / 4)), y);
     line(x, floor(y + YDIVX * size / 2), floor(x + size - (size * YDIVX / 4)), floor(y + YDIVX * size / 2));
     line(x, floor(y + size * YDIVX / 2), x + size, floor(y + size * YDIVX));
}

//********************** DrawS ***************************

void FontObject::DrawS(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     ellipse(x + size / 2, floor(y + size * YDIVX / 4), 0, 270, size / 2, floor(size * YDIVX / 4));
     ellipse(x + size / 2, floor(y + size * 3 * YDIVX / 4), 180, 90, size / 2, floor(size * YDIVX / 4));
}

//********************** DrawT ***************************

void FontObject::DrawT(int x, int y, unsigned size, unsigned color)

{
   setcolor(color);
   line(x, y, x + size, y);
   line(x + size / 2, y, x + size / 2, floor(y + size * YDIVX));
}

//********************** DrawU ***************************

void FontObject::DrawU(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size / 2), floor(y + size * YDIVX - size / 2), 180, 360, floor(size / 2));
     line(x + size, y, x + size, floor(y + size * YDIVX - size / 2));
     line(x, y, x, floor(y + size * YDIVX - size / 2));
}

//********************** DrawV ****************************

void FontObject::DrawV(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x + size / 2, floor(y + size * YDIVX));
     line(x + size, y, x + size / 2, floor(y + size * YDIVX));
}

//************************** DrawW *************************

void FontObject::DrawW(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x + size / 4, floor(y + size * YDIVX));
     line(x + size, y, x + 3 * size / 4, floor(y + size * YDIVX));
     line(x + size / 2, floor(y + size * YDIVX / 2), x + size / 4, floor(y + size * YDIVX));
     line(x + size / 2, floor(y + size * YDIVX / 2), x + 3 * size / 4, floor(y + size * YDIVX));
}

//********************** DrawX ***************************

void FontObject::DrawX(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x + size, floor(y + size * YDIVX));
     line(x, floor(y + size * YDIVX), x + size, y);
}

//********************** DrawY **************************

void FontObject::DrawY(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     arc(floor(x + size / 2), floor(y + size * YDIVX / 2 - size / 2), 180, 0, size / 2);
     line(floor(x + size / 2), floor(y + size * YDIVX / 2), x + size / 2, floor(y + size * YDIVX));
     line(x, y, x, floor(y + size * YDIVX / 2 - size / 2));
     line(x + size, y, x + size, floor(y + size * YDIVX / 2 - size / 2));
}

//************************* DrawZ *****************************

void FontObject::DrawZ(int x, int y, unsigned size, unsigned color)

{
     setcolor(color);
     line(x, y, x + size, y);
     line(x, floor(y + size * YDIVX), x + size, floor(y + size * YDIVX));
     line(x + size, y, x, floor(y + size * YDIVX));
}

/////////////////////////////////////////////////////////////////////
//          CHARACTER DRAWING ROUTINES STOP HERE                   //
/////////////////////////////////////////////////////////////////////

//**************************** Init Graph Letts **********************

void Abort(char *Msg)  //quits

{
   cerr << Msg << ": " << grapherrormsg(graphresult()) << "\n";
   exit(1);          //Shows message and quits with a DOS exit code of 1
}

FontObject::FontObject(float YDX, int tabl, int abf, int abt)

{
   int ErCd, GrMd, GrDr;

   //Registers all the drivers

   if (registerfarbgidriver(EGAVGA_driver_far) < 0)
      Abort("VGA Error!");

   //Registers all the fonts

   if (registerfarbgifont(gothic_font_far) < 0)
      Abort("Gothic Font Error!");
   if (registerfarbgifont(sansserif_font_far) < 0)
      Abort("SansSerif Font Error!");
   if (registerfarbgifont(small_font_far) < 0)
      Abort("Small Font Error!");
   if (registerfarbgifont(triplex_font_far) < 0)
      Abort("Triplex Font Error!");

   GrDr = VGA;
   GrMd = VGAHI;

   initgraph(&GrDr, &GrMd, "");  //activates graphics

   ErCd = graphresult();      //finds the error code(s)
   if (ErCd != grOk)
   {
      cerr << "Graphics error!  VGA graphics could not be initialized!\n";
      cerr << "Error is: " << grapherrormsg(ErCd) << ".\n";
      cerr << "\n\nPress a key.\n";
      getch();
      exit(1);
   }

   YDIVX                = YDX;      //sets all private vars. to the values passed
   tablength            = tabl;
   alarmbellfrequency   = abf;
   alarmbelltime        = abt;

   movetextcursor(2, 2);   //moves text cursor to (2, 2)
   setcurrcolor(12);       //sets text color to light red
   setcurrsize(10);        //sets text size to 10 pixels wide
}

//*********************** MoveTextCursor **************************

void FontObject::movetextcursor(int x, int y)
//moves text cursor to the values passed

{
   textcursorx = x;   //sets private vars. to the passed values
   textcursory = y;
}

//************************ GetTextcursorXY ***************************

void FontObject::gettextcursorXY(int *x, int *y)
//returns the values of the private vars. of the text position

{
     *x = textcursorx; //Assigns the vals. of the var. params. to the values
     *y = textcursory; // of the private vars.
}

//**************************** Set Current Size ******************************

void FontObject::setcurrsize(unsigned size)
//sets the value of the private var. size to the value passed

{
   currentsize = size; //assigns the value of the private var. to the passed value
}

//********************* Set Current Color *************************

void FontObject::setcurrcolor(unsigned color)
//sets the value of the private var. color to the value passed

{
   currentcolor = color; //assigns the value of the private var. to the passed value
}

//************************** Get Current Size ****************************

void FontObject::getcurrsize(unsigned *size)
//returns the value of the private var. size

{
   *size = currentsize; //assigns the value of the var. param. to the value of the private var.
}

//*********************** Get Current Color *****************************

void FontObject::getcurrcolor(unsigned *color)
//returns the text color in the var. param

{
   *color = currentcolor; //sets the value of the var. param. to the value of the private var.
}

//******************************* Destruct **********************************

FontObject::~FontObject()

{
   closegraph();                //exits graphics mode
   textcolor(LIGHTGRAY);      //sets normal text color to bright white
   textbackground(BLACK);     //sets normal text background to black
}


//************************* Ps Printf *******************************

char UpCase(char ch)
{
   if ((ch >= 'a') && (ch <= 'z'))
      return ch + 'A' - 'a';
   else
       return ch;
}

void FontObject::write(int x, int y, int siz, int col, char *chrstr)
//writes a string of characters to the screen.  Pass it a null terminated string

{
   int ctr = 0, ctr1;         //loop counters
   int xpos, ypos;            //position counters
   byte backslash = 0;        //backslash boolean flag
   byte quote = 0;            //quote boolean flag
   unsigned size, color;      //size & color vars

   if ((x >= 0) && (y >= 0))
   {
      xpos = x;            //if new vals. are passed...
      ypos = y;
   }
   else
   {        //if neg. vals. are passed, then private vals. are used
      gettextcursorXY(&xpos, &ypos);
      x = xpos;   y = ypos;
   }

   if (col < 0)       //if neg. vals. are passed, then private vals. are used
      getcurrcolor(&color);
   else            //if new vals. are passed...
       color = col;
   if (siz >= 0)   //if new vals are passed...
      size = siz;
   else              //if neg. vals. are passed, then private vals. are used
       getcurrsize(&size);

   while ((chrstr[ctr] >= 32) && (chrstr[ctr] <= 126) && (ctr <= strlen(chrstr) - 1))
   {
      switch (UpCase(chrstr[ctr]))
      {
         case 'A' : if (backslash)
                    {
                       sound(alarmbellfrequency);
                       delay(alarmbelltime);
                       nosound();
                       xpos = xpos - size - size / 3 - 1;
                       backslash = 0;
                    }
                    else
                        DrawA(xpos, ypos, size, color);
                    break;
         case 'B' : DrawB(xpos, ypos, size, color);
                    break;
         case 'C' : DrawC(xpos, ypos, size, color);
                    break;
         case 'D' : DrawD(xpos, ypos, size, color);
                    break;
         case 'E' : DrawE(xpos, ypos, size, color);
                    break;
         case 'F' : DrawF(xpos, ypos, size, color);
                    break;
         case 'G' : DrawG(xpos, ypos, size, color);
                    break;
         case 'H' : DrawH(xpos, ypos, size, color);
                    break;
         case 'I' : DrawI(xpos, ypos, size, color);
                    break;
         case 'J' : DrawJ(xpos, ypos, size, color);
                    break;
         case 'K' : DrawK(xpos, ypos, size, color);
                    break;
         case 'L' : DrawL(xpos, ypos, size, color);
                    break;
         case 'M' : DrawM(xpos, ypos, size, color);
                    break;
         case 'N' : if (backslash)
                    {
                       ypos = ypos + floor(size * YDIVX) + size / 2 + 1;
                       xpos = xpos - size - size / 3 - 1;
                       backslash = 0;
                    }
                    else
                        DrawN(xpos, ypos, size, color);
                    break;
         case 'O' : DrawO(xpos, ypos, size, color);
                    break;
         case 'P' : DrawP(xpos, ypos, size, color);
                    break;
         case 'Q' : DrawQ(xpos, ypos, size, color);
                    break;
         case 'R' : if (backslash)
                    {
                       ypos = ypos + floor(size * YDIVX) + size / 2 + 1;
                       xpos = x % (size + size / 3 + 1) - size - size / 3 - 1;
                       backslash = 0;
                    }
                    else
                        DrawR(xpos, ypos, size, color);
                    break;
         case 'S' : DrawS(xpos, ypos, size, color);
                    break;
         case 'T' : if (backslash)
                    {
                       xpos = xpos + (size + size / 3 + 1) * (tablength - 1);
                       backslash = 0;
                    }
                    else
                        DrawT(xpos, ypos, size, color);
                    break;
         case 'U' : DrawU(xpos, ypos, size, color);
                    break;
         case 'V' : DrawV(xpos, ypos, size, color);
                    break;
         case 'W' : DrawW(xpos, ypos, size, color);
                    break;
         case 'X' : DrawX(xpos, ypos, size, color);
                    break;
         case 'Y' : DrawY(xpos, ypos, size, color);
                    break;
         case 'Z' : DrawZ(xpos, ypos, size, color);
                    break;
         case '0' : Draw0(xpos, ypos, size, color);
                    break;
         case '1' : Draw1(xpos, ypos, size, color);
                    break;
         case '2' : Draw2(xpos, ypos, size, color);
                    break;
         case '3' : Draw3(xpos, ypos, size, color);
                    break;
         case '4' : Draw4(xpos, ypos, size, color);
                    break;
         case '5' : Draw5(xpos, ypos, size, color);
                    break;
         case '6' : Draw6(xpos, ypos, size, color);
                    break;
         case '7' : Draw7(xpos, ypos, size, color);
                    break;
         case '8' : Draw8(xpos, ypos, size, color);
                    break;
         case '9' : Draw9(xpos, ypos, size, color);
                    break;
         case ' ' : DrawSPC(xpos, ypos, size);
                    break;
         case '!' : DrawEXC(xpos, ypos, size, color);
                    break;
         case '#' : DrawNUM(xpos, ypos, size, color);
                    break;
         case '?' : DrawQUES(xpos, ypos, size, color);
                    break;
         case '$' : DrawDLR(xpos, ypos, size, color);
                    break;
         case 37 : DrawPRC(xpos, ypos, size, color); // % sign
                     break;
         case '(' : DrawOPR(xpos, ypos, size, color);
                    break;
         case ')' : DrawCPR(xpos, ypos, size, color);
                    break;
         case '*' : if (backslash)
                    {
                       DrawTIMES(xpos, ypos, size, color);
                       backslash = 0;
                    }
                    else
                        DrawAST(xpos, ypos, size, color);
                    break;
         case '"' : if (backslash)
                    {
                       quote = 0;
                       xpos = xpos - size - size / 3 - 1;
                       backslash = 0;
                    }
                    else
                    {
                       quote ? DrawCDBQT(xpos, ypos, size, color) : DrawODBQT(xpos, ypos, size, color);
                       quote = !quote;
                    }
                    break;
         case 39 : DrawCSIQT(xpos, ypos, size, color);  //Closed single quote
                    break;
         case '`' : DrawOSIQT(xpos, ypos, size, color);
                    break;
         case '+' : DrawPLS(xpos, ypos, size, color);
                    break;
         case ',' : DrawCMA(xpos, ypos, size, color);
                    break;
         case '-' : DrawMNS(xpos, ypos, size, color);
                    break;
         case '.' : DrawPRD(xpos, ypos, size, color);
                    break;
         case '/' : if (backslash)
                    {
                       DrawDIV(xpos, ypos, size, color);
                       backslash = 0;
                    }
                    else
                        DrawSLSH(xpos, ypos, size, color);
                    break;
         case ':' : DrawCLN(xpos, ypos, size, color);
                    break;
         case ';' : DrawSMICLN(xpos, ypos, size, color);
                    break;
         case '<' : if (backslash)
                       DrawLESOREQ(xpos, ypos, size, color);
                    else
                        DrawLES(xpos, ypos, size, color);
                    break;
         case '>' : if (backslash)
                       DrawGREOREQ(xpos, ypos, size, color);
                    else
                        DrawGRE(xpos, ypos, size, color);
                    break;
         case '=' : if (backslash)
                       DrawAPPROX(xpos, ypos, size, color);
                    else
                        DrawEQU(xpos, ypos, size, color);
                    break;
         case '~' : if (backslash)
                       DrawTIL(xpos, ypos, size, color);
                    else
                        DrawSIM(xpos, ypos, size, color);
                    break;
         case '[' : DrawOBR(xpos, ypos, size, color);
                    break;
         case ']' : DrawCBR(xpos, ypos, size, color);
                    break;
         case '{' : DrawOBRACE(xpos, ypos, size, color);
                    break;
         case '}' : DrawCBRACE(xpos, ypos, size, color);
                    break;
         case '^' : DrawCAR(xpos, ypos, size, color);
                    break;
         case '_' : DrawUND(xpos, ypos, size, color);
                    break;
         case '\\' : if (backslash)
                     {
                        DrawBSH(xpos, ypos, size, color);
                        backslash = 0;
                     }
                     else
                     {
                        backslash = 1;
                        xpos = xpos - size - size / 3 - 1;
                     }
                     break;
         default : ;
      }

      xpos += size + size / 3 + 1; //increments x position by appropriate value
      if (xpos > 639 - size) //carriage return if the string exceeds the length of the screen
      {
         xpos = x % (size + size / 3 + 1);
         ypos = ypos + floor(size * YDIVX) + size / 2 + 1;
      }
      ctr++;  //increments counter by 1
   }

   movetextcursor(xpos, ypos);  //refreshes private vars.
   setcurrcolor(color);
   setcurrsize(size);
}

//****************** Write Char ***************************

void FontObject::write_ch(int x, int y, int siz, int col, char ch)

{
   int xpos = x, ypos = y;
   unsigned size = siz, color = col;

   if (x < 0)
   {
      gettextcursorXY(&xpos, &ypos);
      x = xpos; y = ypos;
   }
   if (siz < 0)
      getcurrsize(&size);
   if (col < 0)
      getcurrcolor(&color);

   switch (UpCase(ch))
   {
      case 'A' : DrawA(xpos, ypos, size, color);
                 break;
      case 'B' : DrawB(xpos, ypos, size, color);
                 break;
      case 'C' : DrawC(xpos, ypos, size, color);
                 break;
      case 'D' : DrawD(xpos, ypos, size, color);
                 break;
      case 'E' : DrawE(xpos, ypos, size, color);
                 break;
      case 'F' : DrawF(xpos, ypos, size, color);
                 break;
      case 'G' : DrawG(xpos, ypos, size, color);
                 break;
      case 'H' : DrawH(xpos, ypos, size, color);
                 break;
      case 'I' : DrawI(xpos, ypos, size, color);
                 break;
      case 'J' : DrawJ(xpos, ypos, size, color);
                 break;
      case 'K' : DrawK(xpos, ypos, size, color);
                 break;
      case 'L' : DrawL(xpos, ypos, size, color);
                 break;
      case 'M' : DrawM(xpos, ypos, size, color);
                 break;
      case 'N' : DrawN(xpos, ypos, size, color);
                 break;
      case 'O' : DrawO(xpos, ypos, size, color);
                 break;
      case 'P' : DrawP(xpos, ypos, size, color);
                 break;
      case 'Q' : DrawQ(xpos, ypos, size, color);
                 break;
      case 'R' : DrawR(xpos, ypos, size, color);
                 break;
      case 'S' : DrawS(xpos, ypos, size, color);
                 break;
      case 'T' : DrawT(xpos, ypos, size, color);
                 break;
      case 'U' : DrawU(xpos, ypos, size, color);
                 break;
      case 'V' : DrawV(xpos, ypos, size, color);
                 break;
      case 'W' : DrawW(xpos, ypos, size, color);
                 break;
      case 'X' : DrawX(xpos, ypos, size, color);
                 break;
      case 'Y' : DrawY(xpos, ypos, size, color);
                 break;
      case 'Z' : DrawZ(xpos, ypos, size, color);
                 break;
      case '0' : Draw0(xpos, ypos, size, color);
                 break;
      case '1' : Draw1(xpos, ypos, size, color);
                 break;
      case '2' : Draw2(xpos, ypos, size, color);
                 break;
      case '3' : Draw3(xpos, ypos, size, color);
                 break;
      case '4' : Draw4(xpos, ypos, size, color);
                 break;
      case '5' : Draw5(xpos, ypos, size, color);
                 break;
      case '6' : Draw6(xpos, ypos, size, color);
                 break;
      case '7' : Draw7(xpos, ypos, size, color);
                 break;
      case '8' : Draw8(xpos, ypos, size, color);
                 break;
      case '9' : Draw9(xpos, ypos, size, color);
                 break;
      case ' ' : DrawSPC(xpos, ypos, size);
                 break;
      case '!' : DrawEXC(xpos, ypos, size, color);
                 break;
      case '#' : DrawNUM(xpos, ypos, size, color);
                 break;
      case '?' : DrawQUES(xpos, ypos, size, color);
                 break;
      case '$' : DrawDLR(xpos, ypos, size, color);
                 break;
      case 37 : DrawPRC(xpos, ypos, size, color); // % sign
                 break;
      case '(' : DrawOPR(xpos, ypos, size, color);
                 break;
      case ')' : DrawCPR(xpos, ypos, size, color);
                 break;
      case '*' : DrawAST(xpos, ypos, size, color);
                 break;
      case '"' : DrawODBQT(xpos, ypos, size, color);
                 break;
      case 39 : DrawCSIQT(xpos, ypos, size, color); // Closed single quote
                break;
      case '`' : DrawOSIQT(xpos, ypos, size, color);
                 break;
      case '+' : DrawPLS(xpos, ypos, size, color);
                 break;
      case ',' : DrawCMA(xpos, ypos, size, color);
                 break;
      case '-' : DrawMNS(xpos, ypos, size, color);
                 break;
      case '.' : DrawPRD(xpos, ypos, size, color);
                 break;
      case '/' : DrawSLSH(xpos, ypos, size, color);
                 break;
      case ':' : DrawCLN(xpos, ypos, size, color);
                 break;
      case ';' : DrawSMICLN(xpos, ypos, size, color);
                 break;
      case '<' : DrawLES(xpos, ypos, size, color);
                 break;
      case '>' : DrawGRE(xpos, ypos, size, color);
                 break;
      case '=' : DrawEQU(xpos, ypos, size, color);
                 break;
      case '~' : DrawSIM(xpos, ypos, size, color);
                 break;
      case '[' : DrawOBR(xpos, ypos, size, color);
                 break;
      case ']' : DrawCBR(xpos, ypos, size, color);
                 break;
      case '{' : DrawOBRACE(xpos, ypos, size, color);
                 break;
      case '}' : DrawCBRACE(xpos, ypos, size, color);
                 break;
      case '^' : DrawCAR(xpos, ypos, size, color);
                 break;
      case '_' : DrawUND(xpos, ypos, size, color);
                 break;
      case '\\' : DrawBSH(xpos, ypos, size, color);
                  break;
      default : ;
   }

   xpos += size + size / 3 + 1; //increments x position by appropriate value
   if (xpos > 639 - size) //carriage return if the string exceeds the length of the screen
   {
      xpos = x % (size + size / 3 + 1);
      ypos = ypos + floor(size * YDIVX) + size / 2 + 1;
   }

   movetextcursor(xpos, ypos);  //refreshes private vars.
   setcurrcolor(color);
   setcurrsize(size);
}

//**************** Flash Paragraph **********************

void FontObject::p_write(int x, int y, unsigned siz, unsigned col, int xin, int yin,
                         byte rows, byte cols, int delaytime, int tail_length,
                         paratype para)

//Outputs a paragraph in "Mission Impossible" font

{
   int xpos = x, ypos = y;         //position counters
   int ctr1, ctr2, ctr3;           //loop counters

   for (ctr1 = 1; ctr1 <= rows; ctr1++) //Draws the specified num. of lines
   {
      for (ctr2 = 1; ctr2 <= cols; ctr2++) //Draws an entire line
      {
         write_ch(xpos, ypos, siz, col + 8, para[ctr1][ctr2]);
         delay(delaytime);      //pauses for the specified amount of time
         if (ctr2 > tail_length)
            write_ch(xpos - xin * tail_length, ypos, siz, col, para[ctr1][ctr2-tail_length]);

         xpos += xin; //Increments the x position by the specified amount
      }
      xpos -= xin * tail_length;
      for (ctr3 = ctr2 - tail_length + 1; ctr3 <= cols; ctr3++)
      {
         write_ch(xpos, ypos, siz, col, para[ctr1][ctr3]);
         xpos += xin;
         delay(delaytime);
      }
      xpos = x;  //carriage return to original column
      ypos += yin;  //y position is incremented by specified amount
   }
}

void pGetTime(long &hr, long &mn, long &sc, long &sc100)

{
   struct time t;

   gettime(&t);

   hr = t.ti_hour;
   mn = t.ti_min;
   sc = t.ti_sec;
   sc100 = t.ti_hund;
}

//********************** Flash Scan Function *************************

int FontObject::read(int x, int y, unsigned siz, unsigned col, unsigned timelapse, char *returnstr[])

//Inputs a string

{
   int xpos, ypos; //Pixel position vars.
   struct timetype
   {
      long hr, mn, sc, sc100;
   } lasttime, nowtime;
   long timeelapsed = 0; //Records the number of 1/100 seconds elapsed since last flash
   byte on = 0, swtch = 1; //Flags which control the flashing of the cursor
   char key = 1; //The key which the user pressed
   char tmpstr[256]; //Temporary string var.  Used to store the input string
                     //and is passed back to the calling module at the end
   int length = 1; //Contains the length of the temp. string
   byte errorcode; //Contains the error code to be passed back to the calling module
   char *retstr; //Temp. string var.
   byte stop = 0; //Stop flag
   int hiX;

   xpos = x;
   while (xpos < 639 - siz)
   {
      xpos += siz + siz / 3 + 1;
   }
   xpos -= siz + siz / 3 + 1;
   hiX = xpos;

   movetextcursor(x, y);
   setcurrsize(siz);
   setcurrcolor(col);
   setcolor(col);

   xpos = x;
   ypos = y + floor(siz * YDIVX);

   while (!stop)
   {
      if (swtch)
      {
         if (on)
            DrawSPC(xpos, ypos, siz);
         else
         {
            line(xpos, ypos + floor(siz * YDIVX), xpos + siz, ypos + floor(siz * YDIVX));
            line(xpos, ypos + floor(siz * YDIVX) - 1, xpos + siz, ypos + floor(siz * YDIVX) - 1);
         }
         on = !on;
         pGetTime(lasttime.hr, lasttime.mn, lasttime.sc, lasttime.sc100);
         swtch = 0;
      }

      pGetTime(nowtime.hr, nowtime.mn, nowtime.sc, nowtime.sc100);
      timeelapsed = 360000 * (nowtime.hr     - lasttime.hr) +
                    6000   * (nowtime.mn     - lasttime.mn) +
                    100    * (nowtime.sc     - lasttime.sc) +
                             (nowtime.sc100  - lasttime.sc100);
      if (timeelapsed >= timelapse)
         swtch = 1;

      if (kbhit())
      {
         key = getch();
         if ((key >= 32) && (key <= 126) && (key != '@') && (key != '|'))
         {
            if (length > 254)
            {
               retstr = new char[17];
               retstr = "String too long!";
               *returnstr = retstr;
               return 255;
            }
            gettextcursorXY(&xpos, &ypos);
            DrawSPC(xpos, ypos, siz);
            tmpstr[length - 1] = key;
            write_ch(-1, -1, -1, -1, key);
            length++;
            xpos += siz + siz / 3 + 1;
            if (xpos > 639 - siz) //carriage return if the string exceeds the length of the screen
            {
               xpos = x % (siz + siz / 3 + 1);
               ypos = ypos + floor(siz * YDIVX) + siz / 2 + 1;
            }
         }
         else if (key == 8) //Backspace
         {
            if (length <= 1)
               write(-1, -1, -1, -1, "\\A");
            else
            {
               length--;
               gettextcursorXY(&xpos, &ypos);
               DrawSPC(xpos, ypos, siz);
               xpos -= siz + siz / 3 + 1;
               if (xpos < 0)
               {
                  ypos -= floor(siz * YDIVX) + siz / 2 + 1;
                  xpos = hiX;
               }
               DrawSPC(xpos, ypos, siz);
               movetextcursor(xpos, ypos);
            }
         }
         else if (key == 0)
         {
            key = getch();
            if (key == 75) //Left arrow
            {
               if (length <= 1)
                  write(-1, -1, -1, -1, "\\A");
               else
               {
                  length--;
                  gettextcursorXY(&xpos, &ypos);
                  DrawSPC(xpos, ypos, siz);
                  xpos -= siz + siz / 3 + 1;
                  if (xpos < 0)
                  {
                     ypos -= floor(siz * YDIVX) + siz / 2 + 1;
                     xpos = hiX;
                  }
                  DrawSPC(xpos, ypos, siz);
                  movetextcursor(xpos, ypos);
               }
            }
         }
         else if (key == 13)
              stop = 1;
      }
   }
   tmpstr[length - 1] = '\0';
   retstr = new char[length];
   strcpy(retstr, tmpstr);
   *returnstr = retstr;
   delete retstr;
   return 0;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//                         END CONSTRUCTION                            //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//


//~~~~~~~~~~~~~~~~~~~~~~~~ End of FontObject ~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~ Conversion Routines ~~~~~~~~~~~~~~~~~~~~~

//********************* Converts Longs To Strings *************************

char * LongToString(long longtostrnum)

{
   char * returnstring;
   ltoa(longtostrnum, returnstring, 10);
   return returnstring;
}

//**************** Converts Integers to Strings *****************

char * IntToString(int inttoconvert)

{
   char * strtoreturn;
   itoa(inttoconvert, strtoreturn, 10);
   return strtoreturn;
}