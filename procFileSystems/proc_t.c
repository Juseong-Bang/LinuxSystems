#include"proc_header.h"


int proc_t(int num, char *pids[]){

    FILE *fp;
    char buf[1024]="";
    char tag[10],out[50];
    char tempath[1024]="";


    if(num==0)//부모 프로세스에 대한 옵션 
    {
	num=1;
	pids[0]=(char*)malloc(sizeof(rootpid));
	strcpy(pids[0],rootpid);
    }

    for(int t=0;t<num;t++){

	if(num>1)//만약 pid가 복수라면
	{
	    printf("([/proc/%s/status])\n",pids[t]);
	}

	strcpy(tempath,"/proc/");
	strcat(tempath,pids[t]);
	strcat(tempath,"/status");

	if(know_access(tempath)<0)// /proc/[pid]/status에 대한 권한 검사
	    continue;

	if((fp=fopen(tempath,"r"))<0){
	    fprintf(stderr,"fopen err\n");
	    exit(1);
	}else{
	    while((fgets(buf,1024,fp)!=NULL)){

		sscanf(buf,"%s%s",tag,out);//각각의 태그와 값을 읽어옴 
		if(strcmp(tag,"FDSize:")==0)//만약 uid다음 값을 읽었다면 중지
		    break;

		if(strcmp(tag,"State:")==0){//만약 읽어온 값이 state라면 
		    switch(out[0])//각각의 state의 정보를 구분
		    {
			case 'R':
			    strcpy(out,"Running");
			    break;
			case 'S':
			    strcpy(out,"Sleeping in an interruptible wait");
			    break;
			case 'D':
			    strcpy(out,"Waiting in uninterruptible disk sleep");
			    break;
			case 'T':
			    if(out[3]=='s')
				strcpy(out,"Stopped (on a signal)");
			    else
				strcpy(out,"Stopped trace stopped");
			    break;
			case 'Z':
			    strcpy(out,"Zombie");
			    break;
			case 'X':
			    strcpy(out,"Dead");
			    break;
			default:
			    return -1;
		    }
		}
		printf("%s	%s\n",tag,out);//읽은 태그와 값을 출력
	    }}}
fclose(fp);
return 0;
}
