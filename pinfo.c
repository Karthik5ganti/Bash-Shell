#include<wait.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include <sys/utsname.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/prctl.h>
#include<sys/wait.h>
#include<string.h>
#include<signal.h>
#include<setjmp.h>
#define MAX_LENGTH 1024
#define TIMEOUT (20)
#include"echo.h"
#include"pinfo.h"

void pinfo(char *main,char *c[])
{

	FILE *pointer;
	char temp1[1000],temp2[1000],name[1000];
	char state;
	long unsigned int size;
	int pid;
	if(c[1]==NULL)
	{
		//process id
		sprintf(temp1,"/proc/%d/stat",getpid());
		//link to the executable of this process
		sprintf(temp2,"/proc/%d/exe",getpid());
	}
	else
	{
		//filename of exectable(token[1])
		sprintf(temp1,"/proc/%s/stat",(c[1]));
		sprintf(temp2,"/proc/%s/exe",(c[1]));
	}
	if((pointer=fopen(temp1,"r"))!=NULL)
	{
		/*scan the stat file*/
		fscanf(pointer,"%d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d",&pid,&state,&size);
		fclose(pointer);
		printf("PID -- %d\nProcess Status -- %c\nMemory -- %lu\n",pid,state,size);
		readlink(temp2,name,256);
		if(strstr(name,main))
		{
			char tilda[1000]="~";
			strcat(tilda,name+strlen(main));
			printf("Executable Path -- %s\n",tilda);    //print print the stuff

		}
		else    
			printf("Executable Path -- %s\n",name);
	}
	else    
		fprintf(stderr,"No such process\n");
}

