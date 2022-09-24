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

int main()
{
  int file_descript;
  unsigned long file_size;
  char *file_buffer;

  char total[100][100];

  int fp;

  int i = 0;
  const char *pathdatabase = "malwaredatabase.txt";

  int fddatabase = open(pathdatabase, O_WRONLY | O_APPEND);
  printf("%d\n", fddatabase);
  char line[500];
  char md5string[500];
  std::ifstream input("malware_name.txt");
  for (std::string line; getline(input, line);)
  { 
    // printf("%s\n",line.c_str());
    file_descript = open(line.c_str(), O_RDONLY);

    if (file_descript < 0)
    {
      printf("Error");
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
  

  close(fddatabase);
  
  return 0;
}