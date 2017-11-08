#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>
#include<dirent.h>

#define optf 1
#define optt 2
#define optc 4
#define optn 8
#define optm 16
#define optk 32
#define optw 64
#define opte 128
#define optl 256
#define optv 512
#define optr 1024
#define opts 2048
#define opto 4096
//각각의 옵션의 사용 여부를 판단하기 위한 마스킹용 플래그

#define atrf 1
#define atrc 2
#define atrt 4
#define atrn 8
#define atrm 16
//s 옵션에서 인자의 유무를 기록해두는 플래그

#define optmask(x) (flag&x)// 옵션의 플래그를 검사해주는 매크로
#define atrmask(x) (attflag&x)//s옵션의 인자를 검사해주는 매크로

char* pidset[6][16];//5개 옵션에서 사용되는 각각의 pid집합과 key가 들어있는 포인터 
int pidsetnum[6];//각각 옵션의 인자의 갯수 
int flag;//옵션의 플래그 
char filepath[1024];//o옵션의 파일 경로 저장

int proc_opt(int argc,char *argv[]);//옵션 검사 함수
int many_get(char *args[]);//옵션의 인자의 갯수를 세주는 함수
int put_arr(char opt ,int many,char *args[]);//옵션에서 사용되는 인자를 각각의 포인터에 동적할당 해주는 함수

char *rootpid;//부모 프로세스의 pid저장
uid_t rootuid;//부모 프로세스의 uid저장

int attrnum;//s옵션의 인자 갯수
int attflag;//s옵션의 인자 종류 여부

int know_access(char * path);//접근 읽기 권한 검사 

int proc_f(int num,char *pids[]);
int proc_t(int num,char *pids[]);
int proc_c(int num,char *pids[]);
int proc_n(int num,char *pids[]);
int proc_m(int num,char *pids[]);


int proc_w();
int proc_e();
int proc_l();
int proc_v();

int proc_r();
int proc_s();
