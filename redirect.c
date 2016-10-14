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
#include <fcntl.h>
#define MAX_LENGTH 1024
#define TIMEOUT (20)
#include"echo.h"
#include"pinfo.h"
#include"redirect.h"

int redirect(char *main,char *c[],int status,int start,int gol)
{
	int flag1=0,q,in=0,out=0,out1=0,gan=0,j=0;
	char input[64],output[64],output1[64];
	for(q=start;c[q]!='\0';q++)
	{
		if(strcmp(c[q],"<")==0)
		{
			c[q]=NULL;
			strcpy(input,c[q+1]);
		//	c[q+1]=NULL;
			in=1;
			gan=1;
		}
		else if(strcmp(c[q],">")==0)
		{
			c[q]=NULL;
			strcpy(output,c[q+1]);
		//	c[q+1]=NULL;
			out=1;
			gan=1;
		}
		else if(strcmp(c[q],">>")==0)
		{
			c[q]=NULL;
			strcpy(output1,c[q+1]);
			out1=1;
			gan=1;
		}
/*		if(gan!=1) 
		{
			c[j]=c[q];
			j++;
		}*/
	}
	if(gan==0)
		return 0;
	else
	{
		int pid=0;
		if(gol==0)
		{
		pid=fork();
		if(pid<0)
			perror("fork");
		}
		if(pid==0)
		{
			if(in==1)
			{
				int fd0;
				fd0= open(input, O_RDONLY,0);
				if(fd0<0)
				{
					perror("Couldn't open input file");
					exit(0);
				}
				dup2(fd0,0);
				close(fd0);
			}
			if(out==1)
			{
				int fd1;
				fd1= creat(output,0644);
				if(fd1<0)
				{
					perror("Couldn't open the output file");
					exit(0);
				}
				dup2(fd1,1);
				close(fd1);
			}
			if(out1==1)
			{
				int fd2;
				fd2=open(output1,O_WRONLY | O_APPEND | O_CREAT);
				chmod(output1,0777);
				if(fd2<0)
				{
					perror("Couldn't open");
					exit(0);
				};
				dup2(fd2,1);
				close(fd2);
			}
			if(strcmp(c[start],"pinfo")==0)
			{
				pinfo(main,c);
				exit(0);
			}
			if(strcmp(c[start],"echo")==0)
			{
				echo(c,pid);
				exit(0);
			}

			else
			{
				execvp(c[start],&c[start]);
			}

		}
		else if( gol==0)
		{
			while (!(wait(&status) == pid)) ;
		}
		return 1;
	}

}


