#include "proc_header.h"

int proc_e(){
    FILE *fp;
    char buf[1024];
    char fs[5][20];
    int i=0,j=0;
    
    if((fp=fopen("/proc/filesystems","r"))<0)
    {
	fprintf(stderr,"fopen err at option e\n");
	return -1;
    }
    printf("([/proc/filesystems])\n");//옵션명 출력

    printf("<<Supported Filesystems>>\n");


    while(fgets(buf,1024,fp)!=NULL)//한줄씩 buf에 읽어옴
    {
	j=0;

	buf[strlen(buf)-1]='\0';//버퍼의 마지막에 널 붙여줌
	while(buf[j]!='\t')//탭까지 넘김 
	    j++;


	strcpy(fs[i++],buf+j+1);//탭 다음 위치(사용하는 파일시스템의 이름 저장)

	if(i==5){//5개 까지 저장했다면 
		printf("%s,%s,%s,%s,%s\n",fs[0],fs[1],fs[2],fs[3],fs[4]);//츨력 
	    i=0;
	}
    }

    if(i>0){//만약 마지막에 5개까지 채우지 못하고 반복문을 나왓다면 
	for(int t=0;t<i-1;t++)
	    printf("%s,",fs[t]);//남은 개수만큼 출력 
	printf("%s\n",fs[i-1]);//마지막 인자는 콤마가 없어야 하므로 따로 출력
    }
 fclose(fp);
    return 0;
}
