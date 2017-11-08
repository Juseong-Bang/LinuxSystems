#include "proc_header.h"

int proc_l(){
    FILE *fp;
    char wt[20],it[20];
 
    if((fp=fopen("/proc/uptime","r"))<0)
    {
	fprintf(stderr,"fopen err at option l\n");
	return -1;
    }
    printf("([/proc/uptime])\n");

    fscanf(fp,"%s%s",wt,it);//한줄을 읽음 
    printf("Process worked time:%s(sec)\n",wt);//각각을 출력
    printf("Process idle time:%s(sec)\n",it);
    fclose(fp);
    return 0;
}
