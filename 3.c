#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pid, pid1;
    int pip[2];
    char txt[1000];


    pid = fork();
    if (pipe(pip)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }

    if (pid < 0) {
     exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execl("/usr/bin/unzip", "unzip", "/home/ariefp/praktikum/campur2.zip", (char *)0);
    }

    else {
        while((wait(&pid))>0);
        pid1 = fork();

	if (pid1 < 0) {
    	  exit(EXIT_FAILURE);
    	}

        if (pid1 == 0) {
          chdir("/home/ariefp/praktikum/campur2");
          dup2 (pip[1], STDOUT_FILENO);
    	  close(pip[0]);
   	  close(pip[1]);
	  execl("/usr/bin/find", "find" , "-iname", "*.txt", (char *)0);
        }
        else {
	  while((wait(&pid1)) > 0);
          close(pip[1]);
          int n;
          FILE *daftar;
          daftar = fopen("daftar.txt", "w");
	  n =read(pip[0], txt, sizeof(txt));
	  fprintf(daftar, "(%.*s)\n", n, txt);
          fclose(daftar);
	}
    }

    return 0;
}
