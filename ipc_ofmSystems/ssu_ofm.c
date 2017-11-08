#include"ssu_header.h"
//ssu_ofm.c//

FILE *logfile; //file pointer of logfile
int fifo=0; //filedes of fifo 
char logpath[1024]; //full path of log
char shareFile[1024]; //fullpath of sharefile 
char curpath[1024]={0};//path of logs
char optlbuf[1024]={0};//buf for opt -l 
bool opt[5] = { false }; //l,t,n,p,id
int queSize=16; //init que size
int now=0;//now used que number 
int que=0;//filedes of que 
int pidque=0;//opt l que
key_t key=0;//key of que
key_t pidkey=0;//key of opt l que


void writelog(char *log,char *msg){//write msg to log 

  FILE *templog;
  templog=fopen(log,"a+");//open log file 
  fputs(msg,templog);//write log file 
  fclose(templog);//close 

}
static void sig_term(int signo){
  unlink(FIFO_FILE);//unlink fifo file 

  if(msgctl(que,IPC_RMID,NULL)==-1){//que remove 
    fprintf(stderr,"remove msgque err\n");
    exit(1);
  }  
  if(msgctl(pidque,IPC_RMID,NULL)==-1){//opt l que remove 
    fprintf(stderr,"remove msgque err\n");
    exit(1);
  }  
  exit(0);
}

static void sig_usr1(int signo,siginfo_t *sf,ucontext_t *uc){//signal 1 receive 
  char buf[1024] = { 0 };
  char linebuf[1024]={0};
  bzero(linebuf,1024);
  if(opt[0]){//if opt l
    strcat(linebuf,buf);  
  }
  if(read(fifo,buf,1024)<0){
    fprintf(stderr,"read err\n");
    exit(1);
  }

  if(strcmp(buf,shareFile)==0){//requested file and sharefile is same 

    if(opt[1]){//opt t
      time_t now;
      time(&now);
      strftime(buf,1024,"[%F %T]",localtime(&now));
      writelog(logpath,buf);//write buf to logfile signal received time 
      if(opt[0]){//if opt l 
	strcat(linebuf,buf);//add opt l buf to buf    
      }
    }

    sprintf(buf,"Requested process id: %d ,",sf->si_pid);
    writelog(logpath,buf);//write log 
    if(opt[0]){
      strcat(linebuf,buf);  //add opt l buf
    }
    sprintf(buf,"Requested filename :%s\n",shareFile);
    writelog(logpath,buf);//write log 
    if(opt[0]){
      strcat(linebuf,buf);  //add opt l buf
    }

    if(opt[4]){//if opt id 
      struct passwd *pwd;
      pwd=getpwuid(sf->si_uid);//get struct pwd of process sended sigusr1
      sprintf(buf,"User:%s, UID: %d, GID: %d \n",pwd->pw_name,sf->si_uid,pwd->pw_gid);//write username ,uid ,gid 
      writelog(logpath,buf);//write log 
      if(opt[0]){
	strcat(linebuf,buf);  //add opt l buf
      }
    }
    if(opt[0]){
      pushpidQue(linebuf);//push opt l buf to opt l que 
    }
    if(checkLock()==-1)//now lock
    { 
      if(now<queSize){//if que size is ok
	printf("\nlocked pid:%d\n",sf->si_pid);
	pushQue(sf->si_pid);//push que 
      }
      else 
	printf("que siez is full\n");//if que is full
    }
    else{//unlock
      printf("\nunlocked pid:%d\n",sf->si_pid);
      kill(sf->si_pid,SIGUSR1);//send sigusr1 to process 
    }
  }
  else{//requested file is diffrent 
    printf("requested diffrent file\n");
    }
}

