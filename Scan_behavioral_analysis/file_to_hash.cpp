#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<fstream>
#include <sys/stat.h>
#include"sha256.h"
using std::string;
using std::cout;
using std::endl;
 

string readFileIntoString4(char* path) {
    struct stat sb{};
    string res;

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open\n");
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

int main(int argc, char *argv[])
{
    cout<<fpath_to_hash(argv[1])<<endl;
    return 0;
}