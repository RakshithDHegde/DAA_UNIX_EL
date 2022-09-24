#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<fstream>
#include <sys/stat.h>
#include<signal.h>
#include"sha256.h"
#include"strmatch.h"

using namespace std;

bool chk =false;

void str_algos(const char* fname,string pat)
{
    long long int *count1=(long long *)malloc(sizeof(long long));
    long long int *count2=(long long *)malloc(sizeof(long long));
    long long int *count3=(long long *)malloc(sizeof(long long));
    char* buff = (char*) malloc(sizeof(char)*10000);
    int fd = open(fname,O_RDONLY);
    int x = read(fd,buff,10000);
    do
    {
        // printf("%s",buff);
        int k = harspool(buff,pat,count1);
        int k1 = boyer_moore(buff,pat,count2);
        int k2 = rabin_karp(buff,pat,count3);
        if(k>0)
            break;
        x = read(fd,buff,10000);
    }while(x>0);
    cout<<"File: "<<fname<<endl;
    cout<<"Harspool "<<(*count1)%10000007<<endl;
    cout<<"Boyer-moore "<<(*count2)%10000007<<endl;
    cout<<"Rabin karp "<<(*count3)%10000007<<endl;
    cout<<"-------------"<<endl;
}
string readFileIntoString4(char* path) {
    struct stat sb{};
    string res;

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        //perror("open\n");
    }

    fstat(fd, &sb);
    res.resize(sb.st_size);
    read(fd, (char*)(res.data()), sb.st_size);
    close(fd);

    return res;
}
string fpath_to_hash(char *f_path)
{
    string file = readFileIntoString4(f_path);
    return sha256(file);
}
int check(char* f_name)
{
    string curr_hash = fpath_to_hash(f_name);
    
    std::ifstream input("hashes");
    for( std::string line; getline( input, line ); )
    {
        int i=0;
        while(line[i]!=' ')
            i++;
        i++;
        string hash_entry = line.substr(i);
        string file_name_entry = line.substr(0,i-1);
        
        str_algos(file_name_entry.c_str(),hash_entry);
        // cout<<file_name_entry<<endl;
        if(!strcmp(f_name,file_name_entry.c_str()))
        {
            int k = strcmp(curr_hash.c_str(),hash_entry.c_str());
            chk=true;
            return k;
        }
            
    }
    return -1;
}
void update_hash(char* f_name)
{
    std::ifstream input("hashes");
    bool chk=false;
    ofstream book;
    book.open("temp.txt",ios::app);
    for( std::string line; getline( input, line ); )
    {
            int i=0;
            while(line[i]!=' ')
                i++;
            i++;
            string hash_entry = line.substr(i);
            string file_name_entry = line.substr(0,i-1);
            
            // cout<<file_name_entry<<endl;
            if(!strcmp(f_name,file_name_entry.c_str()))
            {
                book<<file_name_entry+" "+fpath_to_hash(f_name)<<endl;
               
            }
            else
            {
                book<<line<<endl;
            }
    }
    book.close();
    rename("temp.txt","hashes");
}
void file_access_detection(char *pid)
{
    char cmd[50] = "lsof -p ";
    strcat(pid," > out.txt");
    strcat(cmd,pid);
    // printf("%s\n",cmd);
    system(cmd);
    int f = open("out.txt",O_RDONLY);
    // printf("%d\n",f);
    // system(path);
    char *token;
    char* buff = (char*) malloc(sizeof(char)*10000);
    // printf(buff);
    
    int x = read(f,buff,10000);
    
    do
    {
        // printf("x = %d\n",x);
        token = strtok(buff, "\n");
        while( token != NULL ) {
        // printf( "%s \n",token );
        
        while(*(token)!='/')
        {
            token++;
        }
            
        if(*(token+1)!='O')
        {
            // printf("%s\n",token);  //Files which are opened by the process...
            int t = check(token);
            if(chk&&t==0)
                printf("File %s is accessed but hasn't been changed \n",token);
            else if(chk&&t!=0)
            {
                printf("File %s has been changed by the process \n",token);
                update_hash(token);
                kill(stoi(pid),SIGKILL);
            }
            
        }
        token = strtok(NULL, "\n");
        }
        x = read(f,buff,10000);
    }while(x>0);
}

int main(int argc,char* argv[])
{
    file_access_detection(argv[1]);
    
}
