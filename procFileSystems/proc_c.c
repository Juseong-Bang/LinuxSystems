#include"proc_header.h"
int proc_c(int num, char *pids[]){
    char buf[1024]="";
    char tempath[1024]="";
    int fd;

    if(num==0)//pid 갯수가 0 부모 프로세스에 대한 동작 
    {
	num=1;
	pids[0]=(char*)malloc(sizeof(rootpid));
	strcpy(pids[0],rootpid);
    }

    for(int t=0;t<num;t++){

	if(num>1)//만약 pid가 복수라면
	{
	    printf("([/proc/%s/cmdline])\n",pids[t]);
	}

	strcpy(tempath,"/proc/");
	strcat(tempath,pids[t]);
	strcat(tempath,"/cmdline");

	if(know_access(tempath)<0)//권한 검사
	    continue;

	if((fd=open(tempath,O_RDONLY))<0){//읽기 오류가 안생긴다면 
	    fprintf(stderr,"fopen err\n");
	    exit(1);
	}else{

	    long rbyte=read(fd,buf,sizeof(buf));//읽은 바이트수 리턴
	    int s=0,k=0;
	    for(int n=0;n<rbyte;n++)
	    {
		if(buf[n]=='\0'){
		    printf("argv[%d]%s\n",k++,buf+s);//읽은 cmdline의 정보를 출력
		    s=n+1;
		}
	    }

	}
    }
    close(fd);//닫아줌
    return 0;
}
