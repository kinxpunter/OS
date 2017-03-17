#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>

DWORD WINAPI  dX(LPVOID lpParam);
DWORD WINAPI  func0(LPVOID lpParam);
DWORD WINAPI Iter(LPVOID lpParam);
	HANDLE EnterM1;
	HANDLE EnterM2;
	HANDLE Res;
	DWORD ThreadsArray[n];
	DWORD PtM1;
	DWORD PtM2;
	int n;
	int i;
	float array[2];
	
	
DWORD WINAPI Dx(LPVOID lpParam)
{
	float dX;
	dX=(array[1]-array[0])/n;
	return GetLastError();
};

DWORD WINAPI func0(LPVOID lpParam)
{
	float func;
	func=sin(array[0])+cos(array[0]);
	return GetLastError();
};

DWORD WINAPI Iter(LPVOID lpParam)
{
   
  float func[n];
  func[i]=(sin(array[0]+i*dX)+cos(array[0]+i*dX))*func[i-1];
  prinf("func for %d = %f \n", &i, func[i]);  
  return GetLastError();        
};      





int _tmain(int argc, _TCHAR* argv[])
{
	printf("Enter nuber of points\n");
	scanf("%d", &n);
	printf("Enter array\n ");
	scanf("%f%c%f",array[0],' ',array[1]);
	EnterM1= CreateThread(NULL, 0, dX, NULL, CREATE_SUSPENDED,0 );
	ResumeThread(EnterM1);
	WaitForSingleObject(EnterM1,INFINITE);
	EnterM2 = CreateThread(NULL, 0, func0, NULL, CREATE_SUSPENDED,0 );
	ResumeThread(EnterM2);
    WaitForSingleObject(EnterM2,INFINITE);
    for(i=1;i<n;i++){
	Res = CreateThread(NULL, 0, Iter, NULL, NULL, 0);
	WaitForSingleObject(Res,INFINITE);}
	system("pause");
	return 0;
};
