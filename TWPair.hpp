#include <fstream.h>
#include "TWTypes.hpp"
#include "AbEnds.hpp"

#ifndef _TWPAIR_HPP_
#define _TWPAIR_HPP_

class TWPair
{
   public:

   inline TWPair()
   {
      sect1=0;
      sect2=0;
   }

   inline TWPair(USHORT s1, USHORT s2)
   {
      sect1=s1;
      sect2=s2;
   }

   inline TWPair(TWPair &copy)
   {
      sect1=copy.sect1;
      sect2=copy.sect2;
   }

   inline Bool operator==(const TWPair& compare) const
   {
      if (sect1==compare.sect1 && sect2==compare.sect2 ||
          sect1==compare.sect2 && sect2==compare.sect1)
         return true;
      else
         return false;
   }

   inline USHORT sector1(void) const
   {
      return sect1;
   }

   inline USHORT sector2(void) const
   {
      return sect2;
   }

   inline void setSector1(USHORT newSect)
   {
      sect1=newSect;
   }

   inline void setSector2(USHORT newSect)
   {
      sect2=newSect;
   }

   void writeSectTo(UCHAR, ofstream&) const;

   private:

   USHORT sect1, sect2;
};

class TWPairSet
{
   public:

   inline TWPairSet()
   {
      numPairs=0;
      pairs=0;
   }

// Add your compiler's define if it optimizes better with the
// following functions inlined.  Be sure to modify the .CPP
// file accordingly.

   #if (defined(__IBMCPP__) || defined(__TCPLUSPLUS__))
   inline TWPairSet(TWPairSet &copy)
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

   inline Bool hasPair(TWPair &comp) const
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
   #else
   TWPairSet(TWPairSet&);
   Bool hasPair(TWPair&) const;
   #endif

   inline ~TWPairSet()
   {
      if (numPairs>0)
         delete[] pairs;
   }

   inline TWPair& pairAtPosition(USHORT index)
   {
      return pairs[index];
   }

   inline TWPair& operator[](USHORT index)
   {
      return pairs[index];
   }

   Bool addPair(TWPair&);

   inline USHORT numberOfPairs(void) const
   {
      return numPairs;
   }

   void writeTo(ofstream&) const;

   private:

   TWPair *pairs;
   USHORT numPairs;
};
#endif
