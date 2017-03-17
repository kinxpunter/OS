#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include "tree.c"
typedef struct bin_tree node;

node* dummy_tree()
{
 node *root= NULL;     
 insert(&root, 2);   
 insert(&root, 4);   
 insert(&root, 3);    
 insert(&root, 6);
 insert(&root, 1);
 insert(&root, 7);
 insert(&root, 2);
 insert(&root, 7);
 insert(&root, 1);
 return root;
}

int _tmain(int argc, _TCHAR* argv[])
{
    
    int i;
  node *root=NULL;
  node *tmp=NULL;
  root=dummy_tree();

	int choise=0;
	HANDLE Son;
	LPCSTR AdresNameFile = "\\\\.\\pipe\\MyPipe";
	Sleep (12);
	Son = CreateFile(AdresNameFile, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	if (Son == INVALID_HANDLE_VALUE)
	{
		printf("I'm Son.\n Daddy doesn't createeeeeeeeee");
		getchar();
		printf("NNNNNNOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
		return GetLastError();
	};
//	bool connect = ConnectNamedPipe(Son, &l);
	/*if (connect == false)
	{

		int i = GetLastError();
		if (i == 997 || i == 536)
		{
		}
		else {
			return GetLastError();
		};
	};*/
	DWORD in;
	while(choise!=7)
{
	if (!ReadFile(Son, &choise,sizeof(int), &in , NULL))
	/*{
		printf("I CAN'T READ FILE");
		return GetLastError();
	};*/
	/*Son = CreateFile(AdresNameFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (Son == INVALID_HANDLE_VALUE)
	{
		printf("Son doesn't open file");
		getchar();
		printf("NNNNNNOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
		ExitProcess(1);
		return 1;

	};*/	
printf("%d", choise);
if(choise==1)
	{
        printf("Enter elem\n");
        scanf("%d", &i);
        insert(&root, i);
         print_inorder(root);
        printf("Elem in tree\n");

}                   
if(choise==2)
{
     printf("Pre Order Display\n");
    print_preorder(root); 

 
            
}
if(choise==3)
{
     printf("Post Order Display\n");
    print_postorder(root);  
      
    
}
if(choise==4)
{
     printf("In Order Display\n");
    print_inorder(root);

    	       
}

if(choise==6)
{
     printf("Search node. Enter one\n");
     scanf("%d", &i);
     tmp = find(root, i);
    if (tmp)
    {
        printf("Searched node=%d\n", tmp->data);
    }
    else
    {
        printf("Data Not found in tree.\n");
    }

       	
}
if(choise==5)
{
 printf("enter node value\n");
  scanf("%d", &i);
  tree_remove(&root, i);           
}             
if(choise>8 && choise<1)
{printf("wrong choise");
}
DWORD write;
WriteFile(Son, &choise, sizeof(int), &write, NULL);
	if (Son == INVALID_HANDLE_VALUE)
	{
		printf("Error! Can not write data in file\n");
		ExitProcess(1);
		return 1;
	}
	Sleep(100);
}
if(choise==7)
{
     printf("Exit\n");
 //   char Out[20];
 // 	char *PtrToOut;
 // 	PtrToOut = Out;
 // 	wsprintf(PtrToOut, "%d", SUM);
 // 	WriteFile(Son, PtrToOut, 15, 0, NULL);
 // 	if (Son == INVALID_HANDLE_VALUE)
 // 	{
 // 		printf("Error! Can not write data in file\n");
 // 		ExitProcess(1);
 // 		return 1;
 // 	};   
	ExitProcess(1);
	return 0;         
}	


	ExitProcess(1);
	return 0;
}
LPCVOID Error(int i)
{
	LPCVOID Errors;
	if (i == 1)
	{
		Errors = TEXT("you bad write \n");
		return Errors;
	};
	return 0;
};





