#include"proc_header.h"


int proc_opt(int argc,char *argv[]){

    for(int i=1;i<argc;i++)
    {
	if(argv[i][0]=='-')
	{
	    switch(argv[i][1])
	    {
		case 'f':
		    flag|=optf;//f 옵션을 사용했다는 플래그 설정 
		    pidsetnum[0]=many_get(argv+i);//f 뒤에 몇개의 인자가 오는지 검사 
		    if(put_arr('f' ,pidsetnum[0],argv+i+1)<0){//인자들을 포인터에 넣어주는 함수 
			fprintf(stderr,"wrong use option -f \n");
			return -1;
		    }
		    i+=pidsetnum[0];//읽은 인자의 수만큼 넘어가서 계속 탐색 
		    //filedes 
		    break;
		case 't':
		    flag|=optt;
		    pidsetnum[1]=many_get(argv+i);
		    if(put_arr('t' ,pidsetnum[1],argv+i+1)<0){
			fprintf(stderr,"wrong use option -t \n");
			return -1;
		    }
		    i+=pidsetnum[1];
		    //status
		    break;

		case 'c':
		    flag|=optc;
		    pidsetnum[2]=many_get(argv+i);
		    if(put_arr('c' ,pidsetnum[2],argv+i+1)<0){
			fprintf(stderr,"wrong use option -c \n");
			return -1;
		    }
		    i+=pidsetnum[2];
		    //cmdline
		    break;

		case 'n':
		    flag|=optn;
		    pidsetnum[3]=many_get(argv+i);
		    if(put_arr('n' ,pidsetnum[3],argv+i+1)<0){
			fprintf(stderr,"wrong use option -n \n");
			return -1;
		    }
		    i+=pidsetnum[3];
		    //io
		    break;

		case 'm':
		    flag|=optm;
		    pidsetnum[4]=many_get(argv+i);
		    if(put_arr('m' ,pidsetnum[4],argv+i+1)<0){
			fprintf(stderr,"wrong use option -m \n");
			return -1;
		    }
		    i+=pidsetnum[4];
		    //environ
		    break;

		case 'k':
		    flag|=optk;
		    pidsetnum[5]=many_get(argv+i);
		    if(optmask(optm)!=optm){//만약 k옵션이 m옵션이 먼저 오지 않았는데 온다면 에러
			fprintf(stderr,"-k option is must be after -m \n");
			return -1;
		    }
		    if(put_arr('k' ,pidsetnum[5],argv+i+1)<0){
			fprintf(stderr,"wrong use option -k \n");
			return -1;
		    }
		    i+=pidsetnum[5];
		    //if after -m
		    break;
		case 'r':
		    flag|=optr;
		    // aline by ascending
		    break;
		case 'w':
		    flag|=optw;
		    // proc/interrupts
		    break;

		case 'e':
		    flag|=opte;
		    // proc/filesystems
		    break;

		case 'l':
		    flag|=optl;
		    // proc/uptime
		    break;

		case 'v':
		    flag|=optv;
		    // proc/version
		    break;

		case 's':
		    flag|=opts;
		    attrnum=many_get(argv+i);
		    if(put_arr('s' ,attrnum,argv+i+1)<0){
			fprintf(stderr,"wrong use option -s \n");
			return -1;
		    }
		    i+=attrnum;
		    //option -s
		    break;

		case 'o':
		    flag|=opto;
		    if((argv[i+1]==NULL)||(argv[i+1][0]=='-'))//만약 o 옵션 뒤에 아무것도 없거나 다른 옵션이 온다면 에러
		    {
			fprintf(stderr,"filename is not exist.\n");
			return -1;
		    }else{
			strcpy(filepath,argv[i+1]);
		    }
		    // file_name
		    break;

		default:
		    fprintf(stderr,"unknown option\n");//지정된 옵션 이외에 다른 옵션이 들어온다면 에러 
		    //known option
		    break;
	    }
	}
    }

    return 0;
}
int many_get(char *input[]){
    int count=0;
    if(input[count+1]==NULL)//만약 마지막 인자였다면
	return 0;

    while((input[++count]!=NULL)&&(input[count][0]!='-'))//받은 인자에서부터 다음 옵션이나 마지막 인자까지 루프
	;

    return count-1;//해당 옵션 뒤에 온 인자의 개수 출력 

}

int put_arr(char opt ,int number,char *args[]){//16개의 인자가 오는

    int option=0;

    if(opt=='f'){
	option=0;
    }else if(opt=='t'){
	option=1;
    }else if(opt=='c'){
	option=2;
    }else if(opt=='n'){
	option=3;
    }else if(opt=='m'){
	option=4;
    }else if(opt=='k'){
	option=5;
    }else if(opt=='s'){//s 옵션에서 불럿다면 

	for(int n=0;n<number;n++){
	    if((number>5)||(number==0))//인자가 1~5개 와야함
	    {
		printf("At least 1st to 5th arguments: FILEDES,CMDLINE,IO,STAT,ENVIRON\n");
		return -1;
	    }
	    else
	    {
		if(strcmp(args[n],"FILEDES")==0)
		{
		    attflag|=atrf;//각각을 사용했다는 플래그
		}else
		if(strcmp(args[n],"CMDLINE")==0)
		{
		    attflag|=atrc;
		}else
		if(strcmp(args[n],"IO")==0)
		{
		    attflag|=atrn;
		}else
		if(strcmp(args[n],"STAT")==0)
		{
		    attflag|=atrt;
		}else
		if(strcmp(args[n],"ENVIRON")==0)
		{
		    attflag|=atrm;
		}else
		{
		return -1;
		}
	    }
	}
	return 0;
    }else{
	return -1;
    }
//s이외에 정상적인 옵션이 온다면 
    for(int n=0;n<number;n++){
	if(n>15)//만약 옵션 뒤의 pid 또는 key가 0~15개가 넘는다면 
	{
	    printf("Maximum Number of Argument Exceeded. :: %s\n",args[n]);

	}
	else//아니라면 동적할당 
	{
	    pidset[option][n]=(char*)malloc(sizeof(args[n]));
	    strcpy(pidset[option][n],args[n]);
	}
    }

    if(number>16)//만약 16개가 넘는 인자가 온다면 
	pidsetnum[option]=16;//16개만 저장했으므로 갯수도 입력

    return 0;
}
