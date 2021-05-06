//vga256 graphics stuff

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
   public:
   	GraphObject(void) { _p_graphobject_gmodeon = 0; }
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

      void makevirtual(void far **VScr);               //Virtual screen
      void clearvirtual(void far **VScr);
      void clearviewport(void far *VScr);
      void putpixel(int x, int y, char color, void far *VScr);
      char getpixel(int x, int y, void far *VScr);
      void getpixel(int x, int y, char *color, void far *VScr);
      void putimage(int x, int y, void *image, char mode, void far *VScr);
      void getimage(int xs, int ys, int xe, int ye, void *image, void far *VScr);
      void hline(int xs, int ys, int xe, int ye, void far *VScr);
      void vline(int xs, int ys, int xe, int ye, void far *VScr);
      void line(int xs, int ys, int xe, int ye, void far *VScr);
      void rectangle(int xs, int ys, int xe, int ye, void far *VScr);
      void bar(int xs, int ys, int xe, int ye, void far *VScr);
      void outtextxy(int x, int y, char *string, void far *VScr);
      void outtext(char *string, void far *VScr);
};
