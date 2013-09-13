#include <iostream.h>
#include <iomanip.h>
#include <memory.h>

#include "TWPair.hpp"
#include "TWPort.hpp"
#include "TWTypes.hpp"
#include "TWSector.hpp"
#include "AbEnds.hpp"
#include "Globals.hpp"
#include "TWPPFind.h"

void TWPair::writeSectTo(UCHAR sn, ofstream& ofile) const
{
   TWPort pt;
   USHORT sect;

   if (sn==1)
      sect=sect1;
   else
      sect=sect2;

   pt=Globals::sectors[sect-1].port();

   if (Globals::sectors.portHiddenAt(sect-1))
      ofile << "*";
   else
      ofile << " ";

   ofile << setw(5) << sect << " - ";

   if (pt.buysOre())
      ofile << "B";
   else
      ofile << "S";
   if (pt.buysOrg())
      ofile << "B";
   else
      ofile << "S";
   if (pt.buysEqu())
      ofile << "B";
   else
      ofile << "S";
   ofile << " - ";
   ofile << setw(5) << pt.oreCap() << " " << setw(5) << pt.orgCap()
         << " " << setw(5) << pt.equCap();
   if (ofile.fail())
      FileError(REPORT_WRITE);
}

// Add your compiler's define if it optimizes better with the
// following functions inlined.  Be sure to modify the header
// file accordingly.

#if (!defined(__IBMCPP__) && !defined(__TCPLUSPLUS__))
TWPairSet::TWPairSet(TWPairSet &copy)
{
   numPairs=copy.numPairs;
   if (numPairs>0)
      {
      pairs = new TWPair[numPairs];
      if (pairs==NULL)
         OutOfMemory();
      memcpy(pairs, copy.pairs, sizeof(TWPair)*numPairs);
      }
   else
      pairs=0;
}

Bool TWPairSet::hasPair(TWPair &comp) const
{
   if (numPairs==0)
      return false;
   for (int i=0; i<numPairs; i++)
      {
      if (pairs[i]==comp)
         return true;
      }
   return false;
}
#endif


Bool TWPairSet::addPair(TWPair &add)
{
   if (hasPair(add))
      return true;
   if (numPairs==0)
      {
      pairs = new TWPair[1];
      if (pairs==NULL)
         return false;
      numPairs++;
      pairs[0]=add;
      }
   else
      {
      TWPair *tempArray = new TWPair[numPairs+1];
      if (tempArray==NULL)
         return false;
      memcpy(tempArray, pairs, sizeof(TWPair)*numPairs);
      tempArray[numPairs]=add;
      numPairs++;
      delete[] pairs;
      pairs=tempArray;
      }
   return true;
}

void TWPairSet::writeTo(ofstream& ofile) const
{
   USHORT i, j;

   for (i=0; i<numPairs; i+=2)
      {
      if (i>=numPairs)
         break;
      if (i+1<numPairs)
         j=i+1;
      else
         j=0;
      pairs[i].writeSectTo(1, ofile);
      if (j)
         {
         ofile << "         ";
         pairs[j].writeSectTo(1, ofile);
         }
      ofile << endl;
      pairs[i].writeSectTo(2, ofile);
      if (j)
         {
         ofile << "         ";
         pairs[j].writeSectTo(2, ofile);
         }
      ofile << endl << endl;
      }
}
