#include "ssu_header.h"
//ssu_vim.c//

bool opt[3]={false};//check for t s d option
char *tempName;  //store temp file name
bool confirm=false;//check confirm to ofm
int fd;//sharefile descripter
pid_t ofmPid=0;//pid of ofm


char filepath[1024];//full path of sharefile
char file[1024];//share file name
struct flock fl;//file lock 

void sig_usr1(int signo){//sigusr1 handler
  confirm=true;//make confirm 
  alarm(0);//disable alarm 
  fl.l_type=F_WRLCK;
  fl.l_start=0;
  fl.l_whence=SEEK_SET;
  fl.l_len=0;
  fd=open(filepath,O_RDWR);//open for lock 
  fcntl(fd,F_SETLK,&fl);//file lock

}
void sig_alrm(int signo){
  time_t now;
  char buf[200];  
  time(&now);//current time 
  strftime(buf,1024,"[%F %T]",localtime(&now));//write time format of [yyyy-mm-dd hh-mm-ss]
  if(opt[0])//if opt t is on 
    printf("Wait for Token...%s%s\n",file,buf);//print file name and time
  else//isn`t 
    printf("Wait for Token...%s\n",file);//print file name
}
int main(int argc ,char *argv[]){
  strcpy(file,argv[1]);//copy share file name 
  if(signal(SIGUSR1,sig_usr1)==SIG_ERR){//register sigusr1 handler 
    perror("signal err:");
    exit(1);
  }

  struct stat obuf,nbuf;//check for modification
  int pid;//option d need fork

  for(int i=3;i<argc;i++)
  {
    if(argv[i][1]=='t'){//if there was option t
      opt[0]=true;
    }
    else if(argv[i][1]=='s'){//if there was option s

      opt[1]=true;
    }
    else if(argv[i][1]=='d'){//if there was option d

      opt[2]=true;
    }
  }


  if(stat(argv[1],&obuf)==-1)//store state brfore change
  {
    fprintf(stderr,"file dosent exist...\n");
    exit(1);
  }
  if(opt[0])//if option t used 
  {
    struct tm* time;
    char buf[1024];
    printf("##[Modidication Time]##\n");
    time=localtime(&obuf.st_mtime);//get file modification time
    strftime(buf,1024,"[%F %T]",time);//make format 
    printf("Last Modification time of '%s': %s\n",file, buf);//print mtime of sharefile 
    sleep(1);//leasting 1sec 
  }

  switch(argv[2][1]){
    case 'r':
      { 
	rMode();//vim r mode is on
	if(argv[2][2]=='w')//if option rw 
	{
	  char yn[10];
	  printf("Would you like to modify '%s'? (y/n):",file);//check write
	  scanf("%s",yn);

	  if((yn[0]=='y')||(yn[0]=='Y'))//if say yes
	  { 
	    check();//check for lock 
	    wMode();//open with vim for write

	    fl.l_type= F_UNLCK;
	    fl.l_start=0;
	    fl.l_whence=SEEK_SET;
	    fl.l_len=0;
	    fcntl(fd,F_SETLK,&fl);
	    //file unlock
	    kill(ofmPid,SIGUSR2);//send sigusr2 to ofm 
	  }
	}
	break;
      }
    case 'w'://option -w
      {
	if(argv[2][2]=='r')//if user typed -wr
	{
	  fprintf(stderr,"known vim option %s \n",argv[2]);
	  exit(1);
	}
	check();//check for lock 
	wMode();//vim write mode 
	fl.l_type= F_UNLCK;
	fl.l_start=0;
	fl.l_whence=SEEK_SET;
	fl.l_len=0;
	fcntl(fd,F_SETLK,&fl);
	//file unlock 
	kill(ofmPid,SIGUSR2);//send sigusr2 to ofm 
	break;
      }
    default:
      {//if there is unknown option used
	fprintf(stderr,"known vim option %s \n",argv[2]);
	exit(1);
      }
  }

  stat(file,&nbuf);//check state after change

  if(opt[0])//option t used 
  {
    printf("##[Modidication Time]##\n");
    if(nbuf.st_mtime==obuf.st_mtime)//if modification time is changed
      printf("- There was no modification\n");
    else
      printf("- There was modification\n");//else
  }

  if(opt[1]){//option s used
    if(nbuf.st_mtime!=obuf.st_mtime)//if file is modificated
    {
      printf("##[File Size]##\n");
      printf("-- Before modificatoin: %ld(byte)\n",obuf.st_size);//printf before size
      printf("-- After modificatoin: %ld(byte)\n",nbuf.st_size);//printf after size
    }
  }

  if(opt[2])//if option d used 
  {
    if(nbuf.st_mtime!=obuf.st_mtime)//if modificated 
    {
      if((pid=fork())<0)//fork child process 
      {
	fprintf(stderr,"fork err\n");
	exit(1);
      }
      if(pid==0){//child
	printf("##[Compare with Previous File]##\n");
	if(execl("/usr/bin/diff","diff",tempName,file,(char*)NULL)<0)//exec diff 
	  exit(1);
      }
      wait(0);//wait child processor
      unlink(tempName);//remove tempfile
    }

  }


  return 0;
}
int rMode(){//read mode 

  FILE* fp=0;
  int size=1024;
  char buf[size];
  if((fp=fopen(file,"r"))<0)//open for read
    return -1;

  while(fgets(buf,size,fp)!=NULL){//until EOF
    if(fputs(buf,stdout)==EOF)// printf stdout
      return -1;//if err
  }
  fclose(fp);//close file 
  return 0;
}
int wMode(){//write mode 
  int status, pid=0;
  char buf[1024];
  FILE *fp;
  FILE *tempfile;

  if(opt[2]){//option d 
    tempName=tempnam(NULL,"tetee");//make tempfile name 
    tempfile=fopen(tempName,"w");//make temp file
    fp=fopen(file,"r");

    while(fgets(buf,1024,fp)!=NULL){
      if(fputs(buf,tempfile)==EOF)//copy file at tempfile
	return -1;
    }
    fclose(fp);//close file 
    fclose(tempfile);//close tempfile
  }

  if((pid=fork())<0)
  {
    fprintf(stderr,"fork err at vim \n");
    exit(1);
  }
  if(pid==0)//child processor
  {
    execl("/usr/bin/vim","vim",file,(char*)NULL);//exec vim 
  }

  if(wait(&status)!=pid)//wait child process
  {
    fprintf(stderr,"wait err\n");//fork err
    exit(1);
  }
  return 0;
}

