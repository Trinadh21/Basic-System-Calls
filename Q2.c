#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>

#define ll long long 
#define ld long double

ll totsize1=0,totsize2=0;
struct stat file1={0};
struct stat file2 = {0};
struct stat dir={0};
int fdir=-1,ffile1=-1,ffile2=-1,c1=-1,c2=-1,c3=-1;
void verify(char* newfile,char* oldfile,char* directory)
{
	if(stat(directory,&dir)!=-1)
	{
		if(S_ISDIR(dir.st_mode))
		{
			fdir=1;
			char mess[]="Directory is created: Yes\n";
			write(1,mess,strlen(mess));
		}
		else
		{
			fdir=-1;
			char mess[]="Third argument is not a directory\n";
			write(1,mess,sizeof(mess));
			char messs[]="Directory is created: No\n";
			write(1,messs,strlen(messs));
	
		}
	}
	else
	{	perror("Error when checking existence of directory");	
		fdir=-1;
		char  mess[]="Directory is created: No\n";
		write(1,mess,strlen(mess));
		
	}
	if(stat(newfile,&file1)<0)
	{
		ffile1=-1;
		perror("Error while checking newfiles existence");
	}
	else
	{
		if(S_ISDIR(file1.st_mode))
		{
			ffile1=-1;
			c1=-1;
			char ttrr[]="First argument ie 'newfile' is not a file but a directory\n";
			write(1,ttrr,sizeof(ttrr));
		}
		else
		{
		ffile1=1;
		c1=1;
		char  mess[]="Newfile is created: Yes\n";
		write(1,mess,strlen(mess));
		}
	}
	if(stat(oldfile,&file2)<0)
	{
		ffile2=-1;
		perror("Error while checking oldfile existence");
	}
	else
	{
		if(S_ISDIR(file2.st_mode))
		{
			ffile2=-1;
			c2=-1;
			char ttrr[]="Second argument ie 'oldfile' is not a file but a directory\n";
			write(1,ttrr,sizeof(ttrr));
		}
		else
		{
		ffile2=1;
		c2=1;
		char  mess[]="Oldfile is created: Yes\n";
		write(1,mess,strlen(mess));
		}
	}
}
ll check(char* newfile,char* oldfile)
{
	ll var=1000000;
	ll curr=0,pre=totsize1,back,size;
	char buffer[var+5];
	char tem[var+5];
	int f=1;
	ffile1=open(newfile,O_RDONLY);
	if(ffile1<0&&c1>0)perror("Error when trying to open newfile");
	ffile2=open(oldfile,O_RDONLY);
	if(ffile2<0&&c2>0)perror("Error when trying to open oldfile");
	if(ffile1<0)return -1;
	else if(ffile2<0)return -1;
	lseek(ffile2,pre,SEEK_SET);
	while(curr<totsize1)
	{
		size=read(ffile1,buffer,var);
		back=pre-size;
		lseek(ffile2,-size,SEEK_CUR);
		read(ffile2,tem,size);
		lseek(ffile2,-size,SEEK_CUR);
		for(int i=0;i<size;i++)
		{
			if(buffer[i]!=tem[size-i-1])
			{
				f=-1;
				break;
			}
		}
		curr+=size;
		pre=back;
	}
	return f;
}
int main(int arg, char ** str)
{
	if(arg!=4)
	{
		char message[] = "Invalid number of arguments!\n";
		write(1,message,strlen(message));
		return 1;
	}
	char *newfile= str[1];
	char *oldfile= str[2];
	char *directory=str[3];
	ffile1=-1;ffile2=-1;fdir=-1;
	c1=-1;c2=-1;c3=-1;
	verify(newfile,oldfile,directory);
	if(ffile1!=-1)totsize1=file1.st_size;
	if(ffile2!=-1)totsize2=file2.st_size;
	int f=check(newfile,oldfile);
	if(c1==-1||c2==-1)
	{
		char trinadh[]="Either newfile (or) oldfile donot exist OR they are failed to open because of access denial of the directory which they are in as mentioned above";
		write(1,trinadh,sizeof(trinadh));
		char mass[]="Whether file contents are reversed in newfile: NA\n";
		write(1,mass,sizeof(mass));
	}
	else if(ffile1<0||ffile2<0)
	{
		write(1,"Files exist but the read permission is NOT granted so we donot speak about reversal\n",sizeof("Files exist but the read permission is NOT granted so we donot speak about reversal\n"));
		char mass[]="Whether file contents are reversed in newfile: NA\n";
		write(1,mass,sizeof(mass));
	}
	else
	{
		write(1,"Whether file contents are reversed in newfile:",sizeof("Whether file contents are reversed in newfile:"));
		if(totsize1!=totsize2||f==-1)write(1," No\n",sizeof(" No\n"));
		else write(1," Yes\n",sizeof(" Yes\n"));
	}
	char * out[2]={" No\n"," Yes\n"};
	char * users[3]={"User","Group","Others"};
	char * oper[3]={" has read permissions on "," has write permissions on "," has execute permissions on "};
	char* files[3]={"newfile:","oldfile:","directory:"};
	struct stat tem;
	int cc=0,d,var=256;
	for(int i=0;i<3;i++)
	{
		if(i==0)
		{
			tem=file1;
			cc=c1;
		}
		else if(i==1)
		{
			tem=file2;
			cc=c2;
		}
		else
		{
			tem=dir;
			cc=fdir;
		} 
		if(cc!=-1)
		{
			var=256;
			for(int j=0;j<9;j++)
			{
				write(1,users[j/3],strlen(users[j/3]));
				write(1,oper[j%3],strlen(oper[j%3]));
				write(1,files[i],strlen(files[i]));
				d=tem.st_mode & var;
				var=var/2;
				if(d>0)d=1;
				else d=0;
				write(1,out[d],strlen(out[d]));
			}
		}
	}
	if(ffile1!=-1)close(ffile1);
	if(ffile2!=-1)close(ffile2);
	return 0;
}
