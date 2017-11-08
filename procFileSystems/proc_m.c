#include"proc_header.h"
int proc_k(char *env);
int proc_m(int num, char *pids[]){
    char buf[4096]="";
    char tempath[1024]="";
    int fd;

    if(num==0)//부모 프로세스에 대한 동작
    {
	num=1;
	pids[0]=(char*)malloc(sizeof(rootpid));
	strcpy(pids[0],rootpid);
    }

    for(int t=0;t<num;t++){

	if(num>1)//만약 pid가 복수라면
	{
	    printf("([/proc/%s/environ])\n",pids[t]);
	}

	strcpy(tempath,"/proc/");
	strcat(tempath,pids[t]);
	strcat(tempath,"/environ");

	if(know_access(tempath)<0)//권한 검사
	    continue;

	if((fd=open(tempath,O_RDONLY))<0){
	    fprintf(stderr,"fopen err\n");
	    exit(1);
	}else{//만약 파일이 접근,읽기 가능하다면 
	    long rbyte=read(fd,buf,sizeof(buf));//전체를 buf에 한꺼번에 읽어옴 
	    long s=0,k=0;
	    char tempenv[2048];

	    char *print[pidsetnum[5]];//r옵션의 key 갯수만큼 

	    for(int n=0;n<rbyte;n++)//읽은 바이트 만큼 
	    {
		if(buf[n]=='\0'){//만약 널(구분자가 널임)이라면 
		    if((optmask(optk)==optk)&&(pidsetnum[5]>0))//만약 k옵션에서 인자가 1개 이상이라면 
		    {
			sscanf(buf+s,"%[^=]",tempenv);//=전까지의 값 즉, 태그를 읽어옴 
			if((k=proc_k(tempenv))!=-1)//k옵션에서 사용했는지 판단
			{
			    print[k]=(char*)malloc(sizeof(buf+s));//했다면 
			    strcpy(print[k],buf+s);//값을 저장 
			}
			s=n+1;//개행의 다음주소값 저장
		    }
		    else{//k를 사용하지 않았으면 
			printf("%s\n",buf+s);// 그냥 출력 
			s=n+1;//개행의 다음주소값 저장
		    }
		}
	    }

	    if((optmask(optk)==optk)&&(pidsetnum[5]>0))//만약 k옵션을 사용했고 인자가 1개 이상이라면 
		for(int t=0;t<pidsetnum[5];t++)
		    printf("%s\n",print[t]);//인자의 순서대로 출력됨
	}
    }

close(fd);//닫아줌
    return 0;
}

int proc_k(char *env){

    for(int t=0;t<pidsetnum[5];t++)
    {
	if(strcmp(env,pidset[5][t])==0)	//만약 k옵션의 인자에 있다면 
	    return t;//key 인자의 순서를 리턴 
    }
    return -1;//없다면 -1

}