int check() {//check lock of sharefile ofm 
  int fifo;
  sprintf(filepath,"%s/%s",getcwd(NULL,0),file);//make fullpath of sharefile

  if(access(FIFO_FILE,F_OK)!=0){
    fprintf(stderr,"ofm is not running... \n");
    exit(1);
  }

  fifo= open(FIFO_FILE,O_RDWR);
  write(fifo,filepath,strlen(filepath)+1);//write filepaht at fifo file 

  if(signal(SIGALRM,sig_alrm)==SIG_ERR)//sigalrm handler 
  {
    perror("siganl sigalrm err:");
    exit(1);
  }

  if ((ofmPid=getOfmPid()) == -1) {//get pid of ofm 
    fprintf(stderr,"ofm is not running...\n");
    exit(1);
  }

  kill(ofmPid,SIGUSR1);//send sigusr1 to ofm

  while (confirm==false) {//while sig1 received 
    alarm(1);//make alrm 1sec 
    pause();
  }

  if (signal(SIGALRM,SIG_DFL) == SIG_ERR) {//if confirm sig1
    perror("sig_alrm reset err:");
    exit(1);
  }
  return 0;
}

pid_t getOfmPid(){//return pid of ofm 

  DIR *dp=malloc(sizeof(DIR*));
  struct dirent *dirp=malloc(sizeof(struct dirent));
  char namebuf[1024];
  char temp[1024];

  if((dp=opendir("/proc"))==NULL)//get DIR of /proc
  {
    fprintf(stderr,"opendir err at /proc\n");
    return -1;
  }

  while((dirp=readdir(dp))!=NULL)//search directory of /proc 
  {
    if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0)||(atoi(dirp->d_name)==0))
      continue;// skip directory name . .. and non-digit because of this is not pid

    strcpy(namebuf,"/proc/");
    strcat(namebuf,dirp->d_name);
    strcat(namebuf,"/cmdline");//namebuf has /proc/[pid]/status


    int tempfd;

    if((tempfd=open(namebuf,O_RDONLY))<0){//if there is not err about file descriptor of read file 
      fprintf(stderr,"fopen err\n");
      exit(1);
    }
    else{

      long rbyte=read(tempfd,temp,sizeof(temp));//read cmdline file
      if(strcmp(temp,"./ofm")==0)//compare ofm
      {
	return (pid_t)atoi(dirp->d_name);//return pid
      }
    }
  }
  return -1;
}
