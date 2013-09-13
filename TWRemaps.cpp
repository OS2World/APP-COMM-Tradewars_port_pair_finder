#include "TWRemaps.hpp"

#if (defined(__DJGPP__))
   char* strrev(char* string)
   {
      char buffer[256];
      int i, dest, len;

      strcpy(buffer, string);
      len=strlen(string);
      for (i=0; i<len; i++)
         {
         dest=(len-1)-i;
         string[dest]=buffer[i];
         }
      return string;
   }
#endif
