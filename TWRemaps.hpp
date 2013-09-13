#ifndef _TWREMAPS_HPP_
#define _TWREMAPS_HPP_

#if (!defined(__EMX__) && !defined(__IBMCPP__))
   #define _itoa(X,Y,Z) itoa(X,Y,Z)
#endif
#if (defined(__EMX__))  // add your compiler if strrev() has an underscore up front
   #define strrev(X) _strrev(X)
#endif
#if (defined(__DJGPP__) || defined(__EMX__))
   #define strcmpi(X,Y) stricmp(X,Y) // for case-insensitive string comparison
#endif
#if (defined(__DJGPP__))  // add your compiler if no strrev() function
   char* strrev(char*);
#endif

#endif
