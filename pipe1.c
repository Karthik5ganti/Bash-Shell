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
#include"pipe1.h"
//sigjmp_buf env;
int pipe1(char *c[],int status,pid_t pid,char *main,int flag)
{
	pid_t kidpid;
	void delete_zombies(void);
	int g=0,count=0,kg[100],gani=0,a1=0,pol1=-1;
	for(g=0;c[g]!='\0';g++)
	{
		if(strcmp(c[g],"|")==0)
		{
			c[g]='\0';
			kg[a1]=g+1;
			count++;
			gani=1;
			a1++;
		}
	}
	count+=1;
	status=0;
	if(gani==0)
		return 0;
	else if(gani==1)
	{
		int q=0;
		int fd[2*(count-1)];
		for(q=0;q<2*(count-1);q+=2)
		{
			pipe(fd+q);
		}
		pid=fork();
		if(pid==0)
		{
			dup2(fd[1], 1);
			for(q=0;q<2*(count-1);q++)
				if(q!=1)
					close(fd[q]);
			pol1=redirect(main,c,status,0,1);
			if(pol1==0)
			{
				execvp(c[0],c);
			}
		}
		else
		{
		//	waitpid(pid,&status,0);
			q=0;
			int a=-1,l;
			while(q<=2*(count-3))
			{
				a++;
				if(fork()==0)
				{
					dup2(fd[q], 0);
					dup2(fd[q+3], 1);
					for(l=0;l<2*(count-1);l++)
						//if(l!=q)
							close(fd[l]);
					pol1=redirect(main,c,status,kg[a],1);
					if(pol1==0)
						execvp(c[kg[a]],&(c[kg[a]]));
				}
				q+=2;
			}
			pid=fork();
			if(pid==0)
			{
				dup2(fd[2*(count-2)], 0);
				for(l=0;l<2*(count-1);l++)
				{
					if(l!=2*(count-2))
						close(fd[l]);
				}
				a++;
				pol1=redirect(main,c,status,kg[a],1);
				if(pol1==0)
				{
					if(flag==0)
						execvp(c[kg[a]],&(c[kg[a]]));
					else
					{
						setpgid(0,0);
						execvp(c[kg[a]],&(c[kg[a]]));
					}
				}
			}

		}
		for(q=0;q<2*(count-1);q++)
			close(fd[q]);
		if(flag==0)
		{
			for(q=0;q<=count+1;q++)
				wait(&status);
		}

	}
	return 1;
}

