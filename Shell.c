#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>

#define MAX 1000


int isFileExists(char* filePath);



void run_command(char *command, char *args[])
{
	char *path=getenv("PATH");
	char path_copy[MAX];
	strncpy(path_copy,path,MAX);
	char *tempDir=strtok(path_copy, ":");
	char commandPath[MAX];
	
	int fileExist=0;
	
	
	
	
	while (tempDir !=NULL && fileExist==0)
	{
		strcpy(commandPath,tempDir);
		strcat(commandPath,"/");
		strcat(commandPath,command);
		
		if (isFileExists(commandPath)==1)
		{
			fileExist = 1;
			pid_t pid=fork();
			
			if(pid==0)
			{
				execv(commandPath,args);
				exit(1);
			}
			else if (pid>0)
			{
				int status;
				waitpid(pid,&status,0);
				return;
			}
			else
			{
				exit(1);
			}
		}
		else
		{
			fileExist = 0;
		}
			
		
		tempDir=strtok(NULL,":");
	
	}
	
	printf("command not found: %s\n",command);
}




int main()
{


	char command[MAX];


	while(1)
	{
		printf("$");
		fgets(command,MAX,stdin);
		command[strlen(command)-1]='\0';
		
		if(strcmp(command,"leave")==0)
		{
			exit(1);
		}
		
		if(!strcmp(" ", command)) continue;

		
		
		char *args[MAX];
		char *arg =strtok(command," ");
		int i=0;
		
		while (arg!=NULL)
		{
			args[i]=arg;
			i++;
			arg=strtok(NULL," ");
		}
		args[i]=NULL;
		
		
		run_command(args[0],args);
		
	}
	
	return 0;
}	
		
	

int isFileExists(char* commandPath)
{
	FILE *fin;
	fin = fopen(commandPath,"r");
	if (fin==NULL)
	{	
		return 0;
	}
	fclose(fin);
	return 1;
}

