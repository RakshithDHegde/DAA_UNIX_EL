#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
int main()
{
    int t = open("/media/shree/College/Sem_4/DAA/a.c",O_WRONLY|O_APPEND);
    printf("%d\n",getpid());
    sleep(10);
    printf("Process woke up!\n");
    write(t,"File changed",13);
    while(1)
    {
        
    }
}