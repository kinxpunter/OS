#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include <conio.h>
#define N 100
DWORD WINAPI  dX(LPVOID lpParam);
DWORD WINAPI  Func0(LPVOID lpParam);
DWORD WINAPI Iter(LPVOID lpParam);
	HANDLE EnterM1;
	HANDLE EnterM2;
	HANDLE Res;
	DWORD ThreadsArray[N];
	DWORD PtM1;
	DWORD PtM2;
	int n;
	int i;
	float dx;
	float Func;
	float func[N];
	float array[2];
	
	
DWORD WINAPI dX(LPVOID lpParam)
{
	dx=(array[1]-array[0])/n;
	return GetLastError();
};

DWORD WINAPI Func0(LPVOID lpParam)
{
	Func=sin(array[0])+cos(array[0]);
	printf("func for 0 = %f \n",  Func);
	return GetLastError();
};

DWORD WINAPI Iter(LPVOID lpParam)
{ func[0]=Func;
  float part=  array[0]+i*dx; 
  func[i]=(sin(part)+cos(part))*func[i-1];
  printf("func for %d = %f13 \n", i, func[i]);  
  return GetLastError();        
};      





int _tmain(int argc, _TCHAR* argv[])
{
	printf("Enter nuber of points\n");
	scanf("%d", &n);
	printf("Enter bottom\n");
	scanf("%f",&array[0]);
	printf("Enter upper\n");
	scanf("%f",&array[1]);
	EnterM1= CreateThread(NULL, 0, dX, NULL, CREATE_SUSPENDED,0 );
	ResumeThread(EnterM1);
	WaitForSingleObject(EnterM1,INFINITE);
	EnterM2 = CreateThread(NULL, 0, Func0, NULL, CREATE_SUSPENDED,0 );
	ResumeThread(EnterM2);
    WaitForSingleObject(EnterM2,INFINITE);
   for(i=1;i<n;i++){
	Res = CreateThread(NULL, 0, Iter, NULL, NULL, 0);
	WaitForSingleObject(Res,INFINITE);}
	system("pause");
	return 0;
};
