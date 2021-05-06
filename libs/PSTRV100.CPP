#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>

#include "pstrv100.h"

//********************* Prefix / Postfix ****************************

String& String::operator--()            //removes first character

{
   if (length == 0)
      { cerr << "\nError: Underflow.\n"; exit(1); }
   char * temp;
   temp = new char[length];
   int i;
   for (i = 1; i <= length; i++)
       temp[i - 1] = _string_private_data[i];
   delete [] _string_private_data;
   _string_private_data = new char [length];
   length--;
   strcpy(_string_private_data, temp);
   delete [] temp;
   return *this;
}

inline String& String::operator--(int)     //removes last char.

{
   if (length == 0)
      { cerr << "\nError: Underflow.\n"; exit(1); }
   char * temp;
   temp = new char[length];
   _string_private_data[length - 1] = '\0';
   strcpy(temp, _string_private_data);
   delete [] _string_private_data;
   _string_private_data = new char [length];
   strcpy(_string_private_data, temp);
   delete [] temp;
   length--;
   return *this;
}

inline String& String::operator++()    //Adds a space to front

{
   String temp;
   temp = ' ' + *this;
   *this = temp;
   return *this;
}

inline String& String::operator++(int) //Adds space to back

{
   String temp;
   temp = *this + ' ';
   *this = temp;
   return *this;
}

//********************** Overloaded subscripts ***************************

char& String::operator[](const unsigned sub)

{
   if (sub == 0) { return length; }
   else if (sub <= length) { return _string_private_data[sub - 1]; }
   else { cerr << "\nError: Subscript out of range.\n"; exit(1); }
   return _string_private_data[0];
}

char& String::operator[](const int sub)

{
   if (sub == 0) { return length; }
   else if (sub <= length) { return _string_private_data[sub - 1]; }
   else { cerr << "\nError: Subscript out of range.\n"; exit(1); }
   return _string_private_data[0];
}

char& String::operator[](const char sub)

{
   if (sub == 0) { return length; }
   else if (sub <= length) { return _string_private_data[sub - 1]; }
   else { cerr << "\nError: Subscript out of range.\n"; exit(1); }
   return _string_private_data[0];
}

char& String::operator[](const unsigned char sub)

{
   if (sub == 0) { return length; }
   else if (sub <= length) { return _string_private_data[sub - 1]; }
   else { cerr << "\nError: Subscript out of range.\n"; exit(1); }
   return _string_private_data[0];
}

//******** Addition of strings, chars, char *'s, etc. ***********

inline String operator+(const String S, char ch)

{
   char * temp;
   temp = new char [S.length + 2];
   strcpy(temp, S._string_private_data);
   temp[S.length] = ch;
   temp[S.length + 1] = '\0';
   String temp2;
   temp2 = temp;
   delete [] temp;
   return temp2;
}

inline String operator+(char ch, const String S)

{
   char * temp;
   temp = new char [S.length + 2];
   temp[0] = ch;
   temp[1] = '\0';
   strcat(temp, S._string_private_data);
   String temp2;
   temp2 = temp;
   delete [] temp;
   return temp2;
}

inline String operator+(const String S, char * arg)

{
   char * temp;
   temp = new char [S.length + strlen(arg) + 1];
   strcpy(temp, S._string_private_data);
   strcat(temp, arg);
   String temp2;
   temp2 = temp;
   delete [] temp;
   return temp2;
}

inline String operator+(char * arg, const String S)

{
   char * temp;
   temp = new char [S.length + strlen(arg) + 1];
   strcpy(temp, arg);
   strcat(temp, S._string_private_data);
   String temp2;
   temp2 = temp;
   delete [] temp;
   return temp2;
}

inline String String::operator+(const String &arg)

{
   char * temp;
   temp = new char [length + 1];
   strcpy(temp, _string_private_data);
   strcat(temp, arg._string_private_data);
   String temp2;
   temp2 = temp;
   delete [] temp;
   return temp2;
}

//*************  Relational operators *****************

unsigned char operator==(const String &S1, const String &S2)

{
   char i;

   if (S1.length != S2.length) { return 0; }
   for (i = 0; i < S1.length; i++)
       if (S1._string_private_data[i] != S2._string_private_data[i])
          { return 0; }
   return 1;
}

inline unsigned char operator!=(const String &S1, const String &S2)

{
   return !(S1 == S2);
}

inline unsigned char operator>=(const String &S1, const String &S2)

{
   return ((S1 == S2) || (S1 > S2));
}

inline unsigned char operator<=(const String &S1, const String &S2)

{
   return ((S1 == S2) || (S1 < S2));
}

unsigned char operator<(const String &S1, const String &S2)

{
   char i;
   char m = S1.length > S2.length ? S2.length : S1.length;

   for (i = 0; i < m; i++)
       if (S1._string_private_data[i] >= S2._string_private_data[i])
          { return 0; }
   if (S1.length < S2.length)
      return 1;
   else
       return 0;
}

unsigned char operator>(const String &S1, const String &S2)

{
   char i;
   char m = S1.length > S2.length ? S2.length : S1.length;

   for (i = 0; i < m; i++)
       if (S1._string_private_data[i] <= S2._string_private_data[i])
          { return 0; }
   if (S1.length > S2.length)
      return 1;
   else
       return 0;
}

//****************** += Stuff *****************************

inline String& String::operator+=(const String &arg) //Addition

{
   return *this + arg;
}

inline String& String::operator+=(char * arg)     //of strings

{
   return *this + arg;
}

inline String& String::operator+=(char ch)       //of chars

{
   return *this + ch;
}