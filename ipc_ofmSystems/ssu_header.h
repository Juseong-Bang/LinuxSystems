#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<stdbool.h>
#include<sys/stat.h>
#include<time.h>
#include<sys/ucontext.h>
#include<signal.h>
#include<string.h>
#include<dirent.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<pwd.h>
#include<grp.h>

#define FIFO_FILE "/tmp/fifo"//fifo file name 
#define BUFF_SIZE 1024

int rMode();//read mode 
int wMode();//write mode 
int check();//check log of share file 
pid_t getOfmPid();//return ofm pid 
int checkOpt(int argc,char *argv[]);//check option

void pushQue(pid_t);//input requested pid 
pid_t popQue();//input requested pid 

void pushpidQue(char *);//input requested pid 
char * poppidQue();//input requested pid 

void init();//init demon 
int  checkLock();//check lock 

struct msgbuf{
  long msg_type;
  pid_t msg_pid;

};
//struct of data has ready pid of process 
struct pidbuf{
  long msg_type;
  char msg_line[1024];
};
//struct of data has logdate opt l used
