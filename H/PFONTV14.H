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

   void Draw1(int x, int y, unsigned size);
   void Draw2(int x, int y, unsigned size);
   void Draw3(int x, int y, unsigned size);
   void Draw4(int x, int y, unsigned size);
   void Draw5(int x, int y, unsigned size);
   void Draw6(int x, int y, unsigned size);
   void Draw7(int x, int y, unsigned size);
   void Draw8(int x, int y, unsigned size);
   void Draw9(int x, int y, unsigned size);
   void Draw0(int x, int y, unsigned size);
   void DrawQ(int x, int y, unsigned size);
   void DrawA(int x, int y, unsigned size);
   void DrawZ(int x, int y, unsigned size);
   void DrawX(int x, int y, unsigned size);
   void DrawS(int x, int y, unsigned size);
   void DrawW(int x, int y, unsigned size);
   void DrawE(int x, int y, unsigned size);
   void DrawD(int x, int y, unsigned size);
   void DrawC(int x, int y, unsigned size);
   void DrawV(int x, int y, unsigned size);
   void DrawF(int x, int y, unsigned size);
   void DrawR(int x, int y, unsigned size);
   void DrawB(int x, int y, unsigned size);
   void DrawG(int x, int y, unsigned size);
   void DrawT(int x, int y, unsigned size);
   void DrawY(int x, int y, unsigned size);
   void DrawH(int x, int y, unsigned size);
   void DrawN(int x, int y, unsigned size);
   void DrawM(int x, int y, unsigned size);
   void DrawJ(int x, int y, unsigned size);
   void DrawU(int x, int y, unsigned size);
   void DrawI(int x, int y, unsigned size);
   void DrawK(int x, int y, unsigned size);
   void DrawL(int x, int y, unsigned size);
   void DrawO(int x, int y, unsigned size);
   void DrawP(int x, int y, unsigned size);
   void DrawSPC(int x, int y, unsigned size);
   void DrawEXC(int x, int y, unsigned size);
   void DrawNUM(int x, int y, unsigned size);
   void DrawQUES(int x, int y, unsigned size);
   void DrawDLR(int x, int y, unsigned size);
   void DrawPRC(int x, int y, unsigned size);
   void DrawBSH(int x, int y, unsigned size);
   void DrawODBQT(int x, int y, unsigned size);
   void DrawCDBQT(int x, int y, unsigned size);
   void DrawOSIQT(int x, int y, unsigned size);
   void DrawCSIQT(int x, int y, unsigned size);
   void DrawTIMES(int x, int y, unsigned size);
   void DrawAST(int x, int y, unsigned size);
   void DrawCPR(int x, int y, unsigned size);
   void DrawOPR(int x, int y, unsigned size);
   void DrawPLS(int x, int y, unsigned size);
   void DrawLES(int x, int y, unsigned size);
   void DrawGRE(int x, int y, unsigned size);
   void DrawLESOREQ(int x, int y, unsigned size);
   void DrawGREOREQ(int x, int y, unsigned size);
   void DrawEQU(int x, int y, unsigned size);
   void DrawSIM(int x, int y, unsigned size);
   void DrawAPPROX(int x, int y, unsigned size);
   void DrawOBR(int x, int y, unsigned size);
   void DrawCBR(int x, int y, unsigned size);
   void DrawCAR(int x, int y, unsigned size);
   void DrawUND(int x, int y, unsigned size);
   void DrawOBRACE(int x, int y, unsigned size);
   void DrawCBRACE(int x, int y, unsigned size);
   void DrawTIL(int x, int y, unsigned size);
   void DrawCMA(int x, int y, unsigned size);
   void DrawCLN(int x, int y, unsigned size);
   void DrawMNS(int x, int y, unsigned size);
   void DrawPRD(int x, int y, unsigned size);
   void DrawDIV(int x, int y, unsigned size);
   void DrawSLSH(int x, int y, unsigned size);
   void DrawSMICLN(int x, int y, unsigned size);

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
