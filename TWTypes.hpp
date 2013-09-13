#ifndef _TWTYPES_HPP_
#define _TWTYPES_HPP_

#define ULONG unsigned long
#define LONG long
#define USHORT unsigned short
#define UCHAR unsigned char
#define CHAR char
typedef unsigned char Bool;
#if (((__IBMCPP__ < 400) || defined(__TCPLUSPLUS__)) && !defined(__GNUC__))
   enum BoolConstants {false=0, true=1};
#endif
#define NULL 0
typedef unsigned char TWPortType;
enum TWPortTypeConstants {portUndefined, portBBS, portBSB, portSBB, portSSB,
                          portSBS, portBSS, portSSS, portBBB};
typedef struct PortMatched
{
   Bool BBS_SSB;
   Bool BxS_SxB;
   Bool xBS_xSB;
   Bool BSx_SBx;
   Bool xxB_xxB;
   Bool xxS_xxS;
   Bool BBS_BSB;
};

#endif
