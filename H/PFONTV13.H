/*******************************************************************
NAME: Po-Shen Loh.
TITLE: Po-Shen's font unit #1 (version 1.3).
PURPOSE: Draws characters, strings, and inputs data all in Po-Shen's
         Font #1.
CONTENTS: FontObject (font #1 procedures):
             Draw*(x coordinate, y coordinate, width in pixels, color(0..15).
             psClS(color(0..15).
             InitGraphLetts(ratio of height in pixels to the width in pixels,
                tab length, alarm bell frequency, path of graphics drivers,
                alarm bell time.
             psscanf(scan type, var: str, real, int, etc.). Returns a 1 if
                there is an error, and a 0 if there isn't.
             psprintf(x coordinate, y coordinate, width in pixels,
                color(0..15), font(in this ver. pass the value 1, char. str.).
             MoveTextCursor(x destination, y destination).
             GetTextCursorXY(vars: x coordinate, y coordinate).
             SetCurrColor(color (0..15)).
             GetCurrColor(var: color (0..15)).
             SetCurrSize(width in pixels).
             GetCurrSize(var: width in pixels).
             Destruct.

             FontObject is an object type that must be declared before it
                        may be used.

          Conversions:
             LongToString(long integer). Returns a string.
             IntToString(integer number). Returns a string.

TABLES:
       Color Table: 0 = black             8 = dark gray
                    1 = blue              9 = light blue
                    2 = green             10 = light green
                    3 = cyan              11 = light cyan
                    4 = red               12 = light red
                    5 = magenta           13 = light magenta
                    6 = brown             14 = yellow
                    7 = light gray        15 = white
       Scan Types:  %d = integer          %s = string
                    %l = long integer     %r = real
       Abbrev.:     SPC = space           EXC = !
                    NUM = #               QUES = ?
                    DLR = $               PRC = %
                    BSH = \               ODBQT = ``
                    CDBQT = ''            OSIQT = `
                    CSIQT = '             TIMES = x
                    AST = *               CPR = )
                    OPR = (               PLS = +
                    LES = <               GRE = >
                    LESOREQ = <=          GREOREQ = >=
                    APPROX = .            SIM = ~
                             =                   ~
                    EQU = =               OBR = [
                    CBR = ]               OBRACE = {
                    CBRACE = }            CAR = ^
                    UND = _               TIL = ~
                    CMA = ,               CLN = :
                    MNS = -               PRD = .
                    DIV = +               SLSH = /
                    SMICLN = ;

************************************************************/



//======================= Interface ========================

typedef char **paratype;
typedef unsigned char byte;

class FontObject

{
 private:

