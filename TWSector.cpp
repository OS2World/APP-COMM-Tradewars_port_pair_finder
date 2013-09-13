#if (defined(__OS2__) || defined(__IBMCPP__) || defined(__EMX__))
   #define INCL_DOSPROCESS
   #include <os2.h>
#endif
#include <limits.h>
#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "TWRemaps.hpp"
#include "TWSector.hpp"
#include "TWPPFind.h"

#if (!defined(__IBMCPP__))  // Add your compiler's define if it has _getch and _kbhit
   #define _getch() getch()
   #define _kbhit() kbhit()
#endif

TWSector::TWSector(TWSector& copy)
{
   sdata = new TWSectorData;
   if (sdata==NULL)
      OutOfMemory();
   sdata->numInWarps=copy.sdata->numInWarps;
   if (sdata->numInWarps>0)
      {
      sdata->inWarps = new TWWarp[sdata->numInWarps];
      if (sdata->inWarps==NULL)
         OutOfMemory();
      memcpy(sdata->inWarps, copy.sdata->inWarps, sizeof(TWWarp)*sdata->numInWarps);
      }
   else
      sdata->inWarps=0;
   sdata->numOutWarps=copy.sdata->numOutWarps;
   if (sdata->numOutWarps>0)
      {
      sdata->outWarps = new TWWarp[sdata->numOutWarps];
      if (sdata->outWarps==NULL)
         OutOfMemory();
      memcpy(sdata->outWarps, copy.sdata->outWarps, sizeof(TWWarp)*sdata->numOutWarps);
      }
   else
      sdata->outWarps=0;
   sdata->explored=copy.sdata->explored;
   if (copy.sdata->thePort!=NULL)
      {
      sdata->thePort = new TWPort(*copy.sdata->thePort);
      if (sdata->thePort==NULL)
         OutOfMemory();
      }
   else
      sdata->thePort=0;
}

TWSector& TWSector::operator=(const TWSector& assign)
{
   if (this==&assign)
      return *this;
   if (sdata->numInWarps>0)
      delete[] sdata->inWarps;
   if (sdata->numOutWarps>0)
      delete[] sdata->outWarps;
   sdata->numInWarps=assign.sdata->numInWarps;
   sdata->numOutWarps=assign.sdata->numOutWarps;
   if (sdata->numInWarps>0)
      {
      sdata->inWarps = new TWWarp[sdata->numInWarps];
      if (sdata->inWarps==NULL)
         OutOfMemory();
      memcpy(sdata->inWarps, assign.sdata->inWarps, sizeof(TWWarp)*sdata->numInWarps);
      }
   else
      sdata->inWarps=0;
   if (sdata->numOutWarps>0)
      {
      sdata->outWarps = new TWWarp[sdata->numOutWarps];
      if (sdata->outWarps==NULL)
         OutOfMemory();
      memcpy(sdata->outWarps, assign.sdata->outWarps, sizeof(TWWarp)*sdata->numOutWarps);
      }
   else
      sdata->inWarps=0;
   sdata->explored=assign.sdata->explored;
   if (sdata->thePort!=NULL)
      delete sdata->thePort;
   if (assign.sdata->thePort!=NULL)
      {
      sdata->thePort = new TWPort(*assign.sdata->thePort);
      if (sdata->thePort==NULL)
         OutOfMemory();
      }
   else
      sdata->thePort=0;
   return *this;
}

Bool TWSector::addInWarp(const TWWarp& newSect)
{
   if (sdata->numInWarps==0)
      {
      sdata->inWarps = new TWWarp[1];
      if (sdata->inWarps==NULL)
         return false;
      sdata->numInWarps++;
      sdata->inWarps[0]=newSect;
      }
   else
      {
      TWWarp *tempArray = new TWWarp[sdata->numInWarps+1];
      if (tempArray==NULL)
         return false;
      memcpy(tempArray, sdata->inWarps, sizeof(TWWarp)*sdata->numInWarps);
      tempArray[sdata->numInWarps]=newSect;
      sdata->numInWarps++;
      delete[] sdata->inWarps;
      sdata->inWarps=tempArray;
      }
   return true;
}

