#include "Globals.hpp"
#include "TWSector.hpp"

#if (defined(__OS2__) || defined(__IBMCPP__) || defined(__EMX__))
ULONG Globals::pclass=2;
LONG Globals::pdelta=0;
#endif
USHORT Globals::totalSectors=1000;
USHORT Globals::minCapacity=0;
TWSectorArray Globals::sectors(1);
Bool Globals::noCalc=false;
Bool Globals::BBS_SSB=false;
Bool Globals::BxS_SxB=false;
Bool Globals::xBS_xSB=false;
Bool Globals::BSx_SBx=false;
Bool Globals::xxB_xxB=false;
Bool Globals::xxS_xxS=false;
Bool Globals::BBS_BSB=false;
UCHAR Globals::minorVersion=6;
UCHAR Globals::majorVersion=0;
#ifdef DEBUG_ENABLED
ofstream Globals::debug("debug.log", ios::app);
#endif
