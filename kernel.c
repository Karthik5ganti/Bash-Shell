#include<wait.h>
#include<stdio.h>
#include <ctype.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include <sys/utsname.h>
#include<sys/types.h>
#include <fcntl.h>
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
#include"redirect.h"
#include"pipe1.h"
#include<malloc.h>
sigjmp_buf env;
struct job
{
	char *command;
	pid_t pid;
	struct job *next;
};typedef struct job job;
job *head;
void insert(char *c[],pid_t pid,int no)
{
	job *temp;
	temp=head;
	char inputi[100][100];
	strcpy(inputi[no],c[0]);
	while(temp->next!=NULL)
	{
		temp=temp->next;
	}
	job *ptr=malloc(sizeof(job));
	ptr->command=inputi[no];
	ptr->pid=pid;
	temp->next=ptr;
}
void deletel(pid_t pid)
{
	job *temp;
	temp=head;
	while(temp->next!=NULL)
	{
		if(temp->next->pid==pid)
		{
			temp->next=temp->next->next;
			break;
		}
		if(temp!=NULL)
			temp=temp->next;
		else
			break;
	}
}		
void delete_zombies(void)
{
	pid_t kidpid;
	int status;
	while ((kidpid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		deletel(kidpid);
		printf("\nChild %d terminated\n", kidpid);
	}
	siglongjmp(env,1);
}
pid_t popi=-1;
void  sigfun(int sig)
{
	char  c;

	signal(sig, SIG_IGN);
	if(popi!=-1)
		kill(popi,SIGKILL);
}
int flagy=0;
int main()
{
	pid_t kidpid;
	void delete_zombies(void);
	char buf[300];
	char main[100];
	getcwd(main,sizeof(main));
	char *p=getenv("USER");
	char line[MAX_LENGTH];
	struct utsname unamei;
	uname(&unamei);
	head=malloc(sizeof(job));
	head->command=NULL;
	head->pid=0;
	head->next=NULL;
	int status;
	pid_t pid;
	int no=0;
	while(1)
	{
		(void) signal(SIGINT,sigfun);
//		(void) signal(SIGTSTP,sigfun1);

		printf("<%s@%s:",p,unamei.sysname);
		char len1[100];
		int y1;
		if(getcwd(buf,sizeof(buf))!=NULL)
		{
			int d=strlen(main);
			int g=strlen(buf);
			if(strcmp(buf,main)==0)
				printf("~/>");
			else if(strstr(buf,main)!=NULL && g>d)
			{
				int v=0;
				for(y1=d;y1<g;y1++)
					len1[v++]=buf[y1];
				printf("~%s>",len1);
			}
			else
				printf("%s>",buf);
		}
		for(y1=0;y1<100;y1++)
			len1[y1]='\0';
		if (!fgets(line, MAX_LENGTH, stdin)) break;
		if(line==NULL)
		{
			continue;
		}
		char *lop=line;
		if(lop==NULL)
		{
			printf("\n");
			exit(0);
		}
		int len = strlen(line);
		if (len > 1 && line[len-1] == '\n')
			line[--len] = '\0';
		char *t;
		char *l[10];
		int y=0,j=0;
		t=strtok(line,";");
		while(t!=NULL)
		{
			l[y++]=t;
			t=strtok(NULL,";");
		}
		l[y]='\0';
		while(j<y)
		{
			char *token;
			char *c[100];
			//popi=-1;
			int i=0;
			int flag=0;
			popi=-1;
			flag=0;
			token=strtok(l[j]," ");
			while(token!=NULL)
			{
				c[i++]=token;
				token=strtok(NULL," ");
			}
			if(strcmp(c[i-1],"&")==0)
			{
				flag=1;
				c[i-1]='\0';
			}
			c[i]='\0';
			int flag1=0,gan=0,pol=3;
			int temp1=pipe1(c,status,pid,main,flag);
			if(temp1==0){
				pol=redirect(main,c,status,0,0);
				if(pol==0)
				{
					if(strcmp(c[0],"\n")==0)
						flag1=1;
					if(strcmp(c[0],"cd")==0)
					{
						if(i==1)
							chdir("/main/lost-symbol");
						else
						{
							if(chdir(c[1])!=0)
								fprintf(stderr,"Error:No such file/directory\n");
						}
					}
					else if(strcmp(c[0],"echo")==0)
						echo(c,pid);
					else if(strcmp(c[0],"pinfo")==0)
						pinfo(main,c);
					else if(strcmp(c[0],"jobs")==0)
					{
						job *p=head;
						int flag=0;
						i=0;
						while(p->next!=NULL)
						{
							p=p->next;
							i++;
							printf("[%d] pid => %d   %s\n",i,p->pid,p->command);
							flag=1;
						}
						if(flag==0)
							printf("No background processes\n");
					}
					else if (strcmp(c[0],"killallbg")==0)
					{
						job *p=head;
						while(p->next!=NULL)
						{
							p=p->next;
							kill((p->pid),SIGKILL);
							deletel(p->pid);
						}
					}
					else if (strcmp(c[0],"fg")==0)
					{
						job *p=head;
						int k;
						if(c[1]!=NULL)
						 k = atoi(c[1]);
						else
							break;
						int l=1;
						while(p->next!=NULL && l<=k)
						{
							l++;
							p=p->next;
						}
						deletel(p->pid);
						printf("%d\n",p->pid);
						if(flagy==1)
						kill(p->pid,SIGCONT);
						popi=p->pid;
						deletel(popi);
						waitpid(p->pid,&status,0);
					}
					else if (strcmp(c[0],"kjob")==0)
					{
						job *p=head;
						int k = atoi(c[1]);
						int l=1;
						while(p->next!=NULL && l<=k)
						{
							l++;
							p=p->next;
						}
						kill(p->pid,k);
					}
					else if (strcmp(c[0],"quit")==0)
						exit(0);
					else  if(flag1==0)
					{
						if(flag==0)
						{
							pid=fork();
							if(pid==-1)
							{
								fprintf(stderr,"error file not forked\n");
								exit(1);
							}
							else if(pid==0)
							{
								int l=execvp(c[0],c);
								if(l==-1)
								{	
									fprintf(stderr,"process not executed\n");
									exit(1);
								}
							}
							popi=pid;
							if(flag!=1)
								waitpid(pid,&status,WUNTRACED);
						}
						else if(flag==1)
						{
							no++;
							pid=fork();
							if(pid==0)
							{
								setpgid(0,0);
								if(execvp(c[0],c)<0)
								{
									fprintf(stderr,"process not executed\n");
									exit(1);
								}

							}
							popi=pid;
							insert(c,pid,no);
							struct sigaction sa;
							sigfillset(&sa.sa_mask);
							sa.sa_handler=(void *)delete_zombies;
							sa.sa_flags=0;
							sigaction(SIGCHLD,&sa,NULL);
							sigsetjmp(env,1);
						}

					}
				}
			}

			j++;
		}
	}
	return 0;
}

