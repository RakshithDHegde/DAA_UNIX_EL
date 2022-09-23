/* We want POSIX.1-2008 + XSI, i.e. SuSv4, features */
#define _XOPEN_SOURCE 700

/* Added on 2017-06-25:
   If the C library can support 64-bit file sizes
   and offsets, using the standard names,
   these defines tell the C library to do so. */
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64 

#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <fstream>

#include <openssl/md5.h>

void generate_hash();
extern int errno;
using namespace std;

/* POSIX.1 says each process has at least 20 file descriptors.
 * Three of those belong to the standard streams.
 * Here, we use a conservative estimate of 15 available;
 * assuming we use at most two for other uses in this program,
 * we should never run into any problems.
 * Most trees are shallower than that, so it is efficient.
 * Deeper trees are traversed fine, just a bit slower.
 * (Linux allows typically hundreds to thousands of open files,
 *  so you'll probably never see any issues even if you used
 *  a much higher value, say a couple of hundred, but
 *  15 is a safe, reasonable value.)
*/
#ifndef USE_FDS
#define USE_FDS 15
#endif

//Hashing
unsigned char result[MD5_DIGEST_LENGTH];

// Print the MD5 sum as hex-digits.

void print_md5_sum(unsigned char *md)
{
  int i;
  for (i = 0; i < MD5_DIGEST_LENGTH; i++)
  {
    printf("%02x", md[i]);
  }
}

// Get the size of the file by its file descriptor
unsigned long get_size_by_fd(int fd)
{
  struct stat statbuf;
  if (fstat(fd, &statbuf) < 0)
    exit(-1);
  return statbuf.st_size;
}

int fd;
const char* path="/home/rakshith/Desktop/scannedfilepath.txt";
int print_entry(const char *filepath, const struct stat *info,
                const int typeflag, struct FTW *pathinfo)
{
    /* const char *const filename = filepath + pathinfo->base; */
    const double bytes = (double)info->st_size; /* Not exact if large! */
    struct tm mtime;

    localtime_r(&(info->st_mtime), &mtime);





    if (typeflag == FTW_SL) {
        char   *target;
        size_t  maxlen = 1023;
        ssize_t len;

        while (1) {

            target = (char*)malloc(maxlen + 1);
            if (target == NULL)
                return ENOMEM;

            len = readlink(filepath, target, maxlen);
            if (len == (ssize_t)-1) {
                const int saved_errno = errno;
                free(target);
                return saved_errno;
            }
            if (len >= (ssize_t)maxlen) {
                free(target);
                maxlen += 1024;
                continue;
            }

            target[len] = '\0';
            break;
        }

        printf(" %s -> %s\n", filepath, target);
        free(target);

    } else
    if (typeflag == FTW_SLN)
        printf(" %s (dangling symlink)\n", filepath);
    else
    if (typeflag == FTW_F)
        printf(" %s\n", filepath);
    else
    if (typeflag == FTW_D || typeflag == FTW_DP)
        printf(" %s/\n", filepath);
    else
    if (typeflag == FTW_DNR)
        printf(" %s/ (unreadable)\n", filepath);
    else
        printf(" %s (unknown)\n", filepath);
     
    
     
   if(fd==-1){
   cout<<"ERROR"<<endl;
   }

        write(fd,filepath,strlen(filepath));
  
        write(fd,"\n",strlen("\n"));
        
    

    return 0;
}


int print_directory_tree(const char *const dirpath)
{
    int result;

    /* Invalid directory path? */
    if (dirpath == NULL || *dirpath == '\0')
        return errno = EINVAL;

    result = nftw(dirpath, print_entry, USE_FDS, FTW_PHYS);
    if (result >= 0)
        errno = result;

    return errno;
}
//Generating md5
void generate_hash(){
int file_descript;
  unsigned long file_size;
  char *file_buffer;

  char total[100][100];

  int fp;

  int i = 0;
  const char *pathdatabase = "/home/rakshith/Desktop/scanneddatabase.txt";
  printf("NO segv0\n");
 
  int fddatabase = open(pathdatabase,  O_WRONLY|O_APPEND );
   printf("NO segv later\n");
  
 
  char line[500];
  char md5string[500];

  std::ifstream input("/home/rakshith/Desktop/scannedfilepath.txt");
  printf("NO segv0\n");
  for (std::string line; getline(input, line);)
  { 
    // printf("%s\n",line.c_str());
    file_descript = open(line.c_str(), O_RDONLY| 0777 |O_WRONLY );


    if (file_descript < 0)
    {
     perror("Program");
     close(file_descript);
      continue;
    }

    file_size = get_size_by_fd(file_descript);

    file_buffer = (char *)mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);

    MD5((unsigned char *)file_buffer, file_size, result);
    munmap(file_buffer, file_size);
    

    for (int i = 0; i < 16; ++i)
      sprintf(&md5string[i * 2], "%02x", (unsigned int)result[i]);

    strcpy(total[i], md5string);
    i++;
    printf("%s\n", md5string);
    md5string[strlen(md5string)] = '\n';
    int x = write(fddatabase, md5string, strlen(md5string));
    printf("Write done %d \n", x);

    close(file_descript);
}
}

int main(int argc, char *argv[])
{
    fd=open(path,O_WRONLY|O_CREAT|O_TRUNC,0777);
    int arg;
    int choice;
    cout<<"What do you like to do\n1)Full Scan\n2)Quick Scan\n";
    cin>>choice;
    
    if(choice==1){if (argc < 2) {

        if (print_directory_tree("/")){
            fprintf(stderr, "%s.\n", strerror(errno));
            return EXIT_FAILURE;
        }

    else {

        for (arg = 1; arg < argc; arg++) {
            if (print_directory_tree(argv[arg])) {
                fprintf(stderr, "%s.\n", strerror(errno));
                return EXIT_FAILURE;
            }
        }

    }}
    }
    else if(choice==2){if (argc < 2) {

        if (print_directory_tree("/home/rakshith/Desktop")) {
            fprintf(stderr, "%s.\n", strerror(errno));
            return EXIT_FAILURE;
        }

    } else {

        for (arg = 1; arg < argc; arg++) {
            if (print_directory_tree(argv[arg])) {
                fprintf(stderr, "%s.\n", strerror(errno));
                return EXIT_FAILURE;
            }
        }

    }}
    else{
    cout<<"Enter the valid input"<<endl;
    }

    
      generate_hash();     
        close(fd);
      

    return EXIT_SUCCESS;
}