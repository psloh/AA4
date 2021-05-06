#include <graphics.h>
#include <conio.h>
#include <iostream.h>

#include "pgphv120.h"
#include "pmsev101.h"
#include "pbutv200.h"

void main(void)
{
   MouseObject M;
   ButtonObject B;
   GraphObject G;

   G.initgraph();                      /*
   G.setcolor(12);
   G.settextjustify(LEFT_TEXT, CENTER_TEXT);
   G.outtextxy(5, 5, "Click the button.");

   M.ColdReset();

   B.AddButton(150, 150, 175, 170, GREEN, "Hi!", "key");
   B.ShowAllButtons();
   cerr << "\n\n" << B.TrapClick(LEFT_BUTTON) << "\n\n";
   M.HideMouse();

   getch();                              */

   G.settextjustify(CENTER_TEXT, CENTER_TEXT);
   G.setcolor(2);
   G.rectangle(170, 10, 310, 190);
   G.setcolor(55);
   G.outtextxy(240, 20, "A L I E N   A T T A C K   I V");
   G.outtextxy(240, 180, "B Y   P O - S H E N   L O H");

   G.setcolor(28);
   G.rectangle(10, 10, 150, 190);

   B.AddButton(200, 35, 280, 35+15, GREEN, "New Game", "New Game");
   B.AddButton(200, 35+23, 280, 35+15+23, GREEN, "Load Game", "Load Game");
   B.AddButton(200, 36+23*2, 280, 35+15+23*2, GREEN, "Instructions", "Instructions");
   B.AddButton(200, 36+23*3, 280, 35+15+23*3, GREEN, "High Scores", "High Scores");
   B.AddButton(200, 36+23*4, 280, 35+15+23*4, GREEN, "Credits", "Credits");
   B.AddButton(225, 36+23*5, 255, 35+15+23*5, GREEN, "Quit", "Quit");

   B.ShowAllButtons();
   char *choice;
	do
   {
   	choice = B.TrapClick_Anim(LEFT_BUTTON);
   } while (choice[0] != 'Q');

   G.closegraph();

   return;
}