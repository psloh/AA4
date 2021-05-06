//vga256 graphics stuff

const unsigned VGAaddr = 0xA000;

class GraphObject
{
	private:
   	char _p_graphobject_drawcolor;
      char _p_graphobject_fillcolor;
      char _p_graphobject_backcolor;
      char _p_graphobject_gmodeon;
   public:
   	GraphObject(void) { _p_graphobject_gmodeon = 0; }
   	void initgraph(void);        //basic screen
      void closegraph(void);
      void clearviewport(void);
      void waitretrace(void);
      void putpixel(int x, int y, char color);
      char getpixel(int x, int y);
      void getpixel(int x, int y, char *color);
      void setrgbpalette(char color, unsigned r, unsigned g, unsigned b);
      void getrgbpalette(char color, unsigned *r, unsigned *g, unsigned *b);
      void swap(unsigned dest, unsigned source);
      void putimage(int x, int y, void *image, char mode);
      unsigned imagesize(int xs, int ys, int xe, int ye);
      void getimage(int xs, int ys, int xe, int ye, void *image);
      void setcolor(char color);
      void getcolor(char *color);
      char getcolor(void);
      void setbkcolor(char color);
      void getbkcolor(char *color);
      char getbkcolor(void);
      void setfillcolor(char color);
      void getfillcolor(char *color);
      char getfillcolor(void);
      void hline(int xs, int ys, int xe, int ye);
      void vline(int xs, int ys, int xe, int ye);
      void line(int xs, int ys, int xe, int ye);
      void rectangle(int xs, int ys, int xe, int ye);
      void bar(int xs, int ys, int xe, int ye);

      void makevirtual(void **VScr);               //Virtual screen
      void clearvirtual(void **Vscr);
      void clearviewport(void *VScr);
      void putpixel(int x, int y, char color, void *VScr);
      char getpixel(int x, int y, void *VScr);
      void getpixel(int x, int y, char *color, void *VScr);
      void putimage(int x, int y, void *image, char mode, void *VScr);
      void getimage(int xs, int ys, int xe, int ye, void *image, void *VScr);
      void hline(int xs, int ys, int xe, int ye, void *VScr);
      void vline(int xs, int ys, int xe, int ye, void *VScr);
      void line(int xs, int ys, int xe, int ye, void *VScr);
      void rectangle(int xs, int ys, int xe, int ye, void *VScr);
      void bar(int xs, int ys, int xe, int ye, void *VScr);
};
