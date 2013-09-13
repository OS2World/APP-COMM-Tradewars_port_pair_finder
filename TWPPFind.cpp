#if (defined(__OS2__) || defined(__EMX__) || defined(__OS2__))
   #define INCL_DOSPROCESS
   #define INCL_BASE
   #include <os2.h>
#endif
#if (defined(__TCPLUSPLUS__) || defined(__DJGPP__))
   #include <conio.h>
#endif
#include <signal.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>

#include "TWRemaps.hpp"
#include "TWTypes.hpp"
#include "Globals.hpp"
#include "TWSector.hpp"
#include "TWWarp.hpp"
#include "AbEnds.hpp"
#include "TWPPFind.h"

extern "C" void AbortCatcher(int sigstuff);

int main(int argc, char* argv[], char* envp[])
{
   char sectFileName[256], portFileName[256], outFileName[256],
        binPortName[256], argtype[3], argdata[256], argtemp[256];
   USHORT numPairs, i, zTemp;
   Bool readBinPorts=false, writeBinPorts=false;
   ifstream sectFile;
   ifstream portFile;
   ofstream outFile;
   fstream binPortFile;
   TWPairSet pairSet;
   TWZappedPorts zPorts;
   #if (defined(__IBMCPP__) || defined(__EMX__) || defined(__OS2__))
   VIOCURSORINFO cursorInfo;
   #endif

   signal(SIGINT, AbortCatcher);

   strcpy(sectFileName, "");
   strcpy(portFileName, "");
   strcpy(binPortName, "");
   strcpy(outFileName, "pairs.lst");

   for (i=1; i<argc; i++)
      {
      memset(argtype, 0, sizeof(argtype));
      memset(argdata, 0, sizeof(argdata));
      memset(argtemp, 0, sizeof(argtemp));
      strncpy(argtype, argv[i], 2);
      strncpy(argtemp, strrev(argv[i]), (strlen(argv[i])-2));
      strcpy(argdata, strrev(argtemp));
      if (!strcmpi(argtype, "/t") || !strcmpi(argtype, "-t"))
         {
         if (!strcmpi(argdata, "bbs_ssb"))
            Globals::BBS_SSB=true;
         else if (!strcmpi(argdata, "xbs_xsb"))
            Globals::xBS_xSB=true;
         else if (!strcmpi(argdata, "bsx_sbx"))
            Globals::BSx_SBx=true;
         else if (!strcmpi(argdata, "xbs_xsb"))
            Globals::xBS_xSB=true;
         else if (!strcmpi(argdata, "xxb_xxb"))
            Globals::xxB_xxB=true;
         else if (!strcmpi(argdata, "xxs_xxs"))
            Globals::xxS_xxS=true;
         else if (!strcmpi(argdata, "bbs_bsb"))
            Globals::BBS_BSB=true;
         else
            BadUsage(argdata, BAD_PAIR_TYPE);
         }
      else if (!strcmpi(argtype, "/b") || !strcmpi(argtype, "-b"))
         {
         memset(binPortName, 0, sizeof(binPortName));
         strcpy(binPortName, argdata);
         }
      else if (!strcmpi(argtype, "/s") || !strcmpi(argtype, "-s"))
         {
         memset(sectFileName, 0, sizeof(sectFileName));
         strcpy(sectFileName, argdata);
         }
      else if (!strcmpi(argtype, "/o") || !strcmpi(argtype, "-o"))
         {
         memset(outFileName, 0, sizeof(outFileName));
         strcpy(outFileName, argdata);
         }
      else if (!strcmpi(argtype, "/r") || !strcmpi(argtype, "-r"))
         {
         memset(portFileName, 0, sizeof(portFileName));
         strcpy(portFileName, argdata);
         }
      else if (!strcmpi(argtype, "/n") || !strcmpi(argtype, "-n"))
         {
         Globals::totalSectors=(USHORT)atoi(argdata);
         if (Globals::totalSectors==0)
            BadUsage(argdata, BAD_TOTAL_SECTORS);
         }
      else if (!strcmpi(argtype, "/z") || !strcmpi(argtype, "-z"))
         {
         zTemp=(USHORT)atoi(argdata);
         if (zTemp==0)
            BadUsage(argdata, BAD_ZAP_SECTOR);
         if (!zPorts.addZap(zTemp))
            OutOfMemory();
         }
      else if (!strcmpi(argtype, "/c") || !strcmpi(argtype, "-c"))
         {
         if (!strcmp(argtype, "0"))
            Globals::minCapacity=0;
         else
            {
            Globals::minCapacity=(USHORT)atoi(argdata);
            if (Globals::minCapacity==0)
               BadUsage(argdata, BAD_CAPACITY);
            }
         }
      else if (!strcmpi(argtype, "/a") || !strcmpi(argtype, "-a"))
         {
         Globals::noCalc=true;
         }
      #if (defined(__OS2__) || defined(__EMX__)) // priority options
      else if (!strcmpi(argtype, "/p") || !strcmpi(argtype, "-p"))
         {
         Globals::pclass=(ULONG)atol(argdata);
         if (Globals::pclass==0 || Globals::pclass>4)
            BadUsage(argdata, BAD_PRIORITY_CLASS);
         }
      else if (!strcmpi(argtype, "/d") || !strcmpi(argtype, "-d"))
         {
         if (!strcmpi(argdata, "0"))
            {
            Globals::pdelta=0;
            }
         else
            {
            Globals::pdelta=(LONG)atol(argdata);
            if (Globals::pdelta==0 || Globals::pdelta<0 || Globals::pdelta>31)
               BadUsage(argdata, BAD_PRIORITY_DELTA);
            }
         }
      #endif
      else if (!strcmpi(argtype, "/h") || !strcmpi(argtype, "-h") ||
               !strcmpi(argtype, "/?") || !strcmpi(argtype, "-?"))
         {
         BadUsage("", USAGE_HELP);
         }
      else
         BadUsage(argtype, BAD_PARAMETER);
      }

   if (!strcmp(sectFileName, "") || !strcmp(portFileName, ""))
      BadUsage("", USAGE_HELP);

   if (strcmp(binPortName, "")!=0)
      {
      writeBinPorts=true;
      binPortFile.open(binPortName, ios::nocreate | ios::in | ios::binary);
      if (!binPortFile.fail())
         readBinPorts=true;
      }

   sectFile.open(sectFileName, ios::nocreate | ios::in);
   if (sectFile.fail()!=0)
      BadUsage(sectFileName, BAD_SECTOR_FILE);
   portFile.open(portFileName, ios::nocreate | ios::in);
   if (portFile.fail()!=0)
      BadUsage(portFileName, BAD_PORT_FILE);
   outFile.open(outFileName, ios::trunc | ios::out);
   if (outFile.fail()!=0)
      BadUsage(outFileName, BAD_REPORT_FILE);

   if (!Globals::BBS_SSB &&
       !Globals::xBS_xSB &&
       !Globals::BSx_SBx &&
       !Globals::xBS_xSB &&
       !Globals::xxB_xxB &&
       !Globals::BBS_BSB &&
       !Globals::xxS_xxS)
      Globals::xBS_xSB=true;

   if (!Globals::sectors.setNumberOfSectors(Globals::totalSectors))
      OutOfMemory();

   #if (defined(__OS2__) || defined(__EMX__) || defined(__OS2__))
   memset(&cursorInfo, 0, sizeof(cursorInfo));
   cursorInfo.attr=-1;
   VioSetCurType(&cursorInfo, 0);
   #endif
   #if (defined(__TCPLUSPLUS__) || defined(__DJGPP__))
   _setcursortype(_NOCURSOR);
   #endif
   // Add your own compiler's method of turning off the cursor here.

   cout << endl;
   cout << "Reading warp data for sector:      ";
   Globals::sectors.readWarpData(sectFile);
   sectFile.close();

   cout << endl << endl;
   cout << "Setting one-way warp status for sector:      " << flush;
   Globals::sectors.setOneWayStatus();

   cout << endl << endl;
   cout << "Reading port data for sector:      " << flush;
   Globals::sectors.readPortData(portFile);
   portFile.close();

   if (readBinPorts)
      {
      Globals::sectors.initHiddenPorts();
      cout << endl << endl;
      cout << "Reading binary port data for sector:      " << flush;
      Globals::sectors.readBinaryPortData(binPortFile);
      binPortFile.close();
      }

   if (zPorts.numberOfZaps()>0)
      {
      cout << endl << endl;
      cout << "Zapping port in sector:      " << flush;
      zPorts.zapPorts();
      }

   if (writeBinPorts)
      {
      binPortFile.open(binPortName, ios::out | ios::trunc | ios::binary);
      if (binPortFile.fail())
         BadUsage(binPortName, BAD_BINARY_PORT_FILE);
      cout << endl << endl;
      cout << "Writing binary port data for sector:      " << flush;
      Globals::sectors.writeBinaryPortData(binPortFile);
      binPortFile.close();
      }

   #if (defined(__OS2__) || defined(__EMX__))
   DosSetPriority(2, Globals::pclass, Globals::pdelta, 0);
   #endif

   cout << endl << endl;
   cout << "Prospecting potential pairs with primary sector:      " << flush;
   Globals::sectors.findPairs(pairSet);

   numPairs=pairSet.numberOfPairs();
   cout << endl << endl;
   cout << "Number of pairs found: " << numPairs << endl << flush;

   pairSet.writeTo(outFile);

   outFile.close();

   #ifdef DEBUG_ENABLED
   Globals::debug.close();
   #endif
   return 0;
}

extern "C" void AbortCatcher(int sigstuff)
{
   cout << endl << endl << "Aborting." << endl;
   exit(1);
}
