#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/md5.h>


unsigned char result[MD5_DIGEST_LENGTH];

// Print the MD5 sum as hex-digits.


void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

// Get the size of the file by its file descriptor
unsigned long get_size_by_fd(int fd) {
    struct stat statbuf;
    if(fstat(fd, &statbuf) < 0) exit(-1);
    return statbuf.st_size;
}

int main() {
    int file_descript;
    unsigned long file_size;
    char* file_buffer;



char total[100][100];

int fp;
 //   char * line ;
   // size_t len = 0;
    //ssize_t read;
  
  int i=0;
 const  char *pathdatabase="/malwaredatabase.txt";

  FILE* file = fopen("/home/rakshith/Desktop/malwares.txt", "r"); /* should check the result */
    char line[256];
char md5string[33];
    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
           line[strlen(line)-1]='\0';
         
           file_descript=open(line,O_RDONLY);

    if(file_descript < 0) {printf("Error");}
    

    file_size = get_size_by_fd(file_descript);
   
    file_buffer = mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
   
    MD5((unsigned char*) file_buffer, file_size, result);
    munmap(file_buffer, file_size);
   // print_md5_sum(result);
   // printf("\n");
   
   
for(int i = 0; i < 16; ++i)
    sprintf(&md5string[i*2], "%02x", (unsigned int)result[i]);
  
  strcpy(total[i],md5string);
  i++;
   printf("%s\n",md5string);
    //write(fddatabase,md5string,strlen(md5string));
  //printf("Write done\n");
  
    
  close(file_descript);
        
    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);
  
   int fddatabase=open(pathdatabase,0666);
  if(fddatabase<0){
  printf("Error\n");}
  for(int j=0;j<i;j++){
  write(fddatabase,total[i],strlen(total[i]));
  }


close(fddatabase);
 /*  fp = open("/home/rakshith/Desktop/malwares.txt", O_RDONLY|0666);
  
  

    while ((read = getline(&line, &len, fp)) != -1) {

  

file_descript=open((char *)line,O_RDONLY);
   printf("%s",line);
    if(file_descript < 0) {printf("Error");}
    

    file_size = get_size_by_fd(file_descript);
   
    file_buffer = mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
   
    MD5((unsigned char*) file_buffer, file_size, result);
    munmap(file_buffer, file_size);
    print_md5_sum(result);
  close(file_descript);
    }

    close(fp);
   
   */     
     
   

    return 0;
}
