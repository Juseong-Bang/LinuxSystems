#include"proc_header.h"

int main(int argc,char *argv[]){

    int o_serr,o_sout,fd;
    pid_t pid;
    int stat;


    rootpid=(char*)malloc(sizeof(char)*10);//main 함수 즉, 부모 프로세스의 pid저장 
    sprintf(rootpid,"%d",getpid());//pid를 문자열로 저장
    rootuid=getuid();//부모 프로세스의 uid 저장

    if(proc_opt(argc,argv)<0){//옵션 검사하는 함수
	fprintf(stderr,"proc_opt err\n");
	exit(1);
    }
    printf(">: ssu_lsproc start. :<\n");

    if(optmask(opto)==opto){//만약 O옵션을 사용했다면 
	o_sout=dup(1);
	o_serr=dup(2);//stdout stderr에 연관된 파일 디스크립터를 저장
	close(1);
	close(2);//두개를 닫고 
	if((fd=creat(filepath,0644))<0)
	{
	    fprintf(stderr,"Cannot make file:%s\n",filepath);//파일을 만들지 못하는 경우 에러 출력
	return -1;
	}
	fd=dup(1); 
	fd=dup(2);//o옵션에서 받은 경로에 1,2번 파일 디스크립터를 연결
    }

    if(optmask(optr)==optr){//만약 r옵션을 사용했다면 
	if (proc_r()<0){
	    fprintf(stderr,"option r err\n");
	    exit(1);
	}}

    if(optmask(optf)==optf){//만약 f옵션을 사용했다면
	if((pid=fork())<0){//자식 프로세스 호출
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){//자식프로세스의 경우
	    if(proc_f(pidsetnum[0],pidset[0])<0)
	    {
		fprintf(stderr,"option r err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(wait(&stat)!= pid)//자식 프로세스를 기다림
	{
	    fprintf(stderr,"wait err\n");
	    exit(1);
	}}

    if(optmask(optt)==optt){//만약 t옵션을 사용했다면
	if((pid=fork())<0){//자식 프로세스 호출
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){//자식프로세스의 경우
	    if(proc_t(pidsetnum[1],pidset[1])<0)
	    {
		fprintf(stderr,"option t err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}

    if(optmask(optc)==optc){//만약 c옵션을 사용했다면
	if((pid=fork())<0){
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){
	    if(proc_c(pidsetnum[2],pidset[2])<0)
	    {
		fprintf(stderr,"option c err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}

    if(optmask(optn)==optn){//만약 n옵션을 사용했다면
	if((pid=fork())<0){
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){
	    if(proc_n(pidsetnum[3],pidset[3])<0)
	    {
		fprintf(stderr,"option n err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}

    if(optmask(optm)==optm){//만약 m옵션을 사용했다면
	if((pid=fork())<0){
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){
	    if(proc_m(pidsetnum[4],pidset[4])<0)
	    {
		fprintf(stderr,"option m err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}

    if(optmask(optw)==optw){//만약 w옵션을 사용했다면
	if((pid=fork())<0){
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){
	    if(proc_w()<0)
	    {
		fprintf(stderr,"option w err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}
    if(optmask(opte)==opte){//만약 e옵션을 사용했다면
	if((pid=fork())<0){
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){
	    if(proc_e()<0)
	    {
		fprintf(stderr,"option e err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}
    if(optmask(optl)==optl){//만약 l옵션을 사용했다면
	if((pid=fork())<0){
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){
	    if(proc_l()<0)
	    {
		fprintf(stderr,"option l err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}

    if(optmask(optv)==optv){//만약 v옵션을 사용했다면
	if((pid=fork())<0){
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){
	    if(proc_v()<0)
	    {
		fprintf(stderr,"option v err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}

    if(optmask(opts)==opts){//만약 s옵션을 사용했다면
	if((pid=fork())<0){
	    fprintf(stderr,"fork err\n");
	    exit(1);
	}else if(pid==0){
	    if(proc_s()<0)
	    {
		fprintf(stderr,"option s err\n");
		exit(1);
	    }
	    exit(0);
	}
	if(waitpid(pid,NULL,0)<0)
	{
	    fprintf(stderr,"waitpid err\n");
	    exit(1);
	}}

    if(optmask(opto)==opto){//o옵션을 사용한 경우 마지막 완료 문장을 위해서 
	close(fd);//원래 사용하던 파일 디스크립터를 닫아준뒤
	dup2(o_sout,1);
	dup2(o_serr,2);//원래의 복사해둔 stdout,stderr에 다시 연결

    }
    printf(">: ssu_lsproc terminated. :<\n");
    return 0;
}

int know_access(char * path){//접근 권한을 검사하는 함수 
    if(access(path,F_OK)<0)//존재하지 않거나 
    {
	printf("%s dosen`t exist.\n",path);
	return -1;
    } if(access(path,R_OK)<0){//읽지 못하는경우 
	printf("%s can`t be read.\n",path);
	return -1;
    }
    return 0;
}
