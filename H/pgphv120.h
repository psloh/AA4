//vga256 graphics stuff
#ifndef __PGPHV120_H
#define __PGPHV120_H

#include <math.h>

typedef void far *VirtualScreen;

const unsigned VGAaddr = 0xA000;

class GraphObject
{
	private:
   	char _p_graphobject_drawcolor;
      char _p_graphobject_fillcolor;
      char _p_graphobject_backcolor;
      char _p_graphobject_gmodeon;
      int _p_graphobject_xpos;
      int _p_graphobject_ypos;
      int _p_graphobject_hjust;
		int _p_graphobject_vjust;
		float _p_graphobject_bright;
		char _p_graphobject_palette[256][3];  // color palette

		void _p_graphobject_DrawA(int x, int y, void far *Screen);
		void _p_graphobject_DrawB(int x, int y, void far *Screen);
		void _p_graphobject_DrawC(int x, int y, void far *Screen);
      void _p_graphobject_DrawD(int x, int y, void far *Screen);
      void _p_graphobject_DrawE(int x, int y, void far *Screen);
      void _p_graphobject_DrawF(int x, int y, void far *Screen);
		void _p_graphobject_DrawG(int x, int y, void far *Screen);
      void _p_graphobject_DrawH(int x, int y, void far *Screen);
      void _p_graphobject_DrawI(int x, int y, void far *Screen);
      void _p_graphobject_DrawJ(int x, int y, void far *Screen);
      void _p_graphobject_DrawK(int x, int y, void far *Screen);
      void _p_graphobject_DrawL(int x, int y, void far *Screen);
      void _p_graphobject_DrawM(int x, int y, void far *Screen);
      void _p_graphobject_DrawN(int x, int y, void far *Screen);
		void _p_graphobject_DrawO(int x, int y, void far *Screen);
      void _p_graphobject_DrawP(int x, int y, void far *Screen);
      void _p_graphobject_DrawQ(int x, int y, void far *Screen);
      void _p_graphobject_DrawR(int x, int y, void far *Screen);
		void _p_graphobject_DrawS(int x, int y, void far *Screen);
      void _p_graphobject_DrawT(int x, int y, void far *Screen);
      void _p_graphobject_DrawU(int x, int y, void far *Screen);
      void _p_graphobject_DrawV(int x, int y, void far *Screen);
      void _p_graphobject_DrawW(int x, int y, void far *Screen);
      void _p_graphobject_DrawX(int x, int y, void far *Screen);
      void _p_graphobject_DrawY(int x, int y, void far *Screen);
      void _p_graphobject_DrawZ(int x, int y, void far *Screen);
		void _p_graphobject_DrawPRD(int x, int y, void far *Screen);
      void _p_graphobject_DrawQUES(int x, int y, void far *Screen);
      void _p_graphobject_DrawEXC(int x, int y, void far *Screen);
      void _p_graphobject_DrawCMA(int x, int y, void far *Screen);
		void _p_graphobject_DrawSPC(int x, int y, void far *Screen);
      void _p_graphobject_Draw1(int x, int y, void far *Screen);
      void _p_graphobject_Draw2(int x, int y, void far *Screen);
      void _p_graphobject_Draw3(int x, int y, void far *Screen);
      void _p_graphobject_Draw4(int x, int y, void far *Screen);
      void _p_graphobject_Draw5(int x, int y, void far *Screen);
      void _p_graphobject_Draw6(int x, int y, void far *Screen);
      void _p_graphobject_Draw7(int x, int y, void far *Screen);
		void _p_graphobject_Draw8(int x, int y, void far *Screen);
      void _p_graphobject_Draw9(int x, int y, void far *Screen);
		void _p_graphobject_Draw0(int x, int y, void far *Screen);
      void _p_graphobject_DrawMNS(int x, int y, void far *Screen);
		void _p_graphobject_DrawGRE(int x, int y, void far *Screen);
      void _p_graphobject_DrawLES(int x, int y, void far *Screen);
      void _p_graphobject_DrawQTE(int x, int y, void far *Screen);
      void _p_graphobject_DrawPRC(int x, int y, void far *Screen);
      void _p_graphobject_DrawCAR(int x, int y, void far *Screen);
      void _p_graphobject_DrawLpar(int x, int y, void far *Screen);
      void _p_graphobject_DrawRpar(int x, int y, void far *Screen);
      void _p_graphobject_DrawUND(int x, int y, void far *Screen);
		void _p_graphobject_DrawEQL(int x, int y, void far *Screen);
      void _p_graphobject_DrawPLUS(int x, int y, void far *Screen);
		void _p_graphobject_DrawLbrk(int x, int y, void far *Screen);
      void _p_graphobject_DrawRbrk(int x, int y, void far *Screen);
		void _p_graphobject_DrawLbrc(int x, int y, void far *Screen);
      void _p_graphobject_DrawRbrc(int x, int y, void far *Screen);
      void _p_graphobject_DrawPIPE(int x, int y, void far *Screen);
      void _p_graphobject_DrawBSLH(int x, int y, void far *Screen);
      void _p_graphobject_DrawFSLH(int x, int y, void far *Screen);
      void _p_graphobject_DrawCLN(int x, int y, void far *Screen);
		void _p_graphobject_DrawSCLN(int x, int y, void far *Screen);
		void _p_graphobject_DrawDQTE(int x, int y, void far *Screen);

