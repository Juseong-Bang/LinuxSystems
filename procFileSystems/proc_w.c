#include "proc_header.h"

int proc_w(){
    FILE *fp;
    char buf[1024];
    char te[10];
    float sum=0;
    int cpunum=0,j=0,noff=0, offset=0;
    char tm[20];

    if((fp=fopen("/proc/interrupts","r"))<0)
    {
	fprintf(stderr,"fopen err at option e\n");
	return -1;
    }
    printf("([/proc/interrupts])\n");//무슨 옵션의 기능인지 출력 
    if(fgets(buf,1024,fp)!=NULL)
    {
	while(sscanf(buf+j,"%s%n",te,&offset)==1)//첫번째 줄에서 한 문자열씩 읽어옴 (cpu의 갯수를 세기 위해)
	{
	    j+=offset;//받은 offset으로 다음 문자열 읽음
	    cpunum++;//cpu갯수를 기록해줌 
	}
	printf("---Number of CPUs:%d---\n",cpunum);
	printf("\t[Average] : [Description]\n");
    }
    if(cpunum==0){//cpu 갯수가 0이면 에러
	fprintf(stderr,"number of cpu is 0\n");
	return -1;
    }

    while(fgets(buf,1024,fp)!=NULL){//한줄씩 읽음 
	offset=0;
	sum=0;
	sscanf(buf," %[^:]%n",te,&offset);// : 전까지 읽음 
	if((strcmp(te,"0")==0)||(atoi(te)!=0))//태그가 숫자일경우 무시
	    continue;
	for(int j=0;j<cpunum;j++){//cpu 갯수만큼 뒤의 숫자를 읽으려고 시도 
	    sscanf(buf+offset,"%[^0-9]%n",tm,&noff);//숫자 전까지 공백 무시 
	    offset+=noff;
	    if(sscanf(buf+offset,"%[0-9]%n",tm,&noff)!=1)//숫자 읽음,만약 읽을것 없는 경우 continue
		continue;
	    offset+=noff;//offset 기록 
	    sum+=atoi(tm);//숫자로 바꿔줌 
	    sscanf(buf+offset,"%[^0-9]%n",tm,&noff);//다음의 공백 무시 
	}

	printf("%15.3f:<%s>",sum/cpunum,te);//15필드 만큼에 소숫점 3자리 출력 ,태그 출력
	if(printf("%s",buf+offset)==0)//설명 출력
	    printf("\n");//만약 출력 없다면 개행
	bzero(buf,sizeof(buf));//버퍼 초기화
    }
    fclose(fp);
    return 0;
}
