//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project Name : Memory_Inspection_Tool
//  Language     : C
//  Platform     : Linux
//
//  Description  : This application analyzes Linux process information using the /proc filesystem.
//                 It allows users to inspect a process by providing its PID.
//
//                 The application performs the following tasks:
//                     1. Displays process information (Name, PID, State, Threads)
//                     2. Analyzes memory layout of the process
//                     3. Classifies memory regions such as:
//                        - Heap
//                        - Stack
//                        - Text (Executable Code)
//                        - Data (Writable Memory)
//                        - VDSO / VVAR
//                        - Anonymous regions
//                     4. Calculates memory usage of each segment in KB
//
//                 This project demonstrates concepts of:
//                     - Linux System Programming
//                     - /proc filesystem usage
//                     - Process memory management
//                     - File handling and parsing in C
//
//  Author       : Simran Naveen Tejwani
//  Date         : 08 / 03 / 2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Required Header Files
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : GetSectionType()
//  Description   : Classifies the memory section based on details and permission
//  Input         : details , permissions , section
//  Output        : Nothing
//  Author        : Simran Naveen Tejwani
//  Date          : 08 / 03 / 2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

void GetSectionType(char *details, char *perms, char *section)
{
    // Check for special memory regions
    
    if(strcmp(details,"[heap]") == 0)
    {
        strcpy(section,"HEAP");     // Heap memory (dynamic allocation)
    }
    else if(strcmp(details,"[stack]") == 0)
    {
        strcpy(section,"STACK");    // Stack memory (function calls)
    }
    else if(strcmp(details,"[vdso]") == 0)
    {
        strcpy(section,"VDSO");     // Virtual Dynamic Shared Object
    }
    else if(strcmp(details,"[vvar]") == 0)
    {
        strcpy(section,"VVAR");     // Kernel variable region
    }

    // Check permissions to classify section

    else if(perms[2] == 'x')
    {
        strcpy(section,"TEXT");     // Executable code segment
    }
    else if(perms[1] == 'w')
    {
        strcpy(section,"DATA");     // Writable data segment
    }
    else
    {
        strcpy(section,"OTHER");    // Other memory regions
    }
}// End of GetSectionType() Function.

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : ShowMemoryLayout()
//  Description   : Reads /proc/[pid]/maps and displays memory layout of process
//  Input         : Process ID
//  Output        : Nothing
//  Author        : Simran Naveen Tejwani
//  Date          : 08 / 03 / 2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

void ShowMemoryLayout(int pid)
{
	FILE *fp = NULL;
	char line[80];
	char filename[100];
	
	sprintf(filename,"/proc/%d/maps",pid);
	printf("Accesing the file : %s\n",filename);
	
	fp = fopen(filename,"r");
	
	if(fp == NULL)
	{
		printf("Unable to access system file\n");
		return;
	}
	
	printf("----------------------------------------------------------\n");	
	printf("----------------------- Memory Layout --------------------\n");
	printf("----------------------------------------------------------\n");

	printf("--- header ---\n");
	
	while(fgets(line,sizeof(line),fp) != NULL)
	{
		char address[100], perms[10], offset[20], dev[20], details[300] = "";
		unsigned long inode;
		unsigned long start , end;
		unsigned long sizekb;
		char section[50];
		int ret = 0;
		
		ret = sscanf(line,"%s %s %s %s %lu %[^\n]",address,perms,offset,dev,&inode,details);
		
		if(ret >= 5)
		{
			sscanf(address,"%lx-%lx",&start,&end);
			sizekb = (end - start) / 1024;
			
			GetSectionType(details,perms,section);	
		
			if(strlen(details) == 0)
			{
				strcpy(details,"Anonymous");
			}
			
			printf("%lx %lx %lu %s %s %s\n",start, end, sizekb,perms,section, details);
		}// End of if
		
	}//End of while

	printf("----------------------------------------------------------\n");	
	
	fclose(fp);
}// End of ShowMemoryLayout() Function

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : ShowProcessInformation()
//  Description   : Reads /proc/[pid]/status and displays key process info
//  Input         : Process ID
//  Output        : Nothing
//  Author        : Simran Naveen Tejwani
//  Date          : 08 / 03 / 2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

void ShowProcessInformation(int pid)
{
	FILE *fp = NULL;
	char line[80];
	char filename[100];
	
	sprintf(filename,"/proc/%d/status",pid);
	printf("Accesing the file : %s\n",filename);
	
	fp = fopen(filename,"r");
	
	if(fp == NULL)
	{
		printf("Unable to access system file\n");
		return;
	}
	
	printf("----------------------------------------------------------\n");	
	printf("------------------- Process Information ------------------\n");
	printf("----------------------------------------------------------\n");

	while(fgets(line,sizeof(line),fp) != NULL)
	{
		if(strncmp(line,"Name:",5) == 0 ||
		   strncmp(line,"Pid:",4) == 0 ||
		   strncmp(line,"State:",6) == 0 ||
		   strncmp(line,"Threads:",8) == 0
		)
		{	
			printf("%s\n",line);
		}
	}//End of while

	printf("----------------------------------------------------------\n");
}// End of ShowProcessInformation() Function

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name : main
//  Description   : Entry point of the program
//  Author        : Simran Naveen Tejwani
//  Date          : 08 / 03 / 2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	int pid = 0;
	
	printf("----------------------------------------------------------\n");
	printf("----------- Marvellous Process Inspector -----------------\n");	
	printf("----------------------------------------------------------\n");
	
	printf("Enter the PID of a process that you want to inspect\n");	
	scanf("%d",&pid);
	
	if(pid <= 0)
	{
		printf("Invalid PID\n");
		return -1;
	}
	
	ShowProcessInformation(pid);

	ShowMemoryLayout(pid);	
			
	return 0;
}//End of main













