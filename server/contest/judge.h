#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ptrace.h>


#define MAX(a,b) (((a)>(b))?(a):(b))
using namespace std;

void set_limit(int time_limit)
{
    rlimit lim;
    lim.rlim_cur = (time_limit + 999)/1000 + 1;
    lim.rlim_max = lim.rlim_cur * 10;
    if (setrlimit(RLIMIT_CPU, &lim) < 0)
    {
        perror("error setrlimit for rlimit_cpu");
        return;
    } 
  
  /*  lim.rlim_cur = (time_limit + 999)/1000 + 1;
    lim.rlim_max = lim.rlim_cur * 10;
    if (setrlimit(RLIMIT_RTTIME, &lim) < 0)
    {
        perror("error setrlimit for rlimit_cpu");
        return;
    }*/

    getrlimit(RLIMIT_STACK, &lim);
    int rlim = 32992;
    if (lim.rlim_max <= rlim)
    {
        perror("can't set steck size to higher");
    }
    else
    {
        lim.rlim_max = rlim;
        lim.rlim_cur = rlim;
        if (setrlimit(RLIMIT_STACK, &lim) < 0)
        {
            perror("setrlimit error");
            return;
        }
    }
    lim.rlim_max = 1024;
    lim.rlim_cur = lim.rlim_max;
    if (setrlimit(RLIMIT_FSIZE, &lim) < 0)
    {
        perror("setrlimit error");
        return;
    }

}


int compiler(std::string &dir_path, int compile_time_limit,int id){
   int status = 0;
   pid_t compiler = fork();
   if (compiler < 0){
      printf("compiler fork error...");
      exit(EXIT_FAILURE);
   } else if (compiler == 0){
      dir_path+="/temp";
      printf("path=%s",dir_path.c_str());
      chdir(dir_path.c_str());
      freopen("ce.txt","w",stderr);
      alarm(compile_time_limit);
      std::string f=std::to_string(id)+".c";
      std::string f_out=std::to_string(id);//+".out";
      char* arg_list[]={"gcc",(char*)f.c_str(),"-o",(char*)f_out.c_str(),"-Wall",NULL};
      execvp(arg_list[0],(char* const*)arg_list);
   } else{
       waitpid(compiler,&status,0);
       return status;
   }
}

int get_memory_usage(pid_t pid){
   int fd, data, stack;
   char buffer[4096], status_child[100];
   char *vm;
   
   sprintf(status_child, "/proc/%d/status",pid); /*radi istu stvar kao printf
   samo sto umesto da stampa cuva string odgovarajuceg formata*/
   if ((fd=open(status_child,O_RDONLY))<0)
      return -1;
   /* ako dodje do greske, inace u ovom direktorijumu su info o ovom procesu*/
   read(fd, buffer, 4095); /* u buffer-u je ucitan fajl */
   buffer[4095]='\0';
   data=stack=0;
   vm = strstr(buffer, "VmData:"); /* trazimo prvo pojavljivanje zadatog paterna u
   buffer-u, ako ne postoji rezultat je NULL*/
   if (vm){
       sscanf(vm, "%*s%d",&data);
   }
   vm = strstr(buffer, "VmStk:");
   if (vm){
       sscanf(vm, "%*s%d", &stack);
   }
   printf("data=%d stack=%d\n",data,stack);
   return data+stack;
}


bool isInFile(char *filename)
{
    int len = strlen(filename);
    if (len < 3 || strcmp(filename+len-3, ".in") != 0)
        return false;
    return true;
}

void Judger(std::string &dir_path, int id, int memory_limit, int time_limit, int compile_time_limit){
       if (compiler(dir_path,compile_time_limit,id)!=0){
           printf("compilation error");
           exit(EXIT_FAILURE);
       } else{
            printf("compilation succeded");
            printf("here i go running...");
            DIR *dp;
            struct dirent* dirp;
            std::string pomocni_path=dir_path+"/pomocni";
            dp=opendir(pomocni_path.c_str());
            if (dp==NULL){
                 perror("opendir error");
                 exit(EXIT_FAILURE);
            }
            char nametmp[1024]; 
            while ((dirp=readdir(dp))!=NULL){
                if (strcmp(dirp->d_name,".")==0 || strcmp(dirp->d_name,"..")==0)
                      continue;
                //printf("%s\n",dirp->d_name);
               // printf("here am i");
                if (isInFile(dirp->d_name)){
                   printf("here i go again");
                   strcpy(nametmp,dirp->d_name);
                   int len=strlen(nametmp);
                   nametmp[len-3]='\0';
                   printf("ime ulaznog fajla:%s\n",nametmp);
                   

                   pid_t userexe;
                   int status=0;
                   struct rusage rused;
                   if ((userexe=fork())<0){
                        perror("fork error");
                       // exit(EXIT_FAILURE);
                       return;
                   } else if (userexe==0){ //child
                       printf("u child-u sam");
                       std::string file=dir_path+"/temp/"+std::to_string(id);
                       printf("file:%s\n",file.c_str());
                       std::string program_input=pomocni_path+"/"+nametmp+".in";
                       printf("ulaz je: %s",program_input.c_str());
                       freopen(program_input.c_str(),"r",stdin);
                       std::string program_output_std=pomocni_path+"/"+nametmp+".out";
                       std::string program_output=dir_path+"/temp/"+std::to_string(id)+".out";
                       //kreirati ovaj fajl
                       freopen(program_output.c_str(),"w",stdout);
                      alarm(time_limit);
                       set_limit(time_limit);
                      // std::string file=dir_path+"/temp/"+std::to_string(id);
                      // printf("file:%s\n",file.c_str());
                       
                       execvp(file.c_str(),NULL);
                   } else{
                   int memory_used=128;
while(1){

                      if (wait4(userexe, &status, 0, &rused) < 0){
                         perror("wait4 error");
                         return;
                        // exit(EXIT_FAILURE);
                      }
                      if (WIFEXITED(status)){
                        printf("i did it!\n");
                       /* printf("%d kB",get_memory_usage(userexe));
                                memory_used = MAX(memory_used, get_memory_usage(userexe));
                                if (memory_used > memory_limit){
                                   printf("memory limit exceeded");
                                   kill(userexe, SIGKILL);
                                  // break;
                                }*/
                        break;
                      } 
               
                          if (WIFSIGNALED(status) || (WIFSTOPPED(status) && WSTOPSIG(status) != SIGTRAP))
                          {
                                  int signo = 0;
                                  if (WIFSIGNALED(status))
                                      signo = WTERMSIG(status);
                                  else
                                      signo = WSTOPSIG(status);
                                  if (signo==SIGALRM)
                                      printf("time limit exceeded...");
                                  if (signo==SIGXCPU)
                                    printf("used too much processor time...");
                                  if (signo==SIGXFSZ)
                                     printf("too much files used...");
                                  if (signo==SIGSEGV)
                                       printf("segmentation fault");
                                  printf("something else...");
                                  kill(SIGKILL,userexe);
                                  break;
                          }  
                     
                                printf("%d kB",get_memory_usage(userexe));
                                memory_used = MAX(memory_used, get_memory_usage(userexe));
                                if (memory_used > memory_limit){
                                   printf("memory limit exceeded");
                                   kill(userexe, SIGKILL);
                                   break;
                                }
                     
                   
}


                }
            }}
           closedir(dp);
      }
     //  printf("hello");
}

/*
int main(){
       std::string path="/home/jelena/Desktop/judge2";
       Judger(path, 1, 194, 4, 1);
       return 0;
}*/
