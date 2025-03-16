#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void list_files(const char* dirname){
    DIR *dir = opendir(dirname);
    if(!dir){
        perror("directory does not exist");
        return;
    }

    struct dirent *entry;
    printf("%s directory content: \n", dir);
    while((entry = readdir(dir))){
        printf("%s\n", entry->d_name);
    }
    printf("\n");

    closedir(dir);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("please input a valid directory");
    }

    for(int i = 1;i < argc;i++){
        list_files(argv[i]);
    }
    return 0;
}
