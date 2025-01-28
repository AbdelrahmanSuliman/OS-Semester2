#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void environment(){
    char **s =__environ;
    
    for(; *s; s++){
        printf("%s\n", *s);
    }
}

void list_files(const char* path){

    struct dirent *entry;

    DIR *dir =  opendir(path);

    if(dir == NULL){
        perror("opening directory failed");
        return;
    }

    while((entry = readdir(dir)) != NULL){
       if(entry->d_name[0] == '.'){
           continue;
       }
       else{
         printf("%s ", entry->d_name); 
       }

    }
    printf("\n");
    closedir(dir);
}

void set(char* variable, char* value){
    setenv(variable, value, 1);
}

void change_directory(char *path){
    char cwd[1024];

    if(path == NULL){
       char* cwd = getenv("PWD");
        if(cwd){
            printf("%s\n", cwd);
        }
        return;
    }

    else  if(strcmp(path, "..") == 0){
        chdir("..");
        setenv("PWD",cwd, 1 );
    }


    if(chdir(path)!= 0){
        perror("cd failed");
        return;
    }

    if(getcwd(cwd, sizeof(cwd)) != NULL){
        setenv("PWD", cwd, 1);
    };

}


void execute_command(char** words){
    pid_t pid = fork();

    if(pid < 0) {
        perror("fork failed");
        return;
    }
    else if(pid == 0) { 
        if(execvp(words[0], words) == -1) {
            perror("command execution failed");
            exit(EXIT_FAILURE);
        }
    }
    else {  
        waitpid(pid, NULL, 0);
    }
}

int main(){
    chdir("/");
    setenv("PWD", "/", 1);
    while(1){
        //current working directory
        char cwd[1024];
       
        
        printf("/root%s> ", getenv("PWD"));

        //taking in a command (maximum 100 words)
        size_t len = 100;
        char* input =  malloc(len * sizeof(char));


        getline(&input,&len, stdin);
        
        //creating array of strings to store command (10 words maximum)
        size_t capacity = 10;
        char** words = malloc(capacity * sizeof(char*));
    
        char* token = strtok(input," \n");

        //save each individual token(word) into an array
        size_t count = 0;
        while (token != NULL){
            words[count] = strdup(token);
            count++;
            token = strtok(NULL, " \n");

    }


    //checks the inputted command
        if(words[0] == NULL){
            printf("please input a command \n");
        }
        else  if(strcmp(words[0],"ls") == 0 || strcmp(words[0], "dir") == 0){
            if(words[1]){
                list_files(words[1]);
            }
            else{
                list_files(".");
            }
        }
        else if (strcmp(words[0], "cd") == 0) {
            if (words[1] == NULL) {

                change_directory(NULL);
            }
            else if(strcmp(words[1],".." ) == 0){
                change_directory("..");
            }
            else {
                change_directory(words[1]);
            }
        }
        else if(strcmp(words[0],"exit") == 0){
            break;
        }
        else if(strcmp(words[0], "echo") == 0){
            size_t index = 2;
            char *current = words[1];
            while(current != NULL){
                printf("%s ", current);
                current =  words[index++];
            }
            printf("\n");
        }
        else if(strcmp(words[0],"set") == 0){
            set(words[1], words[2]);
        }
        else if(strcmp(words[0], "environ") == 0){
            environment();
        }
        else{
            execute_command(words);
        }

    for(size_t i = 0; i < count;i++){
        free(words[i]);
    }
    free(words);
    free(input);
}

}
