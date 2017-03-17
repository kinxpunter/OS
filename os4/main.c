#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>

#include "mapping.h"

int i;
TCHAR st[126];
TCHAR st1[126];
FILE*  f;
FILE*  f1;
TCHAR buf[50];
TCHAR* sample = 0;
TCHAR* repl = 0;
DWORD sizeSample = 0;
DWORD sizeRepl = 0;
DWORD64 numLine = 0;
BOOL reg = TRUE;
TCHAR str[50];
TCHAR strd[50];
BOOL Cmpeq(const TCHAR* str1, const TCHAR* str2) {
	int n = strlen(str2);
	if (!strncmp((char*)str1, (char*)str2,n))
		return TRUE;
	else
		return FALSE;
}

DWORD ParseCom(const TCHAR *argv[]) {
	const TCHAR* com = argv[0];
	if (Cmpeq(com, TEXT("/com:getLine"))) {
		sscanf((char*)argv[1], "%lu", &numLine);
		return 1;
	}
	else if (Cmpeq(com, TEXT("/com:searchR")) || Cmpeq(com, TEXT("/com:search"))) {
		if (Cmpeq(com, TEXT("/com:searchR")))
			reg = FALSE;
		else
			reg = TRUE;
		sample = (TCHAR*)malloc(sizeof(TCHAR));
		for (sizeSample = 0; argv[1][sizeSample] != '\000'; (sizeSample)++) {
			sample = (TCHAR*)realloc(sample, sizeof(TCHAR)* (sizeSample + 1));
			sample[sizeSample] = argv[1][sizeSample];
		}
		return 2;
	}
	else if (Cmpeq(com, TEXT("/com:replace"))) {
		sample = (TCHAR*)malloc(sizeof(TCHAR));
		for (sizeSample = 0; argv[1][sizeSample] != '\000'; (sizeSample)++) {
			sample = (TCHAR*)realloc(sample, sizeof(TCHAR)* (sizeSample + 1));
			sample[sizeSample] = argv[1][sizeSample];
		}
		repl = (TCHAR*)malloc(sizeof(TCHAR));
		for (sizeRepl = 0; argv[2][sizeRepl] != '\000'; (sizeRepl)++) {
			repl = (TCHAR*)realloc(repl, sizeof(TCHAR)* (sizeRepl + 1));
			repl[sizeRepl] = argv[2][sizeRepl];
		}
		return 3;
	}
	return 0;
}


void help() {
	printf("\t one command mode\n");
	printf("/f:[name file] - select file\n");
	printf("/com:getLine [number Line] - get line\n");
	printf("/com:search [\"sample\"] - search\n");
	printf("/com:searchR [\"sample\"] - search not register\n");
	printf("/com:replace [\"sample\"] [\"replace\"] - replacing\n");
	printf("/min:[number] - min size file\n");
	printf("/max:[number] - max size file\n");
	printf("/ram:[number] - max ram (not less Granularity)\n");
	printf("/info - information file and program settings\n");
	printf("/inter - interactive mode (turns on if there is no /com)\n");
	printf("/help - help\n");
	printf("\n\t interactive mode\n");
	printf("/f:[name file] - select file\n");
	printf("/getLine [number Line] - get line\n");
	printf("/search [\"sample\"] - search\n");
	printf("/searchR [\"sample\"] - search not register\n");
	printf("/replace [\"sample\"] [\"replace\"] - replacing\n");
	printf("/min:[number] - min size file\n");
	printf("/max:[number] - max size file\n");
	printf("/ram:[number] - max ram (not less Granularity)\n");
	printf("/info - information file and program settings\n");
	printf("/inter - interactive mode (turns on if there is no /com)\n");
	printf("/help - help\n");
	printf("/exit\n");

}