		void DrawCrsr(int x_cur, int y_cur);
	public:
		GraphObject(void);

		~GraphObject(void)  { }
		void initgraph(void);                      //universal functions
		void closegraph(void);
		void clearviewport(void);
		void waitretrace(void);
		void moveto(int x, int y);
		int getx(void);
		int gety(void);
      void setrgbpalette(char color, char red, char green, char blue);
      void getrgbpalette(char color, char *red, char *green, char *blue);
      void setcolor(char color);
      void getcolor(char *color);
      char getcolor(void);
      void setbkcolor(char color);
      void getbkcolor(char *color);
      char getbkcolor(void);
      void setfillcolor(char color);
      void getfillcolor(char *color);
      char getfillcolor(void);
      void settextjustify(int h, int v);
      void gettextjustify(int *h, int *v);
      unsigned imagesize(int xs, int ys, int xe, int ye);
      void swap(unsigned dest, unsigned source);

      void putpixel(int x, int y, char color);   //basic screen
      char getpixel(int x, int y);
      void getpixel(int x, int y, char *color);
      void putimage(int x, int y, void *image, char mode);
      void getimage(int xs, int ys, int xe, int ye, void *image);
      void hline(int xs, int ys, int xe, int ye);
      void vline(int xs, int ys, int xe, int ye);
      void line(int xs, int ys, int xe, int ye);
      void rectangle(int xs, int ys, int xe, int ye);
      void bar(int xs, int ys, int xe, int ye);
      void outtextxy(int x, int y, char *string);
		void outtext(char *string);
		void ellipse(float x, float y, float start, float end, float x_rad, float y_rad);
      void h_floodfill(int x, int y, char border);

      void makevirtual(VirtualScreen *VScr);               //Virtual screen
      void clearvirtual(VirtualScreen *VScr);
      void clearviewport(VirtualScreen VScr);
      void putpixel(int x, int y, char color, VirtualScreen VScr);
      char getpixel(int x, int y, VirtualScreen VScr);
      void getpixel(int x, int y, char *color, VirtualScreen VScr);
		void putimage(int x, int y, void *image, char mode, VirtualScreen VScr);
      void putimage_fast(int x, int y, void *image, VirtualScreen VScr);
      void getimage(int xs, int ys, int xe, int ye, void far *image, VirtualScreen VScr);
      void hline(int xs, int ys, int xe, int ye, VirtualScreen VScr);
      void vline(int xs, int ys, int xe, int ye, VirtualScreen VScr);
      void line(int xs, int ys, int xe, int ye, VirtualScreen VScr);
      void rectangle(int xs, int ys, int xe, int ye, VirtualScreen VScr);
      void bar(int xs, int ys, int xe, int ye, VirtualScreen VScr);
		void outtextxy(int x, int y, char *string, VirtualScreen VScr);
		void outtext(char *string, VirtualScreen VScr);
      void ellipse(float x, float y, float start, float end, float x_rad, float y_rad, VirtualScreen VScr);
      void h_floodfill(int x, int y, char border, VirtualScreen VScr);

		void write(int x, int y, int col, int tail_col, char *string, int delay_time);
		void write(int x, int y, int col, int tail_col, char *string, int delay_time, char &stop_flag);
      void intext(int x_s, int y_s, int x_e, int y_e, char color, char *string);

      void fade_out(void);                   // Screen effects
      void fade_to(float percent);  // all colors go to <percent>% intensity
      void fade_in(void);
      void scroll_up(int rows);   // scrolls up <rows> rows.
      void scroll_up(int rows, char &stop_flag);   // scrolls up <rows> rows.
      void fade_into(VirtualScreen VScr);  // fades into virtual screen
      void dissolve(VirtualScreen VScr);   // dissolves into virtual screen
      void bars_wipe(VirtualScreen VScr);  // transition with bars
      void shred_clear(void);          // screen is shredded horiz & vert and returns to background
};


#ifndef __GRAPHICS_H

enum putimage_ops {     /* BitBlt operators for putimage */
    COPY_PUT,       /* MOV */
    XOR_PUT,        /* XOR */
    OR_PUT,         /* OR  */
    AND_PUT,        /* AND */
    NOT_PUT         /* NOT */
};

enum text_just {        /* Horizontal and vertical justification
                   for settextjustify */
    LEFT_TEXT   = 0,
    CENTER_TEXT = 1,
    RIGHT_TEXT  = 2,

    BOTTOM_TEXT = 0,
     /* CENTER_TEXT = 1,  already defined above */
    TOP_TEXT    = 2
};
#endif

#endif

#ifndef __COLORS
#define __COLORS

enum COLORS {
    BLACK,          /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,           /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};
#endif
