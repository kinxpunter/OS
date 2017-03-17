#include "mapping.h"


int CreateFileMap(TMapping *map, const TCHAR* nameFile, DWORD sizeMemory, DWORD64 minSizeFile, DWORD64 maxSizeFile){
	if (minSizeFile > maxSizeFile) {
		printf("ERROR: wrong file size limit\n ");
		return FALSE;
	}

	map->sizeMemory = sizeMemory;

	map->hFile = CreateFile(nameFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (map->hFile == INVALID_HANDLE_VALUE) {
		printf("ERROR: can't open file\n");
		map->hFile = NULL;
		map->hFileMap = NULL;
		return FALSE;
	}
	map->sizeFile = FileSize(map->hFile);
	if (!map->sizeFile) {
		CloseHandle(map->hFile);
		map->hFile = NULL;
		map->hFileMap = NULL;
		printf("ERROR: empty file\n");

		return FALSE;
	}
	if (minSizeFile > map->sizeFile || maxSizeFile < map->sizeFile) {
		printf("the file does not meet the limits\n");
		CloseHandle(map->hFile);
		map->hFile = NULL;
		map->hFileMap = NULL;
		return FALSE;
	}
	map->hFileMap = CreateFileMapping(map->hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (map->hFileMap == NULL) {
		CloseHandle(map->hFile);
		printf("ERROR: create mapping\n");
		map->hFile = NULL;
		map->hFileMap = NULL;
		return FALSE;
	}
	int size = strlen(nameFile);
	map->nameFile = malloc(sizeof(char)* (size + 1));
	strcpy(map->nameFile, nameFile);
	return TRUE;
}


BOOL CloseFileMap(TMapping* map) {
	BOOL f;
	BOOL ans = TRUE;
	f = CloseHandle(map->hFileMap);
	if (!f) {
		printf("ERROR: close map\n");
		ans = FALSE;
	}
	f = CloseHandle(map->hFile);
	if (!f) {
		printf("ERROR: close file\n");
		ans = FALSE;
	}
	map->hFile = NULL;
	map->hFileMap = NULL;
	return ans;
}

DWORD64 FileSize(HANDLE hFile) {
	DWORD hight, lower;
	lower = GetFileSize(hFile, &hight);
	return (DWORD64)hight << 32 | lower;
}


DWORD64 FileNumLine(TMapping *map, DWORD sysGranularity) {
	if (map == NULL) {
		printf("ERROR:map not create\n");
		return FALSE;
	}
	DWORD64 indexLine = 1;
	DWORD64 indexRead = 0;
	DWORD startMapping;
	DWORD sizeView;
	BOOL notEnd = TRUE;
	TCHAR* readingInMap;
	LPVOID pointerMap;
	while (notEnd) {
		startMapping = (indexRead / sysGranularity) * sysGranularity;
		sizeView = (indexRead % sysGranularity) + ((indexRead + map->sizeMemory) < map->sizeFile ? map->sizeMemory : (map->sizeFile - indexRead));
		pointerMap = MapViewOfFile(map->hFileMap, FILE_MAP_READ, (DWORD)(indexRead >> 0x20), startMapping, sizeView);
		if (pointerMap == NULL) {
			printf("ERROR:unmap view\n");
			return 0;
		}
		int distanceToStartRead = indexRead - startMapping;
		readingInMap = (TCHAR*)pointerMap + distanceToStartRead;
		int sizeReadingInMap = sizeView - distanceToStartRead;
		for (int i = 0; i < sizeReadingInMap; i++) {
			if (readingInMap[i] == '\r' && readingInMap[i + 1] == '\n'){
				i++;
				indexLine++;
			}
		}

		if ((startMapping + sizeView) == map->sizeFile)
			notEnd = FALSE;
		if (!UnmapViewOfFile(pointerMap)) {
			printf("ERROR: unmap view\n");
			return 0;
		}

		indexRead += sizeReadingInMap;

	}
	return indexLine;

}

BOOL GetLine(TMapping* map, DWORD64 numLine, DWORD sysGranularity) {
	if (map == NULL) {
		printf("ERROR:map not create\n");
		return FALSE;
	}
	DWORD64 indexLine = 1;
	DWORD64 indexRead = 0;
	DWORD startMapping;
	DWORD sizeView;
	BOOL notEnd = TRUE;
	TCHAR* readingInMap;
	LPVOID pointerMap;
	while (notEnd) {
		startMapping = (indexRead / sysGranularity) * sysGranularity;
		sizeView = (indexRead % sysGranularity) + ((indexRead + map->sizeMemory) < map->sizeFile ? map->sizeMemory : (map->sizeFile - indexRead));
		pointerMap = MapViewOfFile(map->hFileMap, FILE_MAP_READ, (DWORD)(indexRead >> 0x20), startMapping, sizeView);
		if (pointerMap == NULL) {
			printf("ERROR:unmap view\n");
			return FALSE;
		}
		int distanceToStartRead = indexRead - startMapping;
		readingInMap = (TCHAR*)pointerMap + distanceToStartRead;
		int sizeReadingInMap = sizeView - distanceToStartRead;
		for (int i = 0; i < sizeReadingInMap; i++) {
			if (indexLine == numLine)
				printf("%c", readingInMap[i]);
			if (readingInMap[i] == '\r' && readingInMap[i + 1] == '\n'){
				i++;
				indexLine++;
			}
			if (indexLine > numLine) {
				notEnd = FALSE;
				break;
			}
		}

		if ((startMapping + sizeView) == map->sizeFile)
			notEnd = FALSE;
		if (!UnmapViewOfFile(pointerMap)) {
			printf("ERROR: unmap view\n");
			return FALSE;
		}

		indexRead += sizeReadingInMap;

	}
	return TRUE;
}

DWORD64 Search(TMapping* map, DWORD sizeSample, TCHAR* sample, BOOL reg, DWORD sysGranularity) {
	if (map == NULL) {
		printf("ERROR:map not create\n");
		return -1;
	}
	if (sizeSample == 0) {
		return -2;
	}

	DWORD64 indexRead = 0;
	DWORD startMapping;
	DWORD sizeView;
	BOOL notEnd;
	BOOL yes = FALSE;
	TCHAR* readingInMap;
	LPVOID pointerMap;
	int indexSample;
	DWORD64 i;
	DWORD64 num = -2;
	for (i = 0; i < (map->sizeFile - sizeSample + 1); i++) {
		indexSample = 0;
		indexRead = i;
		notEnd = TRUE;
		while (notEnd) {
			startMapping = (indexRead / sysGranularity) * sysGranularity;
			sizeView = (indexRead % sysGranularity) + ((indexRead + map->sizeMemory) < map->sizeFile ? map->sizeMemory : (map->sizeFile - indexRead));
			pointerMap = MapViewOfFile(map->hFileMap, FILE_MAP_READ, (DWORD)(indexRead >> 0x20), startMapping, sizeView);
			if (pointerMap == NULL) {
				printf("ERROR:map view\n");
				return -1;
			}
			int distanceToStartRead = indexRead - startMapping;
			readingInMap = (TCHAR*)pointerMap + distanceToStartRead;
			int sizeReadingInMap = sizeView - distanceToStartRead;
			for (int i = 0; i < sizeReadingInMap && indexSample < sizeSample; i++) {
				if (reg) {
					if (readingInMap[i] != sample[indexSample]) {
						notEnd = FALSE;
						break;
					}
				}
				else {
					if ((char)tolower(readingInMap[i]) != (char)tolower(sample[indexSample])) {
						notEnd = FALSE;
						break;
					}
				}
				indexSample++;
			}
			if (indexSample == sizeSample) {
				yes = TRUE;
				num = i;
				notEnd = FALSE;
			}

			if ((startMapping + sizeView) == map->sizeFile)
				notEnd = FALSE;

			if (!UnmapViewOfFile(pointerMap)) {
				printf("ERROR: unmap view\n");
				return -1;
			}

			indexRead += sizeReadingInMap;
		}
		if (yes)
			break;
	}
	return num;
}


BOOL Replace(TMapping* map, DWORD sizeSample, TCHAR* sample, DWORD sizeRepl, TCHAR* repl,
	DWORD sysGranularity, DWORD64 minSizeFile, DWORD64 maxSizeFile) {

	DWORD64 startRepl = Search(map, sizeSample, sample, FALSE, sysGranularity);
	if (startRepl == -2) {
		printf("Sample not found in file\n");
		return TRUE;
	}
	TMapping* mapRepl = malloc(sizeof(TMapping));
	TCHAR nameFileRepl[10] = TEXT("~2589");
	mapRepl->hFile = CreateFile(nameFileRepl, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (mapRepl->hFile == INVALID_HANDLE_VALUE) {
		printf("ERROR: can't create file\n");
		return FALSE;
	}
	mapRepl->sizeMemory = map->sizeMemory;
	mapRepl->sizeFile = map->sizeFile - sizeSample + sizeRepl;
	mapRepl->nameFile = nameFileRepl;
	if (!mapRepl->sizeFile) {
		if (!CloseFileMap(map))
			return FALSE;
		if (!CloseHandle(mapRepl->hFile)) {
			printf("ERROR: close file\n");
			return FALSE;
		}
		DeleteFile(map->nameFile);
		MoveFile(mapRepl->nameFile, map->nameFile);
		printf("close file (file empty)\n");
		map->sizeFile = mapRepl->sizeFile;
		free(mapRepl);
		return TRUE;
	}
	if (minSizeFile > mapRepl->sizeFile || maxSizeFile < mapRepl->sizeFile) {
		printf("ERROR: after replacing the file will not comply with limits\n ");
		return TRUE;
	}
	mapRepl->hFileMap = CreateFileMapping(mapRepl->hFile, NULL, PAGE_READWRITE, (DWORD)(mapRepl->sizeFile >> 0x20), (DWORD)mapRepl->sizeFile, NULL);
	if (mapRepl->hFileMap == NULL) {
		printf("ERROR: create mapping\n");
		return FALSE;
	}
	mapRepl->sizeMemory = map->sizeMemory;


	DWORD64 indexRead1 = 0;
	DWORD64 indexRead2 = 0;
	DWORD startMapping1;
	DWORD startMapping2;
	DWORD sizeView1;
	DWORD sizeView2;
	BOOL notEnd = TRUE;
	TCHAR* readingInMap1;
	TCHAR* readingInMap2;
	LPVOID pointerMap1;
	LPVOID pointerMap2;
	while (notEnd) {
		startMapping1 = (indexRead1 / sysGranularity) * sysGranularity;
		sizeView1 = (indexRead1 % sysGranularity) + ((indexRead1 + map->sizeMemory) < map->sizeFile ? map->sizeMemory : (map->sizeFile - indexRead1));
		pointerMap1 = MapViewOfFile(map->hFileMap, FILE_MAP_READ, (DWORD)(indexRead1 >> 0x20), startMapping1, sizeView1);
		if (pointerMap1 == NULL) {
			printf("ERROR:map view \n");
			return FALSE;
		}
		int distanceToStartRead1 = indexRead1 - startMapping1;
		readingInMap1 = (TCHAR*)pointerMap1 + distanceToStartRead1;
		int sizeReadingInMap = sizeView1 - distanceToStartRead1;

		startMapping2 = (indexRead2 / sysGranularity) * sysGranularity;
		sizeView2 = (indexRead2 % sysGranularity) + ((indexRead2 + mapRepl->sizeMemory) < mapRepl->sizeFile ? mapRepl->sizeMemory : (mapRepl->sizeFile - indexRead2));
		pointerMap2 = MapViewOfFile(mapRepl->hFileMap, FILE_MAP_WRITE, (DWORD)(indexRead2 >> 0x20), startMapping2, sizeView2);
		if (pointerMap2 == NULL) {
			printf("ERROR:map view\n");
			return FALSE;
		}
		int distanceToStartRead2 = indexRead2 - startMapping2;
		readingInMap2 = (TCHAR*)pointerMap2 + distanceToStartRead2;

		for (int i = 0; i < sizeReadingInMap; i++) {
			if ((indexRead1 + i) == startRepl) {
				notEnd = FALSE;
				break;
			}
			readingInMap2[i] = readingInMap1[i];
		}

		/*	if ((startMapping2 + sizeView2) == map->sizeFile)
		notEnd = FALSE;*/
		if (!UnmapViewOfFile(pointerMap1)) {
			printf("ERROR: unmap view\n");
			return FALSE;
		}
		if (!UnmapViewOfFile(pointerMap2)) {
			printf("ERROR: unmap view\n");
			return FALSE;
		}
		indexRead1 += sizeReadingInMap;
		indexRead2 += sizeReadingInMap;
	}

	indexRead2 = startRepl;
	notEnd = TRUE;
	int indexInRepl = 0;

	while (notEnd) {
		startMapping2 = (indexRead2 / sysGranularity) * sysGranularity;
		sizeView2 = (indexRead2 % sysGranularity) + ((indexRead2 + mapRepl->sizeMemory) < mapRepl->sizeFile ? mapRepl->sizeMemory : (mapRepl->sizeFile - indexRead2));
		pointerMap2 = MapViewOfFile(mapRepl->hFileMap, FILE_MAP_WRITE, (DWORD)(indexRead2 >> 0x20), startMapping2, sizeView2);
		if (pointerMap2 == NULL) {
			printf("ERROR:map view\n");
			return FALSE;
		}
		int distanceToStartRead = indexRead2 - startMapping2;
		readingInMap2 = (TCHAR*)pointerMap2 + distanceToStartRead;
		int sizeReadingInMap = sizeView2 - distanceToStartRead;
		for (int i = 0; i < sizeReadingInMap; i++) {
			if (indexInRepl < sizeRepl)
				readingInMap2[i] = repl[indexInRepl];
			else {
				notEnd = FALSE;
				break;
			}
			indexInRepl++;
		}

		/*if ((startMapping2 + sizeView2) == mapRepl->sizeFile)
		notEnd = FALSE;*/
		if (!UnmapViewOfFile(pointerMap2)) {
			printf("ERROR: unmap view\n");
			return FALSE;
		}

		indexRead2 += sizeReadingInMap;

	}
	if ((startMapping2 + sizeRepl) == mapRepl->sizeFile)
		notEnd = FALSE;
	else
		notEnd = TRUE;
	indexRead2 = startRepl + sizeRepl;
	indexRead1 = startRepl + sizeSample;

	while (notEnd) {
		startMapping1 = (indexRead1 / sysGranularity) * sysGranularity;
		sizeView1 = (indexRead1 % sysGranularity) + ((indexRead1 + map->sizeMemory) < map->sizeFile ? map->sizeMemory : (map->sizeFile - indexRead1));
		pointerMap1 = MapViewOfFile(map->hFileMap, FILE_MAP_READ, (DWORD)(indexRead1 >> 0x20), startMapping1, sizeView1);
		if (pointerMap1 == NULL) {
			printf("ERROR:map view\n");
			return FALSE;
		}
		int distanceToStartRead1 = indexRead1 - startMapping1;
		readingInMap1 = (TCHAR*)pointerMap1 + distanceToStartRead1;
		int sizeReadingInMap = sizeView1 - distanceToStartRead1;

		startMapping2 = (indexRead2 / sysGranularity) * sysGranularity;
		sizeView2 = (indexRead2 % sysGranularity) + ((indexRead2 + mapRepl->sizeMemory) < mapRepl->sizeFile ? mapRepl->sizeMemory : (mapRepl->sizeFile - indexRead2));
		pointerMap2 = MapViewOfFile(mapRepl->hFileMap, FILE_MAP_WRITE, (DWORD)(indexRead2 >> 0x20), startMapping2, sizeView2);
		if (pointerMap2 == NULL) {
			printf("ERROR:map view\n");
			return FALSE;
		}
		int distanceToStartRead2 = indexRead2 - startMapping2;
		readingInMap2 = (TCHAR*)pointerMap2 + distanceToStartRead2;

		for (int i = 0; i < sizeReadingInMap; i++)
			readingInMap2[i] = readingInMap1[i];

		if ((startMapping2 + sizeView2) == mapRepl->sizeFile)
			notEnd = FALSE;
		if (!UnmapViewOfFile(pointerMap1)) {
			printf("ERROR: unmap view\n");
			return FALSE;
		}
		if (!UnmapViewOfFile(pointerMap2)) {
			printf("ERROR: unmap view\n");
			return FALSE;
		}
		indexRead1 += sizeReadingInMap;
		indexRead2 += sizeReadingInMap;
	}
	if (!CloseFileMap(map))
		return FALSE;
	if (!CloseFileMap(mapRepl))
		return FALSE;
	DeleteFile(map->nameFile);
	MoveFile(mapRepl->nameFile, map->nameFile);
	map->hFile = CreateFile(map->nameFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (map->hFile == INVALID_HANDLE_VALUE) {
		printf("ERROR: can't open file\n");
		return FALSE;
	}
	map->hFileMap = CreateFileMapping(map->hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (map->hFileMap == NULL) {
		printf("ERROR: create mapping\n");
		return FALSE;
	}
	map->sizeFile = mapRepl->sizeFile;
	free(mapRepl);

	return TRUE;
}