Bool TWSector::addOutWarp(const TWWarp& newSect)
{
   if (sdata->numOutWarps==0)
      {
      sdata->outWarps = new TWWarp[1];
      if (sdata->outWarps==NULL)
         return false;
      sdata->numOutWarps++;
      sdata->outWarps[0]=newSect;
      }
   else
      {
      TWWarp *tempArray = new TWWarp[sdata->numOutWarps+1];
      if (tempArray==NULL)
         return false;
      memcpy(tempArray, sdata->outWarps, sizeof(TWWarp)*sdata->numOutWarps);
      tempArray[sdata->numOutWarps]=newSect;
      sdata->numOutWarps++;
      delete[] sdata->outWarps;
      sdata->outWarps=tempArray;
      }
   return true;
}

// Add your compiler's define if it optimizes better with the
// following functions inlined.  Be sure to modify the header
// file accordingly.

#if (!defined(__IBMCPP__) && !defined(__TCPLUSPLUS__))
Bool TWSector::hasInWarp(USHORT testSect) const
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

Bool TWSector::hasOutWarp(USHORT testSect) const
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
#endif

void TWSectorArray::readWarpData(ifstream& sectFile)
{
   char sectLine[256], tokens[7][10], *toktemp;
   USHORT i, scount=0, lastSect=0;
   Bool noMoreTokens;
   USHORT lineNum=0, numTokens, sectNum=0, currWarpSect;
   TWWarp currWarp;

   sectFile.seekg(0);
   while (!sectFile.eof())
      {
      sectFile.getline(sectLine, 255);
      if (sectFile.eof())
         {
         cout << "\b\b\b\b\b" << setw(5) << lastSect << flush;
         break;
         }
      if (sectFile.fail())
         FileError(SECTOR_READ);
      lineNum++;
      noMoreTokens=false;
      numTokens=0;
      for (i=0; i<7; i++)
         {
         memset(tokens[i], 0, sizeof(tokens[i]));
         }
      toktemp=strtok(sectLine, " ");
      if (toktemp==NULL)
         continue;
      strcpy(tokens[numTokens], toktemp);
      numTokens++;
      while (!noMoreTokens)
         {
         toktemp=strtok(NULL, " ");
         if (toktemp==NULL)
            noMoreTokens=true;
         else
            {
            strcpy(tokens[numTokens], toktemp);
            numTokens++;
            }
         }
      sectNum=(USHORT)atoi(tokens[0]);
      if (sectNum==0)
         continue;
      if (sectNum>Globals::totalSectors)
         BadSector(tokens[0], lineNum, BADSECT_TOO_LARGE);
      if (sects[sectNum-1].isExplored())
         BadSector(tokens[0], lineNum, BADSECT_DUP_DATA);

      lastSect=sectNum;

      scount++;
      if (scount%50==0)
         {
         cout << "\b\b\b\b\b" << setw(5) << sectNum << flush;
         scount=0;
         }

      sects[sectNum-1].setExplored(true);
      for (i=1; i<numTokens; i++)
         {
         currWarpSect=(USHORT)atoi(tokens[i]);
         if (currWarpSect==0)
            BadSector(tokens[i], lineNum, BADSECT_NOT_INTEGER);
         if (currWarpSect>Globals::totalSectors)
            BadSector(tokens[i], lineNum, BADSECT_TOO_LARGE);
         currWarp.setSector(currWarpSect);
         currWarp.setOneWay(false);
         if (!sects[sectNum-1].addOutWarp(currWarp))
            OutOfMemory();
         currWarp.setSector(sectNum);
         if (!sects[currWarpSect-1].addInWarp(currWarp))
            OutOfMemory();
         }
      }
}