   void Draw1(int x, int y, unsigned size, unsigned color);
   void Draw2(int x, int y, unsigned size, unsigned color);
   void Draw3(int x, int y, unsigned size, unsigned color);
   void Draw4(int x, int y, unsigned size, unsigned color);
   void Draw5(int x, int y, unsigned size, unsigned color);
   void Draw6(int x, int y, unsigned size, unsigned color);
   void Draw7(int x, int y, unsigned size, unsigned color);
   void Draw8(int x, int y, unsigned size, unsigned color);
   void Draw9(int x, int y, unsigned size, unsigned color);
   void Draw0(int x, int y, unsigned size, unsigned color);
   void DrawQ(int x, int y, unsigned size, unsigned color);
   void DrawA(int x, int y, unsigned size, unsigned color);
   void DrawZ(int x, int y, unsigned size, unsigned color);
   void DrawX(int x, int y, unsigned size, unsigned color);
   void DrawS(int x, int y, unsigned size, unsigned color);
   void DrawW(int x, int y, unsigned size, unsigned color);
   void DrawE(int x, int y, unsigned size, unsigned color);
   void DrawD(int x, int y, unsigned size, unsigned color);
   void DrawC(int x, int y, unsigned size, unsigned color);
   void DrawV(int x, int y, unsigned size, unsigned color);
   void DrawF(int x, int y, unsigned size, unsigned color);
   void DrawR(int x, int y, unsigned size, unsigned color);
   void DrawB(int x, int y, unsigned size, unsigned color);
   void DrawG(int x, int y, unsigned size, unsigned color);
   void DrawT(int x, int y, unsigned size, unsigned color);
   void DrawY(int x, int y, unsigned size, unsigned color);
   void DrawH(int x, int y, unsigned size, unsigned color);
   void DrawN(int x, int y, unsigned size, unsigned color);
   void DrawM(int x, int y, unsigned size, unsigned color);
   void DrawJ(int x, int y, unsigned size, unsigned color);
   void DrawU(int x, int y, unsigned size, unsigned color);
   void DrawI(int x, int y, unsigned size, unsigned color);
   void DrawK(int x, int y, unsigned size, unsigned color);
   void DrawL(int x, int y, unsigned size, unsigned color);
   void DrawO(int x, int y, unsigned size, unsigned color);
   void DrawP(int x, int y, unsigned size, unsigned color);
   void DrawSPC(int x, int y, unsigned size);
   void DrawEXC(int x, int y, unsigned size, unsigned color);
   void DrawNUM(int x, int y, unsigned size, unsigned color);
   void DrawQUES(int x, int y, unsigned size, unsigned color);
   void DrawDLR(int x, int y, unsigned size, unsigned color);
   void DrawPRC(int x, int y, unsigned size, unsigned color);
   void DrawBSH(int x, int y, unsigned size, unsigned color);
   void DrawODBQT(int x, int y, unsigned size, unsigned color);
   void DrawCDBQT(int x, int y, unsigned size, unsigned color);
   void DrawOSIQT(int x, int y, unsigned size, unsigned color);
   void DrawCSIQT(int x, int y, unsigned size, unsigned color);
   void DrawTIMES(int x, int y, unsigned size, unsigned color);
   void DrawAST(int x, int y, unsigned size, unsigned color);
   void DrawCPR(int x, int y, unsigned size, unsigned color);
   void DrawOPR(int x, int y, unsigned size, unsigned color);
   void DrawPLS(int x, int y, unsigned size, unsigned color);
   void DrawLES(int x, int y, unsigned size, unsigned color);
   void DrawGRE(int x, int y, unsigned size, unsigned color);
   void DrawLESOREQ(int x, int y, unsigned size, unsigned color);
   void DrawGREOREQ(int x, int y, unsigned size, unsigned color);
   void DrawEQU(int x, int y, unsigned size, unsigned color);
   void DrawSIM(int x, int y, unsigned size, unsigned color);
   void DrawAPPROX(int x, int y, unsigned size, unsigned color);
   void DrawOBR(int x, int y, unsigned size, unsigned color);
   void DrawCBR(int x, int y, unsigned size, unsigned color);
   void DrawCAR(int x, int y, unsigned size, unsigned color);
   void DrawUND(int x, int y, unsigned size, unsigned color);
   void DrawOBRACE(int x, int y, unsigned size, unsigned color);
   void DrawCBRACE(int x, int y, unsigned size, unsigned color);
   void DrawTIL(int x, int y, unsigned size, unsigned color);
   void DrawCMA(int x, int y, unsigned size, unsigned color);
   void DrawCLN(int x, int y, unsigned size, unsigned color);
   void DrawMNS(int x, int y, unsigned size, unsigned color);
   void DrawPRD(int x, int y, unsigned size, unsigned color);
   void DrawDIV(int x, int y, unsigned size, unsigned color);
   void DrawSLSH(int x, int y, unsigned size, unsigned color);
   void DrawSMICLN(int x, int y, unsigned size, unsigned color);

 public:

   FontObject(float YDX = 1.4, int tabl = 5, int abf = 1000, int abt = 100);

   void write(int x, int y, int siz, int col, char *chrstr);

   void write_ch(int x, int y, int size, int color, char ch);

   void p_write(int x, int y, unsigned siz, unsigned col, int xin, int yin,
                byte rows, byte cols, int delaytime, int tail_length,
                paratype para);

   int read(int x, int y, unsigned siz, unsigned col, unsigned timelapse, char *returnstr[]);

   void ClrScr(unsigned color);

   void movetextcursor(int x, int y);

   void gettextcursorXY(int *x, int *y);

   void setcurrcolor(unsigned color);

   void getcurrcolor(unsigned *color);

   void setcurrsize(unsigned size);

   void getcurrsize(unsigned *size);

   ~FontObject(void);

 private:

   int textcursorx, textcursory;
   unsigned currentcolor, currentsize;
   float YDIVX;
   int tablength, alarmbellfrequency, alarmbelltime;
};

char * IntToString(int inttoconvert);  //converts integers to strings
char * LongToString(long longtostrnum); //converts longints to strings