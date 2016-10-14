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

void echo(char *c[],pid_t pid)
{

	if(c[1][0]=='$')
	{
		c[1][0]=' ';
		int t,o=strlen(c[1]);
		for(t=0;t<o;t++)
		{
			if(t<=o-1)
			{
				c[1][t]=c[1][t+1];
			}
			else
				c[1][t]='\0';
		}
		if(getenv(c[1])!=NULL)
			printf("%s\n",getenv(c[1]));
		else
			printf("\n");
		return;
	}
	char kar[100];
	int t=0;
	int v=0;
	int o=strlen(c[1]);
	for(t=0;t<o;t++)
	{
		if(c[1][t]!='"')
			kar[v++]=c[1][t];
	}
	kar[v]='\0';
	strcpy(c[1],kar);
	{
		pid=fork();
		if(pid==0)
		{
			execvp(c[0],c);
			exit(0);
		}
		wait(NULL);
	}
}