static void sig_usr2(int signo,siginfo_t *sf,ucontext_t *uc){
  //if vim is done 
  char buf[1024];
  char linebuf[1024];
  bzero(linebuf,1024);//reset opt l buf 
  pid_t pid=0;

  if(opt[1]){//if opt t
    time_t now;
    time(&now);
    strftime(buf,1024,"[%F %T]",localtime(&now));//write sigusr2 received time 
    writelog(logpath,buf);//wite log 
    if(opt[0]){
      strcat(linebuf,buf);  //add opt l buf 
    }
  }

  sprintf(buf,"Finished process id: %d \n",sf->si_pid);
  writelog(logpath,buf);//write log 
  if(opt[0]){
    strcat(linebuf,buf);  //add opt l buf 
    sprintf(buf,"%s%s%s",optlbuf,poppidQue(),linebuf);//add init statement , poped que msg , opt l buf to buf
    char timename[1024]={0};

    time_t now;
    time(&now);
    strftime(linebuf,1024,"[%F %T]",localtime(&now));
    sprintf(timename,"%s/%s",curpath,linebuf);//make name of [yyyy-mm-dd tt-mm-ss] log 
    int timefd=0;

    if((timefd=creat(timename,0666))==-1){//creat l optin  log file 
      fprintf(stderr,"file create err:%s\n",timename);
      exit(1);
    }
    writelog(timename,buf);//write option l logfile
    
    close(timefd);//close opt l log 
  }

  if(now>0)  //if que has ready process 
  {
    pid=popQue();//get pid 
    printf("sigusr2 pid : %d\n", pid);
    kill(pid,SIGUSR1);//send sig usr1 
  }
}
int main (int argc,char *argv[]){
  char buf[1024];
  getcwd(curpath,1024);//get current path 
  sprintf(logpath,"%s/ssu_log.txt",curpath);//make fullpath of log file 
  sprintf(shareFile,"%s/%s",curpath,argv[1]);//make fullpath of sharefile

  if(open(shareFile,O_RDONLY|O_CREAT)==-1){//make sharefile if isn`t existed 
    fprintf(stderr,"err at make:%s\n",argv[1]);
    exit(1);
  }
  checkOpt(argc,argv);//check option

  init();//init -make demon 

  if(opt[1]){//if option t 
    time_t now;
    time(&now);
    strftime(buf,1024,"[%F %T]",localtime(&now));
    writelog(logpath,buf);//write logfile to current time 

    if(opt[0])//if opt l
      strcat(optlbuf,buf);//add opt l buf
  }

  sprintf(buf,"<<Daemon Process Initialized with pid : %d>>\n",getpid());
  writelog(logpath,buf);//write init msg to log
  if(opt[0])
    strcat(optlbuf,buf);//add init msg to opt l buf

  sprintf(buf,"Initialized with default value :%d\n",queSize);//write msg of default que size

  if(opt[2]){//if opt n 

    sprintf(buf,"Initialized Queue size :%d\n",queSize);//change msg of que size
  }

  writelog(logpath,buf);//write to log 
  if(opt[0])
    strcat(optlbuf,buf);//add optl buf 
  //optlbuf is used when option l is on and sigusr2 is received 
  while(true){
    printf("pause\n");
    pause();
  }
  return 0;
}

