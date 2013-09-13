#include "TWTypes.hpp"

#ifndef _TWPORT_HPP_
#define _TWPORT_HPP_

class TWPort
{
   public:

   inline TWPort()
   {
      orecap=0;
      orgcap=0;
      equcap=0;
      pType=portUndefined;
   }

   inline TWPort(TWPortType ptype)
   {
      orecap=0;
      orgcap=0;
      equcap=0;
      setType(ptype);
   }

   inline TWPort(TWPort &copy)
   {
      orecap=copy.orecap;
      orgcap=copy.orgcap;
      equcap=copy.equcap;
      pType=copy.pType;
   }

   inline TWPortType type(void) const
   {
      return pType;
   }

   inline void setType(TWPortType ptype)
   {
      pType=ptype;
   }

   inline USHORT oreCap(void) const
   {
      return orecap;
   }

   inline USHORT orgCap(void) const
   {
      return orgcap;
   }

   inline USHORT equCap(void) const
   {
      return equcap;
   }

   inline void setOreCap(USHORT newVal)
   {
      orecap=newVal;
   }

   inline void setOrgCap(USHORT newVal)
   {
      orgcap=newVal;
   }

   inline void setEquCap(USHORT newVal)
   {
      equcap=newVal;
   }

   inline Bool buysOre(void) const
   {
      Bool rval;
      switch(pType)
         {
         case portBBS:
            rval=true;
            break;

         case portBSB:
            rval=true;
            break;

         case portSBB:
            rval=false;
            break;

         case portSSB:
            rval=false;
            break;

         case portSBS:
            rval=false;
            break;

         case portBSS:
            rval=true;
            break;

         case portSSS:
            rval=false;
            break;

         case portBBB:
            rval=true;
            break;
         }
      return rval;
   }

   inline Bool buysOrg(void) const
   {
      Bool rval;
      switch(pType)
         {
         case portBBS:
            rval=true;
            break;

         case portBSB:
            rval=false;
            break;

         case portSBB:
            rval=true;
            break;

         case portSSB:
            return false;

         case portSBS:
            rval=true;
            break;

         case portBSS:
            rval=false;
            break;

         case portSSS:
            rval=false;
            break;

         case portBBB:
            rval=true;
            break;
         }
      return rval;
   }

   inline Bool buysEqu(void) const
   {
      Bool rval;
      switch(pType)
         {
         case portBBS:
            rval=false;
            break;

         case portBSB:
            rval=true;
            break;

         case portSBB:
            rval=true;
            break;

         case portSSB:
            rval=true;
            break;

         case portSBS:
            rval=false;
            break;

         case portBSS:
            rval=false;
            break;

         case portSSS:
            rval=false;
            break;

         case portBBB:
            rval=true;
            break;
         }
      return rval;
   }

   private:

   TWPortType pType;
   USHORT orecap, orgcap, equcap;
};

class TWZappedPorts
{
   public:

   TWZappedPorts()
   {
      zaps=0;
      numZaps=0;
   }

   ~TWZappedPorts()
   {
      if (numZaps>0)
         delete[] zaps;
   }

   Bool addZap(USHORT snum);

   USHORT numberOfZaps(void)
   {
      return numZaps;
   }

   USHORT operator [](USHORT index)
   {
      return zaps[index];
   }

   USHORT zapAtPosition(USHORT index)
   {
      return zaps[index];
   }

   void zapPorts(void);

   private:

   USHORT *zaps;
   USHORT numZaps;
};

#endif