int _tmain(int argc, TCHAR *argv[]) {
	TCHAR* fileName = NULL;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	DWORD sysGranularity = sysInfo.dwAllocationGranularity;

	DWORD64 minSizeFile = 0x1;
	DWORD64 maxSizeFile = 0xFFFFFFFFFFFFFFFF;
	DWORD maxSizeRam = 262144;

	BOOL bInteractive = TRUE;
	BOOL info = FALSE;

	DWORD numCommand = 0;
	//DWORD64 numLine = 0;

	//DWORD sizeSample;
	for ( i = 1; i < argc; i++) {
		if (Cmpeq(argv[i],TEXT("/com"))) {
			if (argv[i][4] != ':' || !argv[i][5]) {
				printf("ERROR: invalid usage of /com\n");
				return 1;
			}
			numCommand = ParseCom(&argv[i]);
			if (!numCommand) {
				printf("ERROR: invalid com\n");
				return 1;
			}
		}
		else if (Cmpeq(argv[i], TEXT("/f"))) {
			if (argv[i][2] != ':' || !argv[i][2]) {
				printf("ERROR: invalid usage of /f\n");
				return 2;
			}
			fileName = argv[i] + 3;
		}
		else if (Cmpeq(argv[i], TEXT("/help"))) {
			help();
			//return 0;
		}
		else if (Cmpeq(argv[i], TEXT("/min"))) {
			if (argv[i][4] != ':' || !argv[i][5]) {
				printf("ERROR: invalid usage of /min\n");
				return 3;
			}
			sscanf((char*)argv[i] + 5, "%llu", &minSizeFile);
			if (!minSizeFile) {
				printf("ERROR: invalid size specified\n");
				return 3;
			}
		}
		else if (Cmpeq(argv[i], TEXT("/max"))) {
			if (argv[i][4] != ':' || !argv[i][5]) {
				printf("ERROR: invalid usage of /min\n");
				return 4;
			}
			sscanf((char*)argv[i] + 5, "%llu", &maxSizeFile);
			if (!maxSizeFile) {
				printf("ERROR: invalid size specified\n");
				return 4;
			}
		}
		else if (Cmpeq(argv[i], TEXT("/ram"))) {
			if (argv[i][4] != ':' || !argv[i][5]) {
				printf("ERROR: invalid usage of /ram\n");
				return 5;
			}
			sscanf((char*)argv[i] + 5, "%lu", &maxSizeRam);
			if (!maxSizeRam) {
				printf("ERROR: invalid size specified\n");
				return 5;
			}
			if (maxSizeRam < sysGranularity)
				maxSizeRam = sysGranularity;
		}
		else if (Cmpeq(argv[i], TEXT("/inter"))) {
			bInteractive = TRUE;
		}
		else if (Cmpeq(argv[i], TEXT("/info"))) {
			info = TRUE;
		}
	}

	bInteractive = bInteractive && !numCommand;

	if (bInteractive) {
		TCHAR cmq[56];
		DWORD64 i;
		DWORD ram;
		TMapping* map = malloc(sizeof(TMapping));
		map->hFile = NULL;
		map->hFileMap = NULL;
		BOOL exit = TRUE;
		if (fileName) {
			if (!CreateFileMap(map, fileName, maxSizeRam, minSizeFile, maxSizeFile))
				return -1;
		}
		while (exit) {
			numCommand = 0;
			scanf("%s", cmq);
			if (Cmpeq(cmq, TEXT("/f:"))) {
				if (fileName){
					CloseFileMap(map);
					sprintf(str, "CertUtil -hashfile %s MD5 > test1.txt", fileName);
					system(str);
					f1 = fopen("test1.txt", "r");
					f = fopen("test.txt", "r");
					int flag = 1;
					while (!feof(f1))
					{
						flag = 0;
						fgets(st, 126, f1);
						while (!feof(f))
						{
							fgets(st1, 126, f);
							if (strcmp(st, st1) == 0)
							{
								flag = 0;
								break;
							}
							else

								flag = 1;

							if (flag == 1)
								printf("Modyfied file\n");
							break;
						}
					}
					fclose(f);
					fclose(f1);
					printf("%s\n", fileName);
				}
				int n = strlen(cmq);
				fileName = malloc(sizeof(char)* (n - 2));
				sscanf((char*)cmq + 3, "%s", fileName);
				sprintf(str, "CertUtil -hashfile %s MD5 > test.txt", fileName);
				system(str);
				numCommand = 4;
					
			}
			else if (Cmpeq(cmq, TEXT("/getLine"))) {
				scanf("%s", cmq);
				sscanf((char*)cmq, "%lu", &numLine);
				numCommand = 1;
			}
			else if (Cmpeq(cmq, TEXT("/searchR")) || Cmpeq(cmq, TEXT("/search"))) {
				if (Cmpeq(cmq, TEXT("/searchR")))
					reg = FALSE;
				else
					reg = TRUE;
				sample = (TCHAR*)malloc(sizeof(TCHAR));
				getchar();
				TCHAR c = getchar();
				if (c != '"') {
					printf("ERROR: invalid usage of /com:search\n");
					numCommand = 0;
					continue;
				}
				for (sizeSample = 0; (c = getchar()) != '"'; (sizeSample)++) {
					sample = (TCHAR*)realloc(sample, sizeof(TCHAR)* (sizeSample + 1));
					sample[sizeSample] = c;
				}
				numCommand = 2;
			}
			else if (Cmpeq(cmq, TEXT("/replace"))) {
				sample = (TCHAR*)malloc(sizeof(TCHAR));
				getchar();
				TCHAR c = getchar();
				if (c != '"') {
					printf("ERROR: invalid usage of /com:replace\n");
					numCommand = 0;
					continue;
				}
				for (sizeSample = 0; (c = getchar()) != '"'; (sizeSample)++) {
					sample = (TCHAR*)realloc(sample, sizeof(TCHAR)* (sizeSample + 1));
					sample[sizeSample] = c;
				}
				getchar();
				c = getchar();
				if (c != '"') {
					printf("ERROR: invalid usage of /com:replace\n");
					numCommand = 0;
					continue;
				}
				for (sizeRepl = 0; (c = getchar()) != '"'; (sizeRepl)++) {
					repl = (TCHAR*)realloc(repl, sizeof(TCHAR)* (sizeRepl + 1));
					repl[sizeRepl] = c;
				}
				numCommand = 3;
			}
			else if (Cmpeq(cmq, TEXT("/help"))) {
				help();
				numCommand = 0;
			}
			else if (Cmpeq(cmq, TEXT("/min"))) {
				if (cmq[4] != ':' || !cmq[5]) {
					printf("ERROR: invalid usage of /min\n");
					numCommand = 0;
				}
				sscanf((char*)cmq + 5, "%llu", &minSizeFile);
				if (!minSizeFile) {
					printf("ERROR: invalid size specified\n");
					numCommand = 0;
				}
			}
			else if (Cmpeq(cmq, TEXT("/max"))) {
				if (cmq[4] != ':' || !cmq[5]) {
					printf("ERROR: invalid usage of /min\n");
					numCommand = 0;
				}
				sscanf((char*)cmq + 5, "%llu", &maxSizeFile);
				if (!maxSizeFile) {
					printf("ERROR: invalid size specified\n");
					numCommand = 0;
				}
			}
			else if (Cmpeq(cmq, TEXT("/ram"))) {
				if (cmq[4] != ':' || !cmq[5]) {
					printf("ERROR: invalid usage of /ram\n");
					numCommand = 0;
				}
				sscanf((char*)cmq + 5, "%lu", &maxSizeRam);
				if (!maxSizeRam) {
					printf("ERROR: invalid size specified\n");
					numCommand = 0;
				}
				if (maxSizeRam < sysGranularity)
					maxSizeRam = sysGranularity;
			}
			else if (Cmpeq(cmq, TEXT("/exit"))) {
				numCommand = 5;
			}
			else if (Cmpeq(cmq, TEXT("/info"))) {
				numCommand = 6;
			}
			switch (numCommand) {
				case 4:
					if (map->hFileMap)
					if (!CloseFileMap(map))
						return -2;
					if (!CreateFileMap(map, fileName, maxSizeRam, minSizeFile, maxSizeFile))
						continue;
					else
						printf("ok\n");
					break;
				case 1:
					if (!map->hFile) {
						printf("file is not selected\n");
						continue;
					}
					if (!GetLine(map, numLine, sysGranularity))
						continue;
					printf("\n");
					break;
				case 2:
					if (!map->hFile) {
						printf("file is not selected\n");
						continue;
					}
					i = Search(map, sizeSample, sample,reg, sysGranularity);
					if (i == -1)
						continue;
					if (i == -2)
						printf("sample not search\n");
					else
						printf("%llu\n", i);
					break;
				case 3:
					if (!map->hFile) {
						printf("file is not selected\n");
						continue;
					}
					ram = maxSizeRam;
					if ((maxSizeRam / 2) < sysGranularity)
						maxSizeRam = 2 * sysGranularity;
					if (!CloseFileMap(map))
						return -2;
					if (!CreateFileMap(map, fileName, maxSizeRam / 2, minSizeFile, maxSizeFile))
						continue;
					if (!Replace(map, sizeSample, sample, sizeRepl, repl, sysGranularity, minSizeFile, maxSizeFile))
						continue;
					else
						printf("ok\n");
					maxSizeRam = ram;
					break;
				case 5:
					if (map->hFileMap)
					if (!CloseFileMap(map))
						return -3;
					exit = FALSE;
					break;
				case 6:
					if (!map->hFile) {
						printf("file is not selected\n");
						continue;
					}
					i = FileNumLine(map, sysGranularity);
					if (i == 0)
						return -7;
					printf("name file %s\n", map->nameFile);
					printf("size file %llu\n", map->sizeFile);
					printf("quantity line %llu\n", i);
					printf("max size file %llu\n", maxSizeFile);
					printf("min size file %llu\n", minSizeFile);
					printf("max ram %lu\n", maxSizeRam);
					printf("\n");
					break;
         }}
	}
	else {
	//	printf("min size f %llu\n max size f %llu\n max ROM %llu\n", minSizeFile, maxSizeFile, maxSizeRam);

		if (!fileName) {
			printf("file is not selected\n");
			return 0;
		}
		DWORD64 i;
		DWORD ram;
		TMapping* map = malloc(sizeof(TMapping));
		map->hFile = NULL;
		map->hFileMap = NULL;
		switch (numCommand) {
		case 1:
			if (!CreateFileMap(map, fileName, maxSizeRam, minSizeFile, maxSizeFile))
				return -1;
			if (!GetLine(map, numLine, sysGranularity))
				return -11;
			printf("\n");
			if (info) {
				i = FileNumLine(map, sysGranularity);
				if (i == 0)
					return -7;
				printf("name file %s\n", map->nameFile);
				printf("size file %llu\n", map->sizeFile);
				printf("quantity line %llu\n", i);
				printf("max size file %llu\n", maxSizeFile);
				printf("min size file %llu\n", minSizeFile);
				printf("max ram %lu\n", maxSizeRam);
				printf("\n");
				break;
			}
			if (!CloseFileMap(map))
				return -3;
			break;
		case 2:
			if (!CreateFileMap(map, fileName, maxSizeRam, minSizeFile, maxSizeFile))
				return -1;
			i = Search(map, sizeSample, sample, reg, sysGranularity);
			if (i == -1)
				return -21;
			if (i == -2)
				printf("sample not search\n");
			else
				printf("%llu\n", i);

			if (info) {
				i = FileNumLine(map, sysGranularity);
				if (i == 0)
					return -7;
				printf("name file %s\n", map->nameFile);
				printf("size file %llu\n", map->sizeFile);
				printf("quantity line %llu\n", i);
				printf("max size file %llu\n", maxSizeFile);
				printf("min size file %llu\n", minSizeFile);
				printf("max ram %lu\n", maxSizeRam);
				printf("\n");
				break;
			}
			if (!CloseFileMap(map))
				return -3;
			break;
		case 3:
			ram = maxSizeRam;
			if ((maxSizeRam / 2) < sysGranularity)
				maxSizeRam = 2 * sysGranularity;
			if (map->hFileMap)
			if (!CloseFileMap(map))
				return -2;

			if (!CreateFileMap(map, fileName, maxSizeRam / 2, minSizeFile, maxSizeFile))
				return -1;
			if (!Replace(map,sizeSample,sample,sizeRepl,repl,sysGranularity,minSizeFile,maxSizeFile))
				return -31;
			maxSizeRam = ram;
			if (info) {
				i = FileNumLine(map, sysGranularity);
				if (i == 0)
					return -7;
				printf("name file %s\n", map->nameFile);
				printf("size file %llu\n", map->sizeFile);
				printf("quantity line %llu\n", i);
				printf("max size file %llu\n", maxSizeFile);
				printf("min size file %llu\n", minSizeFile);
				printf("max ram %lu\n", maxSizeRam);
				printf("\n");
				break;
			}
			if (!CloseFileMap(map))
				return -3;
			break;
		case 0:
			if (info) {
				i = FileNumLine(map, sysGranularity);
				if (i == 0)
					return -7;
				printf("name file %s\n", map->nameFile);
				printf("size file %llu\n", map->sizeFile);
				printf("quantity line %llu\n", i);
				printf("max size file %llu\n", maxSizeFile);
				printf("min size file %llu\n", minSizeFile);
				printf("max ram %lu\n", maxSizeRam);
				printf("\n");
				break;

			}
		}
		
	}
	return 0;
}
