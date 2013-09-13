#include <stdlib.h>
#include <iostream.h>
#include <iomanip.h>
#include "Globals.hpp"
#include "AbEnds.hpp"
#include "TWPPFind.h"

void BadUsage(char* cause, ULONG type)
{
   cout << endl;
   cout << "Tradewars 2002 Port Pair Finder v" << (int)Globals::majorVersion << "."
        << setfill('0') << setw(2) << (int)Globals::minorVersion
        << "; by Mike Ruskai <thannymeister@yahoo.com>" << endl;
   cout << "Usage: twppfind.exe [options]" << endl << endl;
   cout << "Options:" << endl << endl;
   cout << "/n# - number of sectors in universe (100 to 65535); default 1000" << endl;
   cout << "/c# - minimum capacity of relevant products (0 to 65535); default 0" << endl;
   cout << "/a - use capacities as reported - no calculations" << endl;
   #if (defined(__OS2__) || defined(__EMX__))
   cout << "/p# - priority class (1 to 4); default=2" << endl;
   cout << "/d# - priority delta (0 to 31); default=0" << endl;
   #endif
   cout << "/z# - zap port from binary data file" << endl;
   cout << "/b<filename> - binary port data file" << endl;
   cout << "/s<filename> - sector data file" << endl;
   cout << "/r<filename> - port data file" << endl;
   cout << "/o<filename> - report file; default 'pairs.lst'" << endl;
   cout << "/t<mask> - type of ports to find:" << endl;
   cout << "           BBS_SSB - Triple-trading pairs" << endl;
   cout << "           BxS_SxB - Equipment/Fuel Ore trading" << endl;
   cout << "           xBS_xSB - Equipment/Organics trading" << endl;
   cout << "           BBS_BSB - Equipment/Organics trading, buying Fuel Ore" << endl;
   cout << "           BSx_SBx - Organics/Fuel Ore trading" << endl;
   cout << "           xxB_xxB - Sell/Steal/Move" << endl;
   cout << "           xxS_xxS - Rob/Move" << endl << endl;
   cout << "           If none are specified, xBS_xSB is assumed." << endl;
   cout << "           You may use multiple /t switches." << endl;
   cout << endl;
   cout << "Example: twppfind /n5000 /ssectors.txt /rports.txt /txbs_xsb";
   cout << endl << endl;
   cout << "The above searches for port pairs which buy and sell organics, for the most";
   cout << endl << "cost-effective cross-trading, in a universe with 5000 sectors.";
   cout << endl << "Port data is read in from 'ports.txt', sector data from 'sectors.txt',";
   cout << endl << "and the results are written to the default of 'pairs.lst'." << endl;

   switch (type)
      {
      case USAGE_HELP:
         break;

      case BAD_SECTOR_FILE:
         cout << endl << "Bad sector data filename: " << cause << endl << endl;
         break;

      case BAD_PORT_FILE:
         cout << endl << "Bad port data filename: " << cause << endl << endl;
         break;

      case BAD_REPORT_FILE:
         cout << endl << "Bad report filename: " << cause << endl << endl;
         break;

      case BAD_TOTAL_SECTORS:
         cout << endl << "Invalid number of sectors: " << cause << endl << endl;
         break;

      case BAD_PARAMETER:
         cout << endl << "Invalid parameter: " << cause << endl << endl;
         break;

      case BAD_PAIR_TYPE:
         cout << endl << "Invalid port pair type: " << cause << endl << endl;
         break;

      case BAD_CAPACITY:
         cout << endl << "Invalid capacity number: " << cause << endl << endl;
         break;

      case BAD_ZAP_SECTOR:
         cout << endl << "Invalid zap sector: " << cause << endl << endl;
         break;

      #if (defined(__OS2__) || defined(__EMX__))
      case BAD_PRIORITY_CLASS:
         cout << endl << "Invalid priority class: " << cause << endl << endl;
         break;

      case BAD_PRIORITY_DELTA:
         cout << endl << "Invalid priority delta: " << cause << endl << endl;
         break;
      #endif
      }
   exit(1);
}

void BadSector(char* sectNum, USHORT lineNum, ULONG type)
{
   cout << endl << endl;
   cout << "Bad sector data on line " << lineNum << ": " << sectNum << endl;
   cout << endl;
   switch (type)
      {
      case BADSECT_UNSPECIFIED:
         break;

      case BADSECT_TOO_LARGE:
         cout << "Exceeds number of sectors (" << Globals::totalSectors << ")." << endl;
         break;

      case BADSECT_NOT_INTEGER:
         cout << "Not a valid integer." << endl;
         break;

      case BADSECT_DUP_DATA:
         cout << "Duplicate sector data." << endl;
         break;
      }
   exit(2);
}

void BadPortData(char* badData, USHORT lineNum, ULONG type)
{
   cout << endl << endl;
   cout << "Bad port data on line " << lineNum << ": " << badData << endl;
   cout << endl;
   switch (type)
      {
      case BADPORT_UNSPECIFIED:
         break;

      case BADSECT_NOT_INTEGER:
         cout << "Not a valid integer." << endl;
         break;

      case BADPORT_BIN_DATA_SMALL:
         cout << "Not enough sectors in binary file." << endl;
         break;

      case BADPORT_BIN_DATA_LARGE:
         cout << "Too many sectors in binary file." << endl;
         break;
      }
   exit(2);
}

void FileError(ULONG type)
{
   cout << endl << endl;
   switch (type)
      {
      case SECTOR_READ:
         cout << "Error reading sector file." << endl;
         break;

      case REPORT_WRITE:
         cout << "Error writing to report file." << endl;
         break;

      case PORT_READ:
         cout << "Error reading port file." << endl;
         break;

      case BINPORT_READ:
         cout << "Error reading binary port file." << endl;
         break;

      case BINPORT_WRITE:
         cout << "Error writing binary port file." << endl;
      }
   exit(4);
}

void OutOfMemory(void)
{
   cout << endl;
   cout << "Out of memory." << endl;
   exit(3);
}
