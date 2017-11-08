#include"proc_header.h"
int compare(const char *,const char  *);
int proc_r(){
    int f=1;

    for(int p=0;p<6;p++){
	if(optmask(f)==f)//인자가 오는 옵션들 f,t,c,n,m,k가 사용 되었는지 확인 
	{
	    for (int i = pidsetnum[p]; i>1; i--)//정렬할 범위를 축소해 나가면서
	    {
		for (int j = 1; j<i; j++)
		{
		    if (compare(pidset[p][j-1],pidset[p][j])>0)//앞쪽 원소가 더 크면 바꿈
		    {
			
			char temp[25]="";
			strcpy(temp,pidset[p][j-1]);
			
			pidset[p][j-1]=(char*)malloc(sizeof(pidset[p][j]));
			strcpy(pidset[p][j-1],pidset[p][j]);

			pidset[p][j]=(char*)malloc(sizeof(temp));
			strcpy(pidset[p][j],temp);


		    }
		}
	    }
	}
	f=f*2;
    }
    return 0;
}

int compare(const char *a , const char *b){
    if((atoi(a)!=0)||(atoi(b)!=0))//만약 (pid는 0이 아니므로 ) 숫자라면 
	return atoi(a)-atoi(b);//바꿔준뒤 뺴서 리턴 
    else
	return strcmp(a,b);//문자열이라면 비교해서 리턴 

}
