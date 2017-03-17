#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#ifdef _UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif

void menu()
{
     printf("1.Add elem\n");
      printf("2.Pre Order Display\n");
       printf("3.Post Order Display\n");
        printf("4.In Order Display\n");
         printf("5.Delete node\n");
        printf("6.Search node\n"); 
        printf("7.Exit\n");}

int _tmain(int argc, _TCHAR* argv[])
{
    	int choise=0;
	while(choise==0)
	{printf("Enter choise (0-menu): ");
	scanf ("%d", &choise);
	if(choise==0)
	{
     menu(); 
}}
	HANDLE Daddy;
	LPCSTR AdresNameFile = ("\\\\.\\pipe\\MyPipe");
	//LPWSTR AdresNameFile =TEXT ("C:\\Users\\Arkadii\\Desktop\\OS\\Data.txt");
	Daddy = CreateNamedPipe(AdresNameFile, PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1, 128, 128,
		PIPE_UNLIMITED_INSTANCES,
		NULL);
	if (Daddy == INVALID_HANDLE_VALUE)
	{
		printf("Daddy's PIPE doesn't createeeeeeeeee");
		printf("  NNNNNNOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
		system("pause");
		return GetLastError();
	}
	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
	STARTUPINFO StartupInfo; //This is an [in] parameter
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO); //Only compulsory field
	bool process = CreateProcess("C:\\Users\\L2(client).exe",
		NULL,
		NULL, NULL, true,
		CREATE_NEW_CONSOLE,// CREATE_NEW_CONSOLE|CREATE_SUSPENDED
		NULL, NULL,
		&StartupInfo,
		&ProcessInfo);
	WaitForSingleObject(ProcessInfo.hProcess, 5);
	bool connect = ConnectNamedPipe(Daddy, NULL);
	if (connect == false)
	{

		int i = GetLastError();
		if (i == 997 || i == 536)
		{
		}
		else {
			return GetLastError();
		};
	};
		while (choise!=7){
              DWORD written;
	if (!WriteFile(Daddy, &choise, sizeof(int), &written, NULL))
	{
		printf("Error! Can not write in file\n");
		system("pause");
		return GetLastError();
	}
//	WaitForSingleObject(ProcessInfo.hProcess,2);
		Sleep(1000);
	ReadFile(Daddy, &choise,sizeof(int), &written , NULL);
	printf("\nAnswer = %d\n", choise);
	printf("Enter choise (0-menu): ");
	scanf ("%d", &choise);
	if(choise==0)
	{
     menu(); 
     printf("Enter choise (0-menu): ");
	scanf ("%d", &choise);
}
	if(choise==7)
	{
        WriteFile(Daddy, &choise, sizeof(int), &written, NULL);         
            
            }     
}
	CloseHandle(Daddy);
	system("pause");
	return 0;
}
