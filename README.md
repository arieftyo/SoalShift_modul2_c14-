# SoalShift_modul2_c14-

## Soal 1
1. Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.

## Soal 2
2. Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat
program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu
memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab 

## Soal 3
3. Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. Buatlah program C yang dapat :
i) mengekstrak file zip tersebut. 
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt.
Catatan:
- Gunakan fork dan exec.
- Gunakan minimal 3 proses yang diakhiri dengan exec.
- Gunakan pipe
- Pastikan file daftar.txt dapat diakses dari text editor

###Jawab
```
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

```



## Soal 4
4. Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan
seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang
berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1
sampai tak hingga untuk mengingatkan Elen agar berdiet. 
Contoh:
File makan_enak.txt terakhir dibuka pada detik ke-1
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txtCatatan:
- dilarang menggunakan crontab
- Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

### Jawab
```
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

```

## Soal 5
5. Kerjakan poin a dan b di bawah:
a. Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke
/home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
Ket:
- Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
- Per menit memasukkan log#.log ke dalam folder tersebut
‘#’ : increment per menit. Mulai dari 1
b. Buatlah program c untuk menghentikan program di atas.
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan
program.
