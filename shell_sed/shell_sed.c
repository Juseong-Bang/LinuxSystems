#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<dirent.h>
#include<pwd.h>

#define MAXLINE 2048
#define  MAX_INPUT 1024

int ssu_sed(char *[],int );
int fins(char* ,char **,int *);
void opt(char **,int,int[]);
int findir(char* ,char**,int *);
int par(char *,char*,char *,int,int) ;
int cp(char [],char []);
int mlen(char *);
int mcmp(char*,char*);
char cap(char ,int);
int depth=0;

int main()
{
		struct timeval st,et;
		char CmdInBuf[MAX_INPUT];
		char CmdInBuf2[MAX_INPUT];
		char temp[MAX_INPUT];
		system("clear");
		while(1){

				for(int i=0;i<MAX_INPUT;i++)
				{
						CmdInBuf[i]='\0';//remove all value inside of input array
				}
				char *input[10]={NULL, };
				printf("20132366 $");//print prompt
				fgets(CmdInBuf,MAX_INPUT,stdin);//stdin input store at CmdInBuf
				cp(CmdInBuf2,CmdInBuf);//stdin store at CmdInBuf2
				gettimeofday(&st,NULL);//time start
				char *ptr=strtok(CmdInBuf," 	\n");
				int argtok=0;//store number of input
				char *esptemp;
				while(ptr!=NULL){
						input[argtok]=malloc(strlen(ptr)+1);
						cp(input[argtok],ptr);
						argtok++;
						ptr=strtok(NULL," 	\n");//split by tab space and enter

						if(input[argtok-1][strlen(input[argtok-1])-1]=='\\')//if input has '\ '
						{
								input[argtok-1][strlen(input[argtok-1])-1]=' ';
								esptemp=malloc(strlen(input[argtok-1])+strlen(ptr)+1);
								esptemp=input[argtok-1];
								strcat(esptemp,ptr);
								input[argtok-1]=malloc(strlen(esptemp)+1);
								input[argtok-1]=esptemp;
								ptr=strtok(NULL," 	\n");
						}

				}
				if(argtok>10)//if input over 10
				{
						printf("input count error:Maximun is 10\n");//input err if over 10
						continue;
				}
				fflush(stdin);//clear input
				char tem[MAX_INPUT];
				if(argtok!=0)
						if(!strcmp(input[0],"ssu_sed"))//if call ssu_sed
						{
								ssu_sed(input,argtok);
						}
						else
						{
								system(CmdInBuf2);//else call sysytem funtion
						} 

				gettimeofday(&et,NULL);
				printf("time : %.3f\n",(et.tv_sec-st.tv_sec)+(et.tv_usec-st.tv_usec)*0.000001);//print run time
		}
		return 0;
}
int mcmp(char * str1,char * str2){//funtion for string copy 
		int i=0;
		while((str1[i]=='\0')|(str2[i]=='\0'))
		{  if(str1[i]-str2[i]==0)
				i++;
				else
						return -1;

		}
		return 0;
}
void opt(char *arg[],int num,int flag[8])//funtion for option.  each option recore their position at flag
{
		for(int i=1;i<mlen(arg[num]);i++)
		{
				switch(arg[num][i])
				{    
						case 'b':
								flag[0]=num+1;
								break;
						case 'u':
								flag[1]=num+1;
								break;
						case 'i':
								flag[2]=num+1;
								break;
						case 'e':
								flag[3]=num+1;
								break;
						case 'd':
								flag[4]=num+1;
								break;
						case 'p':
								flag[5]=num+1;
								break;
						case 's':
								flag[6]=num+1;
								break;
						case 'P':
								flag[7]=num+1;
								break;
						default:
								fputs("usage:ssu_sed [TARGET] [SRC_STR] [DEST_STR] [OPTION]\n",stdout);
								break;
				}
		}

}
int ssu_sed(char *input[],int arg)
{
		depth=0;
		int flag[8]={0};
		if(arg<4){//if input less than 4
				fputs("uasge: ssu_sed [target] [src_str] [dest_str] [option]\n",stdout);
				return 1;
		}

		for(int i=0;i<arg;i++)
		{
				fputs("arg",stdout);
				printf("%d",i);
				fputs(":",stdout);
				fputs(input[i],stdout);
				fputs("\n",stdout);//print input argument
				if(input[i][0]=='-')
				{
						opt(input,i,flag);//find option
				}
		}
		if(flag[7]!=0)//when option -P has put absolute root at input[flag[7]]
		{
				char tm[MAXLINE];
				cp(tm,get_current_dir_name());
				cp(tm+mlen(tm),"/");
				cp(tm+mlen(tm),input[flag[7]]);
				if(input[flag[7]][mlen(input[flag[7]])]=='\n')
						tm[mlen(tm)]='\0';
				cp(input[flag[7]],tm);
		}

		findir(input[1],input,flag);//search directories
		depth=0;
		return 0;
}

int findir(char* name ,char *input[],int flag[]){
		char *rpath=get_current_dir_name();
		struct dirent *dentry=malloc(sizeof(struct dirent));
		struct stat statbuf;
		char filename[MAXNAMLEN+1];
		DIR *dirp=malloc(sizeof(DIR*));

		if(lstat(name,&statbuf)<0)
		{
				puts("lstat err\n") ;

		}else if((statbuf.st_mode&S_IFMT)==S_IFREG)//if now is not directory but regular file
		{
				if(flag[1]!=0){//if option u on
						struct passwd *pw;
						if((pw=getpwnam(input[flag[1]]))==NULL)
						{
								puts("wrong username\n");//known username
								return -1;
						}
						else if((pw->pw_uid)!=(statbuf.st_uid))//compare file uid and input username`s uid
								return chdir(rpath);
				}
				fins(name,input,flag);//if option u is off
				return chdir(rpath);//return before path
		}

		if(((dirp=opendir(name))==NULL)||(chdir(name)==-1)){
				puts("opendir err\n");
				return -1;
		}
		while((dentry=readdir(dirp))!=NULL)
		{

				if(dentry->d_ino==0)
						continue;


				strncpy(filename,dentry->d_name,MAXNAMLEN);
				if(!strcmp(".",filename)|!strcmp("..",filename))//pass . and ..  directories
						continue;


				if(lstat(filename,&statbuf)<0)
				{
						puts("stat err\n");
						continue;
				}

				if(flag[1]!=0){//if option u is on at directories
						struct passwd *pw;
						if((pw=getpwnam(input[flag[1]]))==NULL)
						{
								puts("wrong username\n");
								return -1;
						}
						else if((pw->pw_uid)!=(statbuf.st_uid))
								return chdir(rpath);
				}

				//same as befor
				if((statbuf.st_mode&S_IFMT)==S_IFREG)
				{
						fins(filename,input,flag);
				}
				else 
				{
						depth++;
						if(flag[4]!=0){//if option d is on 
								if(depth-1<atoi(input[flag[4]]))//search directories before input depth 
								{
										findir(filename,input,flag);
										depth--;
								}
						}
						else
								findir(filename,input,flag);//d is off
				}
		}
		return chdir(rpath);
}
int cp(char a[],char b[])//func for copy string 
{

		int i=0;
		while(b[i]!='\0')
		{
				a[i]=b[i];
				i++;
		}
		a[i]='\0';

		return 0;
}
int mlen(char *str){//return length befor NULL 
		int i=0;
		while(str[i++]!='\0')
				;
		return i-1;
}

int fins(char* name ,char *input[],int flag[]) {//find string 

		char buf[MAXLINE],dir[MAXLINE];
		char temn[]="tempfile";
		FILE *fp_a;
		FILE *fp_b;
		char tm[MAXLINE];

		int l=0,i=0,m=0,ie[3]={0};

		cp(dir,get_current_dir_name());    
		cp(dir+mlen(dir),"/");
		cp(dir+mlen(dir),name);///make absolute path. dir has absolute path 

		fp_a=fopen(name,"r");

		if(fp_a==NULL)
		{	fputs("open err at read file",stdout);
				fputs(dir,stdout);
				fputs("\n",stdout);
				exit(1);
		}
		while(fgets(buf,MAXLINE,fp_a)!=NULL)
		{ 

				ie[0]=par(buf,input[2],input[2],flag[0],flag[6]);//when [SRC_STR] found 
				if(ie[0]==1)//when [SRC_STR] found 
				{
						rewind(fp_a);
						break;
				}
		}

		if(ie[0]!=1)//[string] is not exist
		{
		fputs(dir,stdout);//absolute path
		fputs(" : ",stdout);
		fputs("failed\n",stdout);//print success
				return 0;
		
		}else{
				ie[0]=0;
				buf[0]='\0';
		}

		if(flag[2]!=0)//when flag i in 
		{
				while(fgets(buf,MAXLINE,fp_a)!=NULL)
				{
						ie[1]=par(buf,input[flag[2]],input[flag[2]],flag[0],flag[6]);//when i option [string] found
						if(ie[1]==1)//when i option [string] found
						{
								fseek(fp_a,0,SEEK_SET);
								break;
						}  
				}
				if(ie[1]!=1)
				{
						fclose(fp_a);
						return 0;
				}
				else{
						ie[1]=0;
						buf[0]='\0';
				}
		}


		if(flag[3]!=0)//when flag e in
		{
				while(fgets(buf,MAXLINE,fp_a)!=NULL)
				{
						ie[2]=par(buf,input[flag[3]],input[flag[3]],flag[0],flag[6]);//when e option [string] found 
						if(ie[2]==1)//when e option [string] found 
						{
								fclose(fp_a);
								return 0;
						}
				}
		}

		if(flag[7]!=0)//if option P is on 
		{
				cp(tm,input[flag[7]]);
				if(tm[mlen(tm)]=='\n')
						tm[mlen(tm)]='\0';
				cp(tm+mlen(tm),"/");
				cp(tm+mlen(tm),name);//tm has absolute path for new file 
		}
		fp_b=fopen(temn,"w");

		if(fp_b==NULL)
		{
				fputs("fopen err at write file:",stdout);
				fputs(name,stdout);
				fputs("\n",stdout);
				exit(1);
		}
		fclose(fp_a);
		char buf2[MAXLINE];
		fp_a=fopen(name,"r");
		while(fgets(buf2,MAXLINE,fp_a)!=NULL)
		{
				++i;
				l=par(buf2,input[2],input[3],flag[0],flag[6]);
				if((l==1)&(flag[5]!=0))//if opt p is on
				{
						m++;
						fputs(dir,stdout);
						fputs(" : ",stdout);
						printf("%d",i);
						fputs("\n",stdout);
				}
				fputs(buf2,fp_b);
		}
		fputs(dir,stdout);//absolute path
		fputs(" : ",stdout);
		fputs("success\n",stdout);//print success

		if(flag[7]!=0)//if opt P is on move file to input path 
		{
				rename(temn,tm);
		}
		else{
				remove(name);
				rename(temn,name);
		}
		fclose(fp_a);
		fclose(fp_b);//close file pointer

		return 0;
}
char cap(char a,int f){//reture capital ascii number
		if(f!=0){
				if((96<a)&(a<123))
						return a-32;
				else
						return a;
		}
		else if(f==0)
				return a;

}
int par(char *tar,char*src,char *dest,int b,int s){//tar is string that fp read. src and dest are bofore and after string to change
		int a=0,j=0,t=0;//b is positive opt b is on and also same as s

		if((b==0))
				for(int i=0;i<mlen(tar);i++)
				{
						if(cap(src[0],s)==cap(tar[i],s))
						{
								a=i;
								for(j=0;j<mlen(src);j++)
								{
										if(cap(src[j],s)==cap(tar[a],s))
										{
												a++;
										}
										else if(cap(src[j],s)!=cap(tar[a],s))
										{
												a=-1;
												break;
										}
								}
								if(a!=-1){//when src string find cutting src pasting dest and make full sentence 
										t=1;
										char temp[MAXLINE];
										cp(temp,tar);
										cp(temp+i,dest);
										cp(temp+i+mlen(dest),tar+i+mlen(src));
										cp(tar,temp);
										i+=(mlen(dest)-1);
										a=-1;
								}
						}
				}else if(b!=0){//when option b is on 
						for(int i=0;i<mlen(tar);i++)
						{
								if(tar[i]==' ')//ignore space 
										continue;
								if(cap(src[0],s)==cap(tar[i],s))
								{
										a=i;
										for(j=0;j<mlen(src);j++)
										{
												if(tar[a]==' ')
												{
														a++;
														j--;
														continue;
												}if(cap(src[j],s)==cap(tar[a],s))
												{
														a++;
												}
												else if(cap(src[j],s)!=cap(tar[a],s))
												{
														a=-1;
														break;
												}
										}
										if(a!=-1){//similler code 
												t=1;
												char temp[MAXLINE];
												cp(temp,tar);
												cp(temp+i,dest);
												cp(temp+mlen(temp),tar+a);
												cp(tar,temp);
												i+=mlen(dest)-1;
												a=-1;
										}
								}
						}
				}

		return t;
}
