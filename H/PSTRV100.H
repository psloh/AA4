#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>

class String
{
 private:

   char length;
   char * _string_private_data;

 public:

   String(void)
   {
      _string_private_data = new char[1];
      length = 0; //Not counting null zero
      _string_private_data[0] = '\0';
   }
   ~String(void)
   {
      delete [] _string_private_data;
   }

   String& operator=(const String &sourceString)  //Assignment
   {
      if (this == &sourceString) { return *this; }
      delete [] _string_private_data;
      _string_private_data = new char [sourceString.length + 1];
      strcpy(_string_private_data, sourceString._string_private_data);
      length = sourceString.length;
      return *this;
   }
   String& operator=(char * sourceString)
   {
      delete [] _string_private_data;
      _string_private_data = new char [strlen(sourceString) + 1];
      strcpy(_string_private_data, sourceString);
      length = strlen(sourceString);
      return *this;
   }

   char& operator[](const unsigned sub);  //Subscripts
   char& operator[](const int sub);
   char& operator[](const char sub);
   char& operator[](const unsigned char sub);

   String operator+(const String &arg);                       //Addition
   friend String operator+(const String S, char * arg);         //of strings
   friend String operator+(char * arg, const String S);
   friend String operator+(const String S, char ch);            //of chars
   friend String operator+(char ch, const String S);

   operator char *()   { return _string_private_data; }

   String& operator--();
   String& operator--(int);

   String& operator++();
   String& operator++(int);

   friend unsigned char operator==(const String &S1, const String &S2);
   friend unsigned char operator!=(const String &S1, const String &S2);
   friend unsigned char operator>=(const String &S1, const String &S2);
   friend unsigned char operator<=(const String &S1, const String &S2);
   friend unsigned char operator<(const String &S1, const String &S2);
   friend unsigned char operator>(const String &S1, const String &S2);

   String& operator+=(const String &arg); //Addition
   String& operator+=(char * arg);     //of strings


   String& operator+=(char ch);       //of chars
};

ostream & operator<<(ostream & out, const String &outString); //  I/O
istream & operator>>(istream & in, String &inString);      //on screen

ofstream & operator<<(ofstream & outf, const String &outString); //of files
ifstream & operator>>(ifstream & inf, String &inString);