#include <iostream.h>
#include <iomanip.h>
#include <memory.h>
#include "TWPort.hpp"
#include "Globals.hpp"
#include "TWSector.hpp"

Bool TWZappedPorts::addZap(USHORT snum)
{
   if (numZaps==0)
      {
      zaps = new USHORT[1];
      if (zaps==NULL)
         return false;
      numZaps++;
      zaps[0]=snum;
      }
   else
      {
      USHORT *tempArray = new USHORT[numZaps+1];
      if (tempArray==NULL)
         return false;
      memcpy(tempArray, zaps, sizeof(USHORT)*numZaps);
      tempArray[numZaps]=snum;
      numZaps++;
      delete[] zaps;
      zaps=tempArray;
      }
   return true;
}

void TWZappedPorts::zapPorts(void)
{
   for (USHORT i=0; i<numZaps; i++)
      {
      cout << "\b\b\b\b\b" << setw(5) << zaps[i] << flush;
      Globals::sectors[(zaps[i]-1)].killPort();
      }
}
