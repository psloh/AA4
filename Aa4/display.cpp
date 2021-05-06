#include <fstream.h>
#include <math.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <string.h>

#include "PGphV120.H"

GraphObject G;

void y_plot(float a, float b, float c,
            float start, float end,
            float x_off, float y_off,
            VirtualScreen &Vscr)
{
	int i;

   for (i = start; i <= end-1; i++)
   	G.line(x_off+i, y_off + a*i*i + b*i + c, x_off+i+1, y_off + a*(i+1)*(i+1) + b*(i+1) + c, Vscr);
}

void x_plot(float a, float b, float c,
            float start, float end,
            float x_off, float y_off,
            VirtualScreen &Vscr)
{
	int i;

   for (i = start; i <= end-1; i++)
   	G.line(x_off + a*i*i + b*i + c, y_off+i, x_off + a*(i+1)*(i+1) + b*(i+1) + c, y_off+i+1, Vscr);
}

void Interpret(char *filename)
{
	ifstream inf(filename);
   VirtualScreen Vscr = MK_FP(VGAaddr, 0);
   char opcode[30];
   float x1, y1, x2, y2;
   float a, b, c;
   float start, end;
   float x_off, y_off;
   float x, y, x_rad, y_rad;
   char string[501];

//   G.makevirtual(&Vscr);
	G.clearviewport(Vscr);

   while (filename)
   {
   	inf >> opcode;
      if (!strcmpi(opcode, "HLINE"))
      {
      	inf >> x1 >> y1 >> x2 >> y2;
         G.hline(x1, y1, x2, y2, Vscr);
      }
      else if (!strcmpi(opcode, "VLINE"))
      {
      	inf >> x1 >> y1 >> x2 >> y2;
         G.vline(x1, y1, x2, y2, Vscr);
      }
      else if (!strcmpi(opcode, "LINE"))
      {
      	inf >> x1 >> y1 >> x2 >> y2;
         G.line(x1, y1, x2, y2, Vscr);
      }
      else if (!strcmpi(opcode, "QUAD_Y="))   // y=
      {
      	inf >> a >> b >> c >> start >> end >> x_off >> y_off;
      	y_plot(a, b, c, start, end, x_off, y_off, Vscr);          // y = ax*x + bx + c
      }
      else if (!strcmpi(opcode, "QUAD_X="))   // x=
      {
      	inf >> a >> b >> c >> start >> end >> x_off >> y_off;
         x_plot(a, b, c, start, end, x_off, y_off, Vscr);          // x = a*y*y + by + c
      }
      else if (!strcmpi(opcode, "ELLIPSE"))
      {
      	inf >> x >> y >> start >> end >> x_rad >> y_rad;
			G.ellipse(x, y, start, end, x_rad, y_rad, Vscr);
      }
      else if (!strcmpi(opcode, "PUTPIXEL"))
      {
      	inf >> a >> b >> c;
         G.putpixel(a, b, c, Vscr);
      }
      else if (!strcmpi(opcode, "SETCOLOR"))
      {
      	inf >> a;
         G.setcolor(a);
		}
		else if (!strcmpi(opcode, "SETFILLCOLOR"))
		{
			inf >> a;
			G.setfillcolor(a);
		}
      else if (!strcmpi(opcode, "SETBACK"))
      {
      	inf >> a;
         G.setbkcolor(a);
         G.clearviewport(Vscr);
      }
      else if (!strcmpi(opcode, "OUTTEXTXY"))
      {
      	inf >> x >> y >> string;
         G.outtextxy(x, y, string, Vscr);
      }
      else if (!strcmpi(opcode, "SETTEXTJUSTIFY"))
      {
      	inf >> a >> b;
         G.settextjustify(a, b);
      }
      else if (!strcmpi(opcode, "FLOODFILL"))
      {
      	inf >> a >> b >> c;
         G.h_floodfill(a, b, c, Vscr);
      }
      else if (!strcmpi(opcode, "SETFILLCOLOR"))
      {
      	inf >> a;
         G.setfillcolor(a);
      }
      else if (!strcmpi(opcode, "REM"))
      	inf.getline(string, 500);
      else if (!strcmpi(opcode, "RECTANGLE"))
      {
      	inf >> x1 >> y1 >> x2 >> y2;
         G.rectangle(x1, y1, x2, y2, Vscr);
		}
		else if (!strcmpi(opcode, "BAR"))
		{
			inf >> x1 >> y1 >> x2 >> y2;
			G.bar(x1, y1, x2, y2, Vscr);
		}
      else
      	break;
   }

   G.setbkcolor(BLACK);

//   G.clearvirtual(&Vscr);
}




int main(void)
{
	G.initgraph();
   Interpret("store.po");
   getch();
   G.closegraph();
   return 0;
}