#define _CRT_SECURE_NO_WARNINGS
#ifndef MAPPING_H_
#define MAPPING_H_

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	TCHAR* nameFile;
	HANDLE hFile;
	HANDLE hFileMap;
	DWORD64 sizeFile;
	DWORD sizeMap;
	DWORD sizeMemory;
} TMapping;

BOOL CreateFileMap(TMapping *map, const TCHAR* nameFile, DWORD sizeMemory, DWORD64 minSizeFile, DWORD64 maxSizeFile);
BOOL CloseFileMap(TMapping *map);
DWORD64 FileSize(HANDLE hFile);
DWORD64 FileNumLine(TMapping *map, DWORD sysGranularity);
BOOL GetLine(TMapping* map, DWORD64 numLine, DWORD sysGranularity);
DWORD64 Search(TMapping* map, DWORD sizeSample, TCHAR* sample, BOOL reg, DWORD sysGranularity);
BOOL Replace(TMapping* map, DWORD sizeSample, TCHAR* sample, DWORD sizeRepl, TCHAR* repl,
	DWORD sysGranularity, DWORD64 minSizeFile, DWORD64 maxSizeFile);

#endif