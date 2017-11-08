#include"proc_header.h"


int proc_f(int num, char *pids[]){

    DIR *dp=malloc(sizeof(DIR*));
    struct dirent *dirp=malloc(sizeof(struct dirent));
    char buf[500]="";
    char namebuf[1024]="";
    char tempath[1024]="";


    if(num==0)//받은 인자가 0개 = 부모 프로세스에 대한 옵션 실행
    {
	num=1;
	pids[0]=(char*)malloc(sizeof(rootpid));//부모 프로세스의 pid를 넣어준다
	strcpy(pids[0],rootpid);
    }
    for(int t=0;t<num;t++){

	if(num>1)//만약 pid가 복수라면
	{
	    printf("([/proc/%s/fd])\n",pids[t]);//각각의 경로 출력
	}

	strcpy(tempath,"/proc/");
	strcat(tempath,pids[t]);
	strcat(tempath,"/fd");

	if(know_access(tempath)<0)//권한 검사
	    continue;

	if((dp=opendir(tempath))==NULL)
	{
	    printf("opendir err\n");
	}

	while((dirp=readdir(dp))!=NULL)
	{
	    if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0))
		continue;//.과 .. 경로에 대한 정보는 건너띔
	    strcpy(namebuf,tempath);
	    strcat(namebuf,"/");
	    strcat(namebuf,dirp->d_name);//namebuf에는 /proc/[pid]/fd/[filedes]가 들어있음 
	    readlink(namebuf,buf,1024);
	    printf("File diecript number:%s, Opened file:%s\n",dirp->d_name,buf);
	}

    }
closedir(dp);//닫아줌
return 0;
}
