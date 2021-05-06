#include <fstream.h>
#include <stdio.h>

int main(void)
{
	ofstream outf;
   int i;
   char filename[30];
   char gifname[30];

   for (i = 0; i <= 27; i++)
   {
		sprintf(filename, "scrn%02d.htm", i);
      sprintf(gifname, "scrn%02d.gif", i);
      outf.open(filename, ios::out);
      outf << "<HTML>" << endl;
      outf << "<HEAD>" << endl;
      outf << "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=iso-8859-1\">" << endl;
      outf << "<META NAME=\"Author\" CONTENT=\"Po-Shen Loh\">" << endl;
      outf << "<META NAME=\"GENERATOR\" CONTENT=\"Po's HTML generator\">" << endl;
      outf << "<TITLE>Screen Shots</TITLE>" << endl;
      outf << "</HEAD>" << endl;
      outf << "<BODY TEXT=\"#FFFFFF\" BGCOLOR=\"#000000\" LINK=\"#33FF33\" VLINK=\"#009900\" ALINK=\"#FF0000\" BACKGROUND=\"galaxy.JPG\">" << endl;
      outf << "<CENTER>" << endl;
      outf << "<IMG SRC=\"" << gifname << "\" HEIGHT=400 WIDTH=640>" << endl;
      outf << "</CENTER>" << endl;
      outf << "<BR>" << endl;
		outf << "<HR>" << endl;
      outf << endl;
		outf << "<P><A HREF=\"shots.htm\">Back</A> to previous page." << endl;
		outf << "<BR>" << endl;
		outf << endl;
		outf << "<P><I><FONT COLOR=\"#FFCCCC\">Copyright (C) 1998 Po-Shen Loh.  All rights reserved.</FONT></I>" << endl;
		outf << "</BODY>" << endl;
		outf << "</HTML>" << endl;
   	outf.close();
   }

   return 0;
}
