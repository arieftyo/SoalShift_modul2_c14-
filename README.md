# SoalShift_modul2_c14-

## Soal 1
1. Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.

### Jawab
```
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

int main() {
  pid_t pid, sid;

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

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
	DIR *d;
	struct dirent *dir;
	char fname[255][255];
	char fnew[255][255];
	char srcpath[255];
	char destpath[255];
	int i=0,j=0;

	d = opendir("campur2/");
	if (d != NULL){
		while ((dir = readdir(d)) != NULL){
			    strcpy(fname[i],dir->d_name);
			char *a = strrchr(dir->d_name, '.');
			if (strcmp(a, ".png") == 0){		
				strncpy(fnew[j], fname[i], (strlen(fname[i])-4));
				strcat(fnew[j], "_grey.png");			
				strcpy(srcpath,"campur2/");
				strcpy(destpath,"gambar/");
				strcat(srcpath, fname[i]);
				strcat(destpath, fnew[j]);


				rename(srcpath, destpath);		
				memset(srcpath, 0, sizeof srcpath);
				memset(destpath, 0, sizeof destpath);				
				j++;
		}
		i++;
	}
        closedir(d);
    }
    sleep(30);
  }
}
```
### Penjelasan :
Pada soal ini, kita disuruh membuat program yang dapat mengubah file yang mempunyai ekstensi .png menjadi “[namafile]_grey.png”.  Kami menggunakan file yang sudah ada yaitu file png yang terdapat dalam folder campuran2.zip. Pertama, kita buka dan membaca file yang ada di dalam folder campuran2. Ketika membaca file-file yang ada di dalam folder campuran2, program akan mencari file yang berekstensi .png, lalu akan dimasukkan ke dalam array fnew[i] dengan hanya mengcopy string yang terdapat di depan ekstensi .png. Setelah itu, nama file tadi akan ditambahkan dengan “_grey.png” dan akan disimpan di dalam folder gambar. Kemudian array path yaitu srcpath dan destpath agar array tersebut dapat dipakai berulang kali. 


## Soal 2
2. Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat
program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu
memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab

### Jawab
```
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>


int main() {
  pid_t pid, sid;

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

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
	struct stat status;
	struct group *group;
	struct passwd *pass;
	char fname[255][255];
	int i;

	stat("elen.ku",&status);

	uid_t userid = status.st_uid;
	gid_t groupid = status.st_gid;
	group = getgrgid(groupid);
	pass = getpwuid(userid);

	if(strcmp(group->gr_name,"www-data") == 0 && strcmp(pass->pw_name,"www-data") == 0){
		remove("elen.ku");
		sleep(30);
		}
	}
	exit(EXIT_SUCCESS);
}

```

### Penjelasan
Pada soal ini, kita disuruh membuat program untuk menghapus file elen.ku pada folder hatiku. Pertama, kita mengambil status dari userid dan groupid untuk mengambil nama user dan nama group di dalam file elen.ku. Setelah ketemu nama user dan groupnya, nama group dan ama user akan dicompare dengan “www-data”. Jika cocok, maka file elen.ku akan dihapus dengan fungsi remove.

## Soal 3
3. Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. Buatlah program C yang dapat :
i) mengekstrak file zip tersebut. 
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt.
Catatan:
- Gunakan fork dan exec.
- Gunakan minimal 3 proses yang diakhiri dengan exec.
- Gunakan pipe
- Pastikan file daftar.txt dapat diakses dari text editor

### Jawab
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
### Penjelasan
Penyelesaian ini menggunakan fork 2 kali untuk memanggil child 2 kali. Dalam soal ini juga menggunakan pipe untuk meneruskan hasil dari child sebelumnya. if pertama (child pertama) digunakan untuk unzip file campur2.zip. child yang selanjutnya digunakan untuk mencari file yang berekstensi txt di dalam folder hasil unzip. Lalu yang terakhir digunakan untuk menuliskan semua nama file yang berekstensi txtx ke file daftar.txt


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

### Penjelasan
Pada soal ini dibutuhkan untuk mendaptakan waktu akses file dan waktu sekarang.rawtime adalah waktu yang sekarang dan access adalah waktu akses file makan_enak.txt dengan menggunakan st.atime. JIka file pernah diakses kurang lebih 30 detik yang lalu maka akan membuat file baru makan_sehat#.txt.

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

### Jawab

```
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main() {
  pid_t pid, sid,child;
  int status;
  static int  urutan ;
  urutan = 0;
  char a[100];
  char letak[150]="/home/ariefp/praktikum/";
  char file[100];
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
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

  if ((chdir("/home/ariefp/praktikum")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

while(1){
  if(urutan%30==0){
    child= fork();
    if(child==0){
        char a[100];
        time_t now = time(NULL);
        strftime(a, 20, "%d:%m:%Y-%H:%M", localtime(&now));
        char *argv[3] = {"mkdir", a, NULL};
  	execv("/bin/mkdir", argv);
    }
  }
  urutan ++;
  child= fork();
  if(child ==0){
   sprintf(file, "/log%d", urutan);
   strcat(letak, a);
   strcat(letak, file);
   strcat(letak, ".log");
   char *argv[4]= {"cp", "/var/log/syslog", letak, NULL};
   execv("/bin/cp", argv);
  }
  sleep(60);
 }
 exit(EXIT_SUCCESS);
}
```
###Penjelasan
Untuk membuat folder bernama waktu sekarang menggunakan mkdir. Waktu didapatkan dengan cara strftime. Urutan selalu ditambah tiap menit. Lalu setelah 30 menit membuat folder baru. Karena membutuhkan exec maka dibuat child tiap akan menjalankannya. var/log/syslog di copy ke nama file yang baru. Karena file belum ada maka file baru akan terbentuk

