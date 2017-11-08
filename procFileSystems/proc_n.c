#include"proc_header.h"


int proc_n(int num, char *pids[]){

    FILE *fp;
    char buf[1024]="";
    char tag[10],out[50];
    char tempath[1024]="";


    if(num==0)//부모 프로세스에 대한 동작
    {
	num=1;
	pids[0]=(char*)malloc(sizeof(rootpid));
	strcpy(pids[0],rootpid);
    }

    for(int t=0;t<num;t++){

	if(num>1)//만약 pid가 복수라면
	{
	    printf("([/proc/%s/io])\n",pids[t]);
	}

	strcpy(tempath,"/proc/");
	strcat(tempath,pids[t]);
	strcat(tempath,"/io");

	if(know_access(tempath)<0)//권한 검사
	    continue;

	if((fp=fopen(tempath,"r"))<0){
	    fprintf(stderr,"fopen err\n");
	    exit(1);
	}else{
	    int n=0;
	    while((fgets(buf,1024,fp)!=NULL)){//한줄씩 읽어옴 

		sscanf(buf,"%s%s",tag,out);//태그와 값을 받아온뒤

		switch(n)//읽은 라인에 따른 정보 출력 
		{
		    case 0:
			printf("Characters read: %s\n",out);
			break;
		    case 1:
			printf("Characters written: %s\n",out);
			break;
		    case 2:
			printf("Read syscalls: %s\n",out);
			break;
		    case 3:
			printf("Write syscalls: %s\n",out);
			break;
			break;
		    case 4:
			printf("Bytes read: %s\n",out);
			break;
		    case 5:
			printf("Bytes written: %s\n",out);
			break;
		    case 6:
			printf("Cancelled write bytes: %s\n",out);
			break;
		    default:
			return -1;
		}
		n++;
	    }
	}
    }
    fclose(fp);//닫아줌
    return 0;
}
