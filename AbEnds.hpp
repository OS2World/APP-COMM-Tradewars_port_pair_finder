#ifndef _ABENDS_HPP_
#define _ABENDS_HPP_

#include "TWTypes.hpp"

void BadUsage(char* cause, ULONG type);

void BadSector(char* sect, USHORT line, ULONG type);

void BadPortData(char* data, USHORT line, ULONG type);

void FileError(ULONG type);

void OutOfMemory(void);

#endif
