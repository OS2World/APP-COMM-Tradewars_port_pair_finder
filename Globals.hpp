#include "TWTypes.hpp"
#include <fstream.h>

#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

class TWSectorArray;

struct Globals
{
   #if (defined(__OS2__) || defined(__IBMCPP__) || defined(__EMX__))
   static ULONG pclass;
   static LONG pdelta;
   #endif
   static USHORT totalSectors;
   static USHORT minCapacity;
   static TWSectorArray sectors;
   static Bool noCalc;
   static Bool BBS_SSB;
   static Bool BxS_SxB;
   static Bool xBS_xSB;
   static Bool BSx_SBx;
   static Bool xxB_xxB;
   static Bool xxS_xxS;
   static Bool BBS_BSB;
   static UCHAR minorVersion;
   static UCHAR majorVersion;
   #ifdef DEBUG_ENABLED
   static ofstream debug;
   #endif
};

#endif
