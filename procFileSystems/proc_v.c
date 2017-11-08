#include "proc_header.h"

int proc_v(){
    FILE *fp;
    char buf[1024];
    if((fp=fopen("/proc/version","r"))<0)
    {
	fprintf(stderr,"fopen err at option l\n");
	return -1;
    }
    printf("([/proc/version])\n");//옵션 이름 출력

    while(fgets(buf,1024,fp)!=NULL)//버전 읽어온뒤 
    printf("%s",buf);//출력 
    
    fclose(fp);
    return 0;
}