void TWSectorArray::readPortData(ifstream& portFile)
{
   char portLine[256], pbuf[5], cbuf[7], rbuf[256], sbuf[6],
        orebuf[25], orgbuf[25], equbuf[25], tbuf[256],
        *ftemp;
   Bool buyOre, buyOrg, buyEqu;
   USHORT lineNum=0, sectNum=0, orecap, orgcap, equcap, scount=0,
          lastSect=0;
   double percent, captemp;
   TWWarp currWarp;
   TWPort tport;
   TWPortType ptype;

   portFile.seekg(0);

   while (!portFile.eof())
      {
      memset(tbuf, 0, sizeof(tbuf));
      memset(sbuf, 0, sizeof(sbuf));
      memset(rbuf, 0, sizeof(rbuf));
      memset(orebuf, 0, sizeof(orebuf));
      memset(orgbuf, 0, sizeof(orgbuf));
      memset(equbuf, 0, sizeof(equbuf));
      portFile.getline(portLine, 255);
      if (portFile.eof())
         {
         cout << "\b\b\b\b\b" << setw(5) << sectNum << flush;
         break;
         }
      if (portFile.fail())
         FileError(PORT_READ);
      lineNum++;
      strcpy(tbuf, portLine);  // so tokenization doesn't screw it up
      ftemp=strtok(tbuf, " ");
      if (ftemp==NULL)
         continue;
      strcpy(sbuf, ftemp);  // sector portion of string
      sectNum=(USHORT)atoi(sbuf);
      if (sectNum==0)
         continue;
      if (sectNum>Globals::totalSectors)
         BadSector(sbuf, lineNum, BADSECT_TOO_LARGE);
      if (sectNum<=lastSect)
         BadSector(sbuf, lineNum, BADSECT_DUP_DATA);
      lastSect=sectNum;

      scount++;
      if (scount%50==0)
         {
         cout << "\b\b\b\b\b" << setw(5) << sectNum << flush;
         scount=0;
         }

      buyOre=false;
      buyOrg=false;
      buyEqu=false;

      strcpy(rbuf, portLine);
      memset(tbuf, 0, sizeof(tbuf));
      strcpy(tbuf, portLine);
      strrev(rbuf);  // reverse string
      ftemp=strrchr(rbuf, '%');  // get ore info and sector
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(orebuf, ftemp, sizeof(orebuf)-1);
      memset(rbuf, 0, sizeof(rbuf));
      strcpy(rbuf, tbuf);
      strrev(rbuf);
      memset(tbuf, 0, sizeof(tbuf));
      strncpy(tbuf, rbuf, (strlen(rbuf)-strlen(orebuf))); // get rid of ore stuff
      ftemp=strrchr(tbuf, '%');  // get organics info
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(orgbuf, ftemp, sizeof(orgbuf)-1);
      memset(tbuf, 0, sizeof(tbuf));
      strncpy(tbuf, rbuf, (strlen(rbuf)-(strlen(orebuf)+strlen(orgbuf))));
      strncpy(equbuf, tbuf, sizeof(equbuf)-1);  // only equipment stuff

      // process ore string
      memset(pbuf, 0, sizeof(pbuf));
      memset(cbuf, 0, sizeof(cbuf));
      ftemp=strtok(orebuf, " ");
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(pbuf, ftemp, sizeof(pbuf)-1);  // percent
      strrev(pbuf);  // percent facing forward
      ftemp=strtok(NULL, " ");
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(cbuf, ftemp, sizeof(cbuf)-1);  // capacity, possibly also -
      memset(tbuf, 0, sizeof(tbuf));
      ftemp=strtok(NULL, " ");
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(tbuf, ftemp, sizeof(tbuf)-1);
      if (!strcmp(tbuf, "-"))
         buyOre=true;
      memset(tbuf, 0, sizeof(tbuf));
      strcpy(tbuf, cbuf);
      ftemp=strchr(tbuf, '-');
      if (ftemp!=NULL)
         {
         buyOre=true;
         memset(tbuf, 0, sizeof(tbuf));
         strncpy(tbuf, cbuf, (strlen(cbuf)-1));  // copy capacity without -
         memset(cbuf, 0, sizeof(cbuf));
         strncpy(cbuf, tbuf, sizeof(cbuf)-1);
         }
      strrev(cbuf);  // capacity forward again
      if (!strcmp(cbuf, "0"))
         orecap=0;
      else
         {
         orecap=(USHORT)atoi(cbuf);
         if (orecap==0)
            BadPortData(cbuf, lineNum, BADPORT_NOT_INTEGER);
         }
      memset(tbuf, 0, sizeof(tbuf));
      strncpy(tbuf, pbuf, (strlen(pbuf)-1));  // copy number without %
      if (!strcmp(tbuf, "0"))
         percent=1.0;  // percent is 0, set to one for division
      else
         {
         percent=atof(tbuf);
         if (percent==0.0)
            BadPortData(tbuf, lineNum, BADPORT_NOT_INTEGER);
         }
      if (!Globals::noCalc && percent<100.0)
         {
         captemp=(double)orecap/percent*100.0;
         if (captemp>(double)USHRT_MAX)
            orecap=USHRT_MAX; // rounding error protection
         else
            orecap=((USHORT)(captemp/100))*100;  // lop off two digits
         }

      //process organics string
      memset(pbuf, 0, sizeof(pbuf));
      memset(cbuf, 0, sizeof(cbuf));
      ftemp=strtok(orgbuf, " ");
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(pbuf, ftemp, sizeof(pbuf)-1);  // percent
      strrev(pbuf);  // percent facing forward
      ftemp=strtok(NULL, " ");
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(cbuf, ftemp, sizeof(cbuf)-1);  // capacity, possibly also -
      memset(tbuf, 0, sizeof(tbuf));
      ftemp=strtok(NULL, " ");
      if (ftemp!=NULL)
         {
         strncpy(tbuf, ftemp, sizeof(tbuf)-1);
         if (!strcmp(tbuf, "-"))
            buyOrg=true;
         }
      ftemp=strchr(cbuf, '-');
      if (ftemp!=NULL)
         {
         buyOrg=true;
         memset(tbuf, 0, sizeof(tbuf));
         strncpy(tbuf, cbuf, (strlen(cbuf)-1));  // copy capacity without -
         memset(cbuf, 0, sizeof(cbuf));
         strncpy(cbuf, tbuf, sizeof(cbuf)-1);
         }
      strrev(cbuf);  // capacity forward again
      if (!strcmp(cbuf, "0"))
         orgcap=0;
      else
         {
         orgcap=(USHORT)atoi(cbuf);
         if (orgcap==0)
            BadPortData(cbuf, lineNum, BADPORT_NOT_INTEGER);
         }
      memset(tbuf, 0, sizeof(tbuf));
      strncpy(tbuf, pbuf, (strlen(pbuf)-1));  // copy number without %
      if (!strcmp(tbuf, "0"))
         percent=1.0;  // percent is 0, set to one for division
      else
         {
         percent=atof(tbuf);
         if (percent==0.0)
            BadPortData(tbuf, lineNum, BADPORT_NOT_INTEGER);
         }
      if (!Globals::noCalc && percent<100.0)
         {
         captemp=(double)orgcap/percent*100.0;
         if (captemp>(double)USHRT_MAX)
            orgcap=USHRT_MAX; // rounding error protection
         else
            orgcap=((USHORT)(captemp/100))*100;  // lop off two digits
         }

      //process equipment string
      memset(pbuf, 0, sizeof(pbuf));
      memset(cbuf, 0, sizeof(cbuf));
      ftemp=strtok(equbuf, " ");
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(pbuf, ftemp, sizeof(pbuf)-1);  // percent
      strrev(pbuf);  // percent facing forward
      ftemp=strtok(NULL, " ");
      if (ftemp==NULL)
         BadPortData(portLine, lineNum, BADPORT_UNSPECIFIED);
      strncpy(cbuf, ftemp, sizeof(cbuf)-1);  // capacity, possibly also -
      memset(tbuf, 0, sizeof(tbuf));
      ftemp=strtok(NULL, " ");
      if (ftemp!=NULL)
         {
         strncpy(tbuf, ftemp, sizeof(tbuf)-1);
         if (!strcmp(tbuf, "-"))
            buyEqu=true;
         }
      ftemp=strchr(cbuf, '-');
      if (ftemp!=NULL)
         {
         buyEqu=true;
         memset(tbuf, 0, sizeof(tbuf));
         strncpy(tbuf, cbuf, (strlen(cbuf)-1));  // copy capacity without -
         memset(cbuf, 0, sizeof(cbuf));
         strncpy(cbuf, tbuf, sizeof(cbuf)-1);
         }
      strrev(cbuf);  // capacity forward again
      if (!strcmp(cbuf, "0"))
         equcap=0;
      else
         {
         equcap=(USHORT)atoi(cbuf);
         if (equcap==0)
            BadPortData(cbuf, lineNum, BADPORT_NOT_INTEGER);
         }
      memset(tbuf, 0, sizeof(tbuf));
      strncpy(tbuf, pbuf, (strlen(pbuf)-1));  // copy number without %
      if (!strcmp(tbuf, "0"))
         percent=1.0;  // percent is 0, set to one for division
      else
         {
         percent=atof(tbuf);
         if (percent==0.0)
            BadPortData(tbuf, lineNum, BADPORT_NOT_INTEGER);
         }
      if (!Globals::noCalc && percent<100.0)
         {
         captemp=(double)equcap/percent*100.0;
         if (captemp>(double)USHRT_MAX)
            equcap=USHRT_MAX; // rounding error protection
         else
            equcap=((USHORT)(captemp/100))*100;  // lop off two digits
         }

      // determine port type
      if (buyOre && buyOrg && !buyEqu)
         ptype=portBBS;
      else if (buyOre && !buyOrg && buyEqu)
         ptype=portBSB;
      else if (!buyOre && buyOrg && buyEqu)
         ptype=portSBB;
      else if (!buyOre && !buyOrg && buyEqu)
         ptype=portSSB;
      else if (!buyOre && buyOrg && !buyEqu)
         ptype=portSBS;
      else if (buyOre && !buyOrg && !buyEqu)
         ptype=portBSS;
      else if (!buyOre && !buyOrg && !buyEqu)
         ptype=portSSS;
      else if (buyOre && buyOrg && buyEqu)
         ptype=portBBB;

      // put port information in sector
      tport.setType(ptype);
      tport.setOreCap(orecap);
      tport.setOrgCap(orgcap);
      tport.setEquCap(equcap);
      if (!Globals::sectors[sectNum-1].setPort(tport))
         OutOfMemory();
      }
}