void init(){//init - make demon and make que and log 
  int fd,maxfd,pid=0;
  if((pid=fork())<0){
    fprintf(stderr,"fork err");
    exit(1);
  }
  else if(pid !=0){
    exit(0);
  }

  pid=getpid();
  setsid();
  signal(SIGTTIN,SIG_IGN);
  signal(SIGTTOU,SIG_IGN);
  signal(SIGTSTP,SIG_IGN);

  maxfd=getdtablesize();
  for(fd=0;fd<maxfd;fd++)
    close(fd);
  umask(0);
  chdir("/");

  fd=open("/dev/null",O_RDWR);
  dup(0);
  dup(0);
  ////make demon done this is role 

  signal(SIGTERM,sig_term);//add sigterm handler 

  struct sigaction usr1,usr2;
  sigemptyset(&usr1.sa_mask);
  usr1.sa_flags=SA_SIGINFO;
  usr1.sa_sigaction=(void(*)(int,siginfo_t*,void *))sig_usr1;
  //register sigusr1 handler 
  if(sigaction(SIGUSR1,&usr1,(struct sigaction *)NULL)<0){
    perror("sigaction for sigusr1 err:");
    exit(1);
  }
  //add sigusr1 handler using sigaction

  sigemptyset(&usr2.sa_mask);
  usr2.sa_flags=SA_SIGINFO;
  usr2.sa_sigaction=(void(*)(int,siginfo_t*,void *))sig_usr2;
  //register sigusr2 handler 

  if(sigaction(SIGUSR2,&usr2,(struct sigaction *)NULL)<0)
  {
    perror("sigaction for sigaction2 err:");
    exit(1);
  }
  //add sigusr2 handler using sigaction

  if(access(logpath,F_OK)==0){
    unlink(logpath);
  }
  //if log file existed then remove 

  if((logfile=fopen(logpath,"w"))==NULL){
    fprintf(stderr,"err at make log file\n");
    exit(1);
  }//make log file 

  fclose(logfile);
  //and close 
  if(access(FIFO_FILE,F_OK)==0){
    unlink(FIFO_FILE);
  }
  //if fifo file is existed then remove

  if((fifo=mkfifo(FIFO_FILE,0666))<0){
    fprintf(stderr,"make fifo err\n");
    exit(1);
  }//make fifo file 
  if((fifo=open(FIFO_FILE,O_RDWR))==-1){

    fprintf(stderr,"fdopen err\n");
    exit(1);
  }
  //open fifo file 
  if((key=ftok(logpath,0))==-1){
    fprintf(stderr,"ftok err\n");
    exit(1);
  }
  //make key of que
  if((que=msgget(key,0644|IPC_CREAT))==-1){
    fprintf(stderr," msgget err\n");
    exit(1);
  }
  //msgaue
  if((pidkey=ftok(shareFile,0))==-1){
    fprintf(stderr,"pid Que ftok err\n");
    exit(1);
  }
  //make key of optl que 
  if((pidque=msgget(pidkey,0644|IPC_CREAT))==-1){
    fprintf(stderr," pid Que msgget err\n");
    exit(1);
  }
  //make que of optl que 

}
int  checkLock(){//check lock of sharefile 
  struct flock lock;
  int fd;

  lock.l_type=0;
  lock.l_start=0;
  lock.l_whence=SEEK_SET;
  lock.l_len=0;


  fd=open(shareFile,O_RDWR);
  fcntl(fd,F_GETLK,&lock);
  //get state of sharefile 
  if(lock.l_type==F_UNLCK)//unlock?
    return 0;//unlock
  else 
    return -1;//lock 

}
void pushQue(pid_t ready){
  //push que 
  struct msgbuf buf;

  buf.msg_type=1;
  buf.msg_pid=ready;
  printf("push que %d\n", que);

  if(msgsnd(que,&buf,sizeof(buf.msg_pid)+1,0)==-1){//push pid of ready process
    fprintf(stderr,"msg snd err\n");
    exit(1);
  }
  now++;//add number 
}

pid_t popQue(){
  //pop que 
  struct msgbuf buf;
  buf.msg_type=1;
  buf.msg_pid=0;


  if(msgrcv(que,&buf,sizeof(buf.msg_pid)+1,0,0)==-1)//pop pid of ready process 
  {
    fprintf(stderr,"msg receive err\n");
    exit(1);
  }

  now--;//sub number 
  return buf.msg_pid;//return pid 
}
void pushpidQue(char* msg){
  //using option l when log que has log info of one pid snding sigusr1
  struct pidbuf buf;

  buf.msg_type=1;
  strcpy(buf.msg_line,msg);

  if(msgsnd(pidque,&buf,sizeof(buf.msg_line)+1,0)==-1){//push msg of sigusr1 received 
    fprintf(stderr,"msg line snd err\n");
    exit(1);
  }
}

char* poppidQue(){
  //using option l pop log que 
  struct pidbuf buf;
  buf.msg_type=1;
  bzero(buf.msg_line,strlen(buf.msg_line));


  if(msgrcv(pidque,&buf,sizeof(buf.msg_line)+1,0,0)==-1)//pop msg
  {
    fprintf(stderr,"msg line receive err\n");
    exit(1);
  }

  char *temp=buf.msg_line;

  return temp;//return msg 
}
int checkOpt(int argc,char *argv[]){

  for(int i=2;i<argc;i++)
  {
    if(argv[i][0]=='-')
    {
      switch(argv[i][1])
      {
	case 'l':
	  opt[0]=true;
	  break;
	case 't':
	  opt[1]=true;
	  break;
	case 'n'://opt n 
	  opt[2]=true;
	  queSize=atoi(argv[i+1]);//que size setting
	  break;
	case 'p':
	  opt[3]=true;
	  char temp[1024]={0};
	  sprintf(temp,"%s/%s",curpath,argv[i+1]);//change path of log made
	  mkdir(temp,0755);
	  sprintf(logpath,"%s/ssu_log.txt",temp);//change fullpath of log
	  strcpy(curpath,temp);//save curpath 
	  break;
	case 'i':
	  if(argv[i][2]=='d'){
	    opt[4]=true;
	    break;
	  }
	default:
	  fprintf(stderr,"unknown option:%s\n",argv[i]);
	  //known option
	  break;
      }
    }
  }
  return 0;
}

