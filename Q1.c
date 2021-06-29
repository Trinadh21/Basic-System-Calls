#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<stdlib.h>

struct stat sd = {0};
struct stat sdd= {0};
struct stat file = {0};
int main(int arg, char ** str)
{
	char buff[1000];
	if(arg!=2)
	{
		write(1,"Invalid number of arguments!\n",strlen("Invalid number of arguments!\n"));
		return 1;
	}

	char * name = str[1];
	int len = strlen(name);
	char new[len+50],last=0;
	for(int i=len-1;i>=0;i--)
	{
		if(name[i]=='/')
		{
			last=i+1;
			break;
		}
	}
	new[0]='.';
	new[1]='/';
	char st[20]="Assignment/";
	for(int i=2;i<2+11;i++)new[i]=st[i-2];
	for(int i=2+11;i<2+11+len-last;i++)new[i]=name[i-13+last];
	new[13+len-last]='\0';
	char dir[]="./Assignment";
//	printf("%s %s %s\n",name,new,dir);
	if(stat(name,&file)<0)
	{
		perror("NoSuch file exists");
		return 1;
	}
	else if(S_ISDIR(file.st_mode)!=0)
	{
		write(1,"Given argument is not a file but a directory\n",sizeof("Given argument is not a file but a directory\n"));
		return 1;
	}
	int fb1 = open(name,O_RDONLY);
	if(fb1<0)
	{
		perror("Please grant read permission");
		return 1;
	}
	if(stat(dir,&sd)!=-1)
	{
		if(S_ISDIR(sd.st_mode))
		chmod(dir,0700);
		else
		{
			char nay[]="Assignment is a file in the current directory so please delete that and run the code as linux doesnt support same filename and directory name in same directory\n";
			write(1,nay,sizeof(nay));
			return 1;
		}
	}
	else
	{
		mkdir(dir,0700);
	}
	int fb2;
	fb2 = open(new,O_CREAT|O_WRONLY|O_TRUNC,0700);
	if(fb2<0)
	{
		perror("Error while opening file to be written");
		return 1;
	}
	chmod(new,0600);
	long long totsize =file.st_size;
	long long var = 1000000;
	char buffer[var+5];
	char tem[var+5];
	long long curr=0,pre=totsize,back,size;
	char string2[2000];
	long double v1;
	long double v2;
	lseek(fb2,pre,SEEK_SET);
	//printf("%Ld\n",pre);
	while(curr<totsize)
	{
		size=read(fb1,buffer,var);
		back=pre-size;
		buffer[size]='\0';
		lseek(fb2,-size,SEEK_CUR);
		for(int i=0;i<size;i++)
		{
			tem[i]=buffer[size-1-i];
		}
		tem[size]='\0';
		curr=curr+size;
		pre=back;
		write(fb2,tem,size);
		lseek(fb2,-size,SEEK_CUR);
		v1=(long double)curr/(totsize);
		v2=v1*100;
		sprintf(string2,"\rPercentage written: %0.4Lf%%",v2);
		write(1,string2,strlen(string2));
	}
	sprintf(string2,"\rPercentage written: 100.0000%%\n");
	write(1,string2,strlen(string2)); 
	write(1,"FILES are getting closed please wait(max 20sec) or refer README.md",strlen("FILES are getting closed please(max 20sec) wait or refer README.md"));
	if(fb1!=-1)
		close(fb1);
	if(fb2!=-1)
		close(fb2);
	write(1,"\r                    Sucessfully closed                             \n",sizeof("\r                  Sucessfully written                             \n"));
	fflush(stdout);
	return 0;
}