void TWSectorArray::setOneWayStatus(void)
{
   UCHAR warps;
   USHORT i, j, sectNum;
   TWWarp currWarp;

   for (i=0; i<Globals::totalSectors; i++)
      {
      if ((i+1)%50==0 || (i+1)==Globals::totalSectors)
         cout << "\b\b\b\b\b" << setw(5) << (i+1) << flush;
      warps=sects[i].numberOfInWarps();
      for (j=0; j<warps; j++)
         {
         currWarp=sects[i].inWarpAtPosition(j);
         sectNum=currWarp.sector();
         if (sects[i].hasOutWarp(sectNum))
            sects[i].inWarpAtPosition(j).setOneWay(false);
         else
            sects[i].inWarpAtPosition(j).setOneWay(true);
         }
      warps=sects[i].numberOfOutWarps();
      for (j=0; j<warps; j++)
         {
         currWarp=sects[i].outWarpAtPosition(j);
         sectNum=currWarp.sector();
         if (sects[i].hasInWarp(sectNum))
            sects[i].outWarpAtPosition(j).setOneWay(false);
         else
            sects[i].outWarpAtPosition(j).setOneWay(true);
         }
      }
}

void TWSectorArray::findPairs(TWPairSet &pairSet)
{
   USHORT i, sectNum;
   UCHAR warps, j;
   Bool warpsGood, adjPortExists, addToSet;
   TWPort currPort, adjPort;
   TWSector tsect, asect;
   TWPair tpair;
   PortMatched pmat;

   for (i=0; i<Globals::totalSectors; i++)
      {
      if ((i+1)%50==0 || (i+1)==Globals::totalSectors)
         cout << "\b\b\b\b\b" << setw(5) << (i+1) << flush;
      while (_kbhit())
         {
         if (_getch()==32)
            {
            fflush(stdin);
            return;
            }
         }
      tsect=Globals::sectors[i];
      if (!tsect.isExplored())
         continue;
      if (!tsect.hasPort())
         continue;
      warps=tsect.numberOfOutWarps();
      if (warps==0)
         continue;
      warpsGood=false;
      adjPortExists=false;
      for (j=0; j<warps; j++)
         {
         asect=Globals::sectors[tsect.outWarpAtPosition(j).sector()-1];
         if (!asect.isExplored())
            continue;
         if (!tsect.outWarpAtPosition(j).isOneWay())
            warpsGood=true;
         if (asect.hasPort())
            adjPortExists=true;
         }

      if (!adjPortExists || !warpsGood)
         continue;

      pmat.BBS_SSB=false;
      pmat.BxS_SxB=false;
      pmat.xBS_xSB=false;
      pmat.BSx_SBx=false;
      pmat.xxB_xxB=false;
      pmat.xxS_xxS=false;
      pmat.BBS_BSB=false;

      currPort=tsect.port();
      if (Globals::BBS_SSB)
         {
         if (currPort.buysOre() && currPort.buysOrg() && !currPort.buysEqu())
            {
            if (Globals::minCapacity>0)
               {
               if (currPort.oreCap()>Globals::minCapacity &&
                   currPort.orgCap()>Globals::minCapacity &&
                   currPort.equCap()>Globals::minCapacity)
                  pmat.BBS_SSB=true;
               }
            else
               pmat.BBS_SSB=true;
            }
         }
      if (Globals::BxS_SxB)
         {
         if (currPort.buysOre() && !currPort.buysEqu())
            {
            if (Globals::minCapacity>0)
               {
               if (currPort.oreCap()>Globals::minCapacity &&
                   currPort.equCap()>Globals::minCapacity)
                  pmat.BxS_SxB=true;
               }
            else
               pmat.BxS_SxB=true;
            }
         }
      if (Globals::xBS_xSB)
         {
         if (currPort.buysOrg() && !currPort.buysEqu())
            {
            if (Globals::minCapacity>0)
               {
               if (currPort.orgCap()>Globals::minCapacity &&
                   currPort.equCap()>Globals::minCapacity)
                  pmat.xBS_xSB=true;
               }
            else
               pmat.xBS_xSB=true;
            }
         }
      if (Globals::BSx_SBx)
         {
         if (currPort.buysOre() && !currPort.buysOrg())
            {
            if (Globals::minCapacity>0)
               {
               if (currPort.oreCap()>Globals::minCapacity &&
                   currPort.orgCap()>Globals::minCapacity)
                  pmat.BSx_SBx=true;
               }
            else
               pmat.BSx_SBx=true;
            }
         }
      if (Globals::xxB_xxB)
         {
         if (currPort.buysEqu())
            {
            if (Globals::minCapacity>0)
               {
               if (currPort.equCap()>Globals::minCapacity)
                  pmat.xxB_xxB=true;
               }
            else
               pmat.xxB_xxB=true;
            }
         }
      if (Globals::xxS_xxS)
         {
         if (!currPort.buysEqu())
            {

            if (Globals::minCapacity>0)
               {
               if (currPort.equCap()>Globals::minCapacity)
                  pmat.xxS_xxS=true;
               }
            else
               pmat.xxS_xxS=true;
            }
         }
      if (Globals::BBS_BSB)
         {
         if (currPort.buysOre() && currPort.buysOrg() && !currPort.buysEqu())
            {
            if (Globals::minCapacity>0)
               {
               if (currPort.orgCap()>Globals::minCapacity &&
                   currPort.equCap()>Globals::minCapacity)
                  pmat.BBS_BSB=true;
               }
            else
               pmat.BBS_BSB=true;
            }
         }

      if (!pmat.BBS_SSB &&
          !pmat.BxS_SxB &&
          !pmat.xBS_xSB &&
          !pmat.BSx_SBx &&
          !pmat.xxB_xxB &&
          !pmat.BBS_BSB &&
          !pmat.xxS_xxS)
         continue;

      for (j=0; j<warps; j++)
         {
         if (tsect.outWarpAtPosition(j).isOneWay())
            continue;
         sectNum=tsect.outWarpAtPosition(j).sector();
         asect=Globals::sectors[sectNum-1];
         if (!asect.isExplored())
            continue;
         if (!asect.hasPort())
            continue;
         addToSet=false;
         if (pmat.BBS_SSB)
            {
            if (asect.port().type()==portSSB)
               {
               if (Globals::minCapacity>0)
                  {
                  if (asect.port().oreCap()>Globals::minCapacity &&
                      asect.port().orgCap()>Globals::minCapacity &&
                      asect.port().equCap()>Globals::minCapacity)
                     addToSet=true;
                  }
               else
                  addToSet=true;
               }
            }
         if (pmat.BxS_SxB)
            {
            if (!asect.port().buysOre() && asect.port().buysEqu())
               {
               if (Globals::minCapacity>0)
                  {
                  if (asect.port().oreCap()>Globals::minCapacity &&
                      asect.port().equCap()>Globals::minCapacity)
                     addToSet=true;
                  }
               else
                  addToSet=true;
               }
            }
         if (pmat.xBS_xSB)
            {
            if (!asect.port().buysOrg() && asect.port().buysEqu())
               {
               if (Globals::minCapacity>0)
                  {
                  if (asect.port().orgCap()>Globals::minCapacity &&
                      asect.port().equCap()>Globals::minCapacity)
                     addToSet=true;
                  }
               else
                  addToSet=true;
               }
            }
         if (pmat.BSx_SBx)
            {
            if (!asect.port().buysOre() && asect.port().buysOrg())
               {
               if (Globals::minCapacity>0)
                  {
                  if (asect.port().oreCap()>Globals::minCapacity &&
                      asect.port().orgCap()>Globals::minCapacity)
                     addToSet=true;
                  }
               else
                  addToSet=true;
               }
            }
         if (pmat.xxB_xxB)
            {
            if (asect.port().buysEqu())
               {
               if (Globals::minCapacity>0)
                  {
                  if (asect.port().equCap()>Globals::minCapacity)
                     addToSet=true;
                  }
               else
                  addToSet=true;
               }
            }
         if (pmat.xxS_xxS)
            {
            if (!asect.port().buysEqu())
               {
               if (Globals::minCapacity>0)
                  {
                  if (asect.port().equCap()>Globals::minCapacity)
                     addToSet=true;
                  }
               else
                  addToSet=true;
               }
            }
         if (pmat.BBS_BSB)
            {
            if (asect.port().buysOre() && !asect.port().buysOrg() &&
                asect.port().buysEqu())
               {
               if (Globals::minCapacity>0)
                  {
                  if (asect.port().orgCap()>Globals::minCapacity &&
                      asect.port().equCap()>Globals::minCapacity)
                     addToSet=true;
                  }
               else
                  addToSet=true;
               }
            }
         if (addToSet)
            {
            tpair.setSector1(i+1);
            tpair.setSector2(sectNum);
            if (!pairSet.addPair(tpair))
               OutOfMemory();
            }
         }
      }
}

