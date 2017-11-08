#include"proc_header.h"

int has_same_uid(char * path);

int proc_s(){

    DIR *dp=malloc(sizeof(DIR*));
    struct dirent *dirp=malloc(sizeof(struct dirent));
    char namebuf[1024]="";
    char *pidbuf[1][256];
    int samepid=0;

    if(attrnum==0)//만약 -s옵션에서 들어온 attribute가 없다면 
	return -1;
    if(optmask(optf)==optf){//만약 f옵션을 사용했다면 
	flag&=(~optf);//다시 꺼줌 
    }


    if((dp=opendir("/proc"))==NULL)//proc에 대해서 접근 권한 검사 
	return -1;

    while((dirp=readdir(dp))!=NULL)//하위 디렉토리 탐색 
    {
	if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0)||(atoi(dirp->d_name)==0))
	    continue;//하위 디렉토리중 . .. 과 pid(숫자)가 아닌것은 건너 띔

	strcpy(namebuf,"/proc/");
	strcat(namebuf,dirp->d_name);
	strcat(namebuf,"/status");//namebuf에는 /proc/[pid]/status 경로가 있음

	if(know_access(namebuf)<0)//읽지 못하는 경로
	    continue;

	if(has_same_uid(namebuf)==0){//같은 uid를 가진 pid를 저장
	    pidbuf[0][samepid]=(char*)malloc(strlen(dirp->d_name)+1);
	    strcpy(pidbuf[0][samepid++],dirp->d_name);
	}
    }

    for(int n=0;n<samepid;n++){//같은 uid를 가진 프로세스의 갯수만큼 


	if(atrmask(atrf)==atrf){//각각의 s옵션 인자를 사용했는지 검사

	    printf("##Attribute:FILEDES, Target ID:%s ##\n",pidbuf[0][n]);
	    proc_f(1,pidbuf[0]+n);//사용 했다면 호출 
	}
	if(atrmask(atrc)==atrc){
	    printf("##Attribute:CMDLINE, Target ID:%s ##\n",pidbuf[0][n]);
	    proc_c(1,pidbuf[0]+n);
	}
	if(atrmask(atrn)==atrn){
	    printf("##Attribute:IO, Target ID:%s ##\n",pidbuf[0][n]);
	    proc_n(1,pidbuf[0]+n);
	}
	if(atrmask(atrt)==atrt){
	    printf("##Attribute:STAT, Target ID:%s ##\n",pidbuf[0][n]);
	    proc_t(1,pidbuf[0]+n);
	}
	if(atrmask(atrm)==atrm){
	    printf("##Attribute:ENVIRON, Target ID:%s ##\n",pidbuf[0][n]);
	    proc_m(1,pidbuf[0]+n);
	}
    }
    closedir(dp);
    return 0;
}

int has_same_uid(char * path){
    FILE* fp;
    char tag[10];
    char out[10];
    char buf[100];
    if(know_access(path)<0)//  /proc/[pid]/status 경로에 대한 접근권한 검사
    {
	return -1;
    }
    else{
	fp=fopen(path,"r");
	while((fgets(buf,1024,fp)!=NULL)){//한줄씩 buf에 저장
	    sscanf(buf,"%s%s",tag,out);//태그와 내용으로 나눠서 자름 
	    if(strcmp(tag,"Uid:")==0)//만약 태그가 uid라면 
		break;
	}
	if((uid_t)atoi(out)==rootuid)//부모 프로세스의 uid와 같은지 비교
	{
	    fclose(fp);
	    return 0;//같으면 0
	}else
	    fclose(fp);
	return -1;//다으면 -1
    }
    return -1;
    }
