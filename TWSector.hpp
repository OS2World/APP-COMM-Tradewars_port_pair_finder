#include <fstream.h>
#include <limits.h>
#include <string.h>
#include "Globals.hpp"
#include "TWTypes.hpp"
#include "AbEnds.hpp"
#include "TWPort.hpp"
#include "TWPair.hpp"
#include "TWWarp.hpp"

#ifndef _TWSECTOR_HPP_
#define _TWSECTOR_HPP_

typedef struct TWSectorData
{
   TWWarp *inWarps;
   TWWarp *outWarps;
   TWPort *thePort;
   UCHAR numInWarps;
   UCHAR numOutWarps;
   Bool explored;
};

class TWSector
{
   public:

   inline TWSector()
   {
      sdata = new TWSectorData;
      if (sdata==NULL)
         OutOfMemory();
      sdata->numInWarps=0;
      sdata->numOutWarps=0;
      sdata->inWarps=0;
      sdata->outWarps=0;
      sdata->explored=false;
      sdata->thePort=0;
   }

   inline ~TWSector()
   {
      if (sdata->numInWarps>0)
         delete[] sdata->inWarps;
      if (sdata->numOutWarps>0)
         delete[] sdata->outWarps;
      if (sdata->thePort!=NULL)
         delete sdata->thePort;
      delete sdata;
   }

   TWSector(TWSector& copy);
   TWSector& operator=(const TWSector& assign);

   inline Bool hasPort(void) const
   {
      if (sdata->thePort!=NULL)
         return true;
      else
         return false;
   }

   inline TWPort& port(void)
   {
      return *sdata->thePort;
   }

   inline Bool setPort(TWPort &newport)
   {
      if (sdata->thePort==NULL)
         {
         sdata->thePort = new TWPort(newport);
         if (sdata->thePort==NULL)
            return false;
         }
      else
         *sdata->thePort=newport;
      return true;
   }

   inline void killPort(void)
   {
      if (sdata->thePort!=NULL)
         delete sdata->thePort;
   }

   inline UCHAR numberOfInWarps(void) const
   {
      return sdata->numInWarps;
   }

   inline UCHAR numberOfOutWarps(void) const
   {
      return sdata->numOutWarps;
   }

   inline TWWarp& inWarpAtPosition(UCHAR index)
   {
      return sdata->inWarps[index];
   }

   inline TWWarp& outWarpAtPosition(UCHAR index)
   {
      return sdata->outWarps[index];
   }

   Bool addInWarp(const TWWarp &addition);
   Bool addOutWarp(const TWWarp &addition);

   inline Bool isExplored(void) const
   {
      return sdata->explored;
   }

   inline void setExplored(Bool expState)
   {
      sdata->explored=expState;
   }

// Add your compiler's define if it optimizes better with the
// following functions inlined.  Be sure to modify the .CPP
// file accordingly.

   #if (defined(__IBMCPP__) || defined(__TCPLUSPLUS__))
   inline Bool hasInWarp(USHORT testSect) const
   {
      if (sdata->numInWarps==0)
         return false;
      for (USHORT i=0; i<sdata->numInWarps; i++)
         {
         if (sdata->inWarps[i].sector()==testSect)
            return true;
         }
      return false;
   }

   inline Bool hasOutWarp(USHORT testSect) const
   {
      if (sdata->numOutWarps==0)
         return false;
      for (USHORT i=0; i<sdata->numOutWarps; i++)
         {
         if (sdata->outWarps[i].sector()==testSect)
            return true;
         }
      return false;
   }
   #else
   Bool hasInWarp(USHORT) const;
   Bool hasOutWarp(USHORT) const;
   #endif

   private:

   TWSectorData *sdata;
};

class TWSectorArray
{
   public:

   inline TWSectorArray(USHORT ns)
   {
      numSects=ns;
      sects = new TWSector[numSects];
      if (sects==NULL)
         OutOfMemory();
      bitarray=0;
   }

   inline TWSectorArray()
   {
      numSects=0;
      sects=0;
      bitarray=0;
   }

   inline ~TWSectorArray()
   {
      if (numSects>0)
         delete[] sects;
      if (bitarray!=NULL)
         delete[] bitarray;
   }

   inline Bool setNumberOfSectors(USHORT ns)
   {
      if (numSects>0)
         delete[] sects;
      numSects=ns;
      sects = new TWSector[numSects];
      if (sects==NULL)
         return false;
      else
         return true;
   }

   inline USHORT numberOfSectors(void) const
   {
      return numSects;
   }

   inline TWSector& operator[](USHORT index)
   {
      return sects[index];
   }

   inline TWSector& sectorAtPosition(USHORT index)
   {
      return sects[index];
   }

   inline void initHiddenPorts(void)
   {
      allocBits(Globals::totalSectors);
   }

   inline Bool portHiddenAt(USHORT index)
   {
      if (bitarray==NULL)
         return false;
      else
         return getbit(bitarray, index);
   }

   inline void setPortHiddenAt(USHORT index, Bool isHidden)
   {
      setbit(bitarray, index, isHidden);
   }

   void readWarpData(ifstream& dataStream);
   void readPortData(ifstream& dataStream);
   void readBinaryPortData(fstream &dataStream);
   void writeBinaryPortData(fstream &dataStream);
   void setOneWayStatus(void);
   void findPairs(TWPairSet &pairSet);

   private:

   inline void allocBits(USHORT numBits)
   {
      USHORT asize=((numBits+CHAR_BIT-1)/CHAR_BIT);
      bitarray = new char[asize];
      memset(bitarray, 0, asize);
   }

   inline Bool getbit(char *set, USHORT index)
   {
      set+=index/CHAR_BIT;
      return (*set & (1 << (index%CHAR_BIT)))!=0;    /* 0 or 1   */
   }

   inline void setbit(char *set, USHORT index, Bool value)
   {
      set += index/CHAR_BIT;
      if (value)
         *set |= 1 << (index%CHAR_BIT);     /* set bit  */
      else
         *set &= ~(1 << (index%CHAR_BIT));  /* clear bit*/
   }

   char *bitarray;
   TWSector *sects;
   USHORT numSects;
};

#endif