void TWSectorArray::readBinaryPortData(fstream &ds)
{
   USHORT scount=0;
   UCHAR ptype;
   USHORT orecap, orgcap, equcap;
   TWPort tport;

   while (!ds.eof())
      {
      if ((scount+1)%50==0)
         cout << "\b\b\b\b\b" << setw(5) << (scount+1) << flush;
      ds.read((char*)&ptype, sizeof(ptype));
      ds.read((char*)&orecap, sizeof(orecap));
      ds.read((char*)&orgcap, sizeof(orgcap));
      ds.read((char*)&equcap, sizeof(equcap));
      if (ds.eof())
         {
         if ((scount+1)<Globals::totalSectors)
            BadPortData("N/A", 0, BADPORT_BIN_DATA_SMALL);
         break;
         }
      if (ds.fail())
         FileError(BINPORT_READ);
      if ((scount+1)>Globals::totalSectors)
         BadPortData("N/A", 0, BADPORT_BIN_DATA_LARGE);
      tport.setType(ptype);
      tport.setOreCap(orecap);
      tport.setOrgCap(orgcap);
      tport.setEquCap(equcap);
      if (!Globals::sectors[scount].hasPort())
         {
         if (tport.type()>0)
            {
            Globals::sectors[scount].setPort(tport);
            Globals::sectors.setPortHiddenAt(scount, true);
            }
         }
      scount++;
      }
}

void TWSectorArray::writeBinaryPortData(fstream &ds)
{
   TWPort tport;
   UCHAR ptype;
   USHORT orecap, orgcap, equcap;

   for (USHORT i=0; i<Globals::totalSectors; i++)
      {
      if ((i+1)%50==0)
         cout << "\b\b\b\b\b" << setw(5) << (i+1) << flush;
      if (Globals::sectors[i].hasPort())
         {
         tport=Globals::sectors[i].port();
         ptype=(UCHAR)tport.type();
         orecap=tport.oreCap();
         orgcap=tport.orgCap();
         equcap=tport.equCap();
         }
      else
         {
         ptype=0;
         orecap=0;
         orgcap=0;
         equcap=0;
         }
      ds.write((char*)&ptype, sizeof(ptype));
      ds.write((char*)&orecap, sizeof(orecap));
      ds.write((char*)&orgcap, sizeof(orgcap));
      ds.write((char*)&equcap, sizeof(equcap));
      if (ds.fail())
         FileError(BINPORT_WRITE);
      }
}
