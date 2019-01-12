#ifndef JUDGER_H
#define JUDGER_H

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <QString>


#define MAX(a,b) (((a)>(b))?(a):(b))
namespace judge{
   int AC = 0;
   int WA = 1;
   int TLE = 2;
   int MLE = 3;
};
int compiler(QString dir_path, qint32 compile_time_limit,quint32 id){
    int status = 0;
     pid_t compiler = fork();
     if (compiler < 0){
        qDebug()<<"compiler fork error...";
        return -1;
     } else if (compiler == 0){
        dir_path+="/temp";
        qDebug()<<"path="<<dir_path;
       // chdir(dir_path.toLocal8Bit().data());
      //  char buffer[1024];
       // char *hello=getcwd(buffer, 1024);
        //qDebug()<<"curdur"<<hello;

       // freopen("ce.txt","w",stderr);
       // alarm(compile_time_limit);
        QString f=dir_path+"/"+QString::number(id)+".c";
        QString f_out=dir_path+"/"+QString::number(id)+".x";//+".out";
        qDebug() << "f=" << f << " f_out=" << f_out;
       // printf("%s %s\n",f,f_out);
        QByteArray fb=f.toLocal8Bit();
        QByteArray f_outb=f_out.toLocal8Bit();
        char* arg_list[]={"gcc","-o",f_outb.data(),fb.data(),"-Wall",NULL};
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
   qDebug()<<"data"<<data<<"stack"<<stack;
  // printf("data=%d stack=%d\n",data,stack);
   return data+stack;
}

bool isInFile(char *filename)
{
    int len = strlen(filename);
    if (len < 3 || strcmp(filename+len-3, ".in") != 0)
        return false;
    return true;
}

void set_limit(qint32 time_limit,qint32 memory_limit)
{
    rlimit lim;
    lim.rlim_cur = (time_limit + 999)/1000 + 1;
    lim.rlim_max = lim.rlim_cur * 10;
    if (setrlimit(RLIMIT_CPU, &lim) < 0)
    {
        qDebug()<<"error setrlimit for rlimit_cpu";
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
 /*   unsigned rlim = memory_limit;
    if (lim.rlim_max <= rlim)
    {
        qDebug()<<"can't set steck size to higher";
    }
    else
    {
        lim.rlim_max = rlim;
        lim.rlim_cur = rlim;
        if (setrlimit(RLIMIT_STACK, &lim) < 0)
        {
            qDebug()<<"setrlimit error";
            return;
        }
    }*/
    lim.rlim_max = 1024;
    lim.rlim_cur = lim.rlim_max;
    if (setrlimit(RLIMIT_FSIZE, &lim) < 0)
    {
        qDebug()<<"setrlimit error";
        return;
    }

}

void compare_until_nonspace(int &c1, int &c2, FILE* &f1, FILE* &f2, int &ret){
   while ((isspace(c1))||(isspace(c2))){
    // printf("c1=%c, c2=%c\n",c1,c2);
      if (c1!=c2){
         if (c2=='\r' && c1=='\n'){
           c2=fgetc(f2);
           if (c1!=c2)
              ret=judge::WA;
          } else
              ret=judge::WA;
      }
      if (isspace(c1))
         c1=fgetc(f1);
      if (isspace(c2))
         c2=fgetc(f2);
   }
}


int compare_f(const char* file1, const char* file2){
   int ret=judge::AC;
   int c1, c2;
   FILE* f1=fopen(file1,"r");
   FILE* f2=fopen(file2,"r");
   if (f1==NULL){
      qDebug()<<"Korisnicki fajl ne postoji";
      ret = -1;
   }
   if (!f1 || !f2){
     ret=-1;
   } else {
        c1=fgetc(f1);
        c2=fgetc(f2);
        while (1){
         //  qDebug()<<"c1="<<c1<<"c2="<<c2;
           compare_until_nonspace(c1,c2,f1,f2,ret);
           while (!isspace(c1) || !isspace(c2)){
              if (c1==EOF && c2==EOF)
                goto end;
              if (c1!=c2){
                 //qDebug()<<"c1="<<c1<<"c2="<<c2;
                 ret=judge::WA;
                 goto end;
              }
              c1=fgetc(f1);
              c2=fgetc(f2);
           }
        }
   }
   end:
       if (f1) fclose(f1);
       if (f2) fclose(f2);
       return ret;
}

int compare_files(const char* file1, const char* file2){
  return compare_f(file1,file2);
}

int Judger(QString &dir_path, qint32 id, qint32 memory_limit, qint32 time_limit){
            DIR *dp;
            struct dirent* dirp;
          //  int programState = judge::AC;
            QByteArray dp_b=dir_path.toLocal8Bit();
            dp=opendir(dp_b.data());
            if (dp==NULL){
                 qDebug() << "opendir error";
                 return -1;
            }
            char nametmp[1024];
            while ((dirp=readdir(dp))!=NULL){
                if (strcmp(dirp->d_name,".")==0 || strcmp(dirp->d_name,"..")==0)
                      continue;
                if (isInFile(dirp->d_name)){
                   strcpy(nametmp,dirp->d_name);
                   int len=strlen(nametmp);
                   nametmp[len-3]='\0';

                   pid_t userexe;
                   int status=0;
                   QString file=dir_path+"/temp/"+QString::number(id);
                   QString program_input=dir_path+"/"+nametmp+".in";
                   QString program_output_std=dir_path+"/"+nametmp+".oout";
                   QString program_output=dir_path+"/temp/"+QString::number(id)+".out";

                   QByteArray pi_b= program_input.toLocal8Bit();
                   QByteArray po_b=program_output.toLocal8Bit();
                   QByteArray file_b=file.toLocal8Bit();
                   QByteArray pos_b=program_output_std.toLocal8Bit();
                   struct rusage rused;
                   if ((userexe=fork())<0){
                        qDebug()<<"fork error";
                        return -1;
                   } else if (userexe==0){ //child
                       qDebug()<<"u child-u smo";
                    //   printf("file:%s\n",file.c_str());
                      // printf("ulaz je: %s",program_input.c_str());

                   freopen(pi_b.data(),"r",stdin);
                   freopen(po_b.data(),"w",stdout);
                   alarm(time_limit);
                   set_limit(time_limit,memory_limit);

                   execvp(file_b.data(),NULL);
                   } else{
                   qint32 memory_used=128;
while(1){

                      if (wait4(userexe, &status, 0, &rused) < 0){
                         qDebug()<<"wait4 error";
                         return -1;
                      }
                      if (WIFEXITED(status)){
                        qDebug()<<"i did it!";

                        if (compare_files(po_b.data(),
                                           pos_b.data())==1){
                           return judge::AC;
                        }
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
                                  kill(SIGKILL,userexe);
                                  switch (signo){
                                        case SIGALRM:
                                            qDebug()<<"time limit exceeded...";
                                            return judge::TLE;
                                        case SIGXCPU:
                                            qDebug()<<"used too much processor time...";
                                            return judge::TLE;
                                         case SIGXFSZ:
                                            qDebug()<<"too much files used...";
                                            return judge::MLE;
                                         case SIGSEGV:
                                            qDebug()<<"segmentation fault";
                                            return judge::MLE;
                                         default:
                                            qDebug()<<"something else...";
                                            return -1;
                                  }
                                 // kill(SIGKILL,userexe);
                                  //break;
                          }

                               // printf("%d kB",get_memory_usage(userexe));
                                memory_used = MAX(memory_used, get_memory_usage(userexe));
                                if (memory_used > memory_limit){
                                   qDebug()<<"memory limit exceeded";
                                   kill(userexe, SIGKILL);
                                   return judge::MLE;
                                   //break;
                                }


}


                }
            }}
           closedir(dp);
           return judge::WA;
}

#endif // JUDGER_H

