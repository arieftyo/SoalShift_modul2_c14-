#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main() {
  pid_t pid, sid;
  int urutan=0;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1){
   FILE* fp;
   struct stat buf;
   stat("/home/ariefp/Documents/makanan/makan_enak.txt", &buf);
   time_t rawtime = time(NULL);
   time_t access = buf.st_atime;
   if((rawtime - access) <= 30){
    char lokasi[150]= "/home/ariefp/Documents/makanan/";
    char nama[100];
    urutan++;
    sprintf(nama, "makan_sehat%d.txt", urutan);
    strcat(lokasi, nama);
    fp = fopen(lokasi, "w+");
    fclose(fp);
   }
  sleep(5);
 }
  return 0;
}
