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
      if(getcwd(cwd, sizeof(cwd)) != NULL){
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


void process_input(FILE* input_source) {
    while(1){
        char cwd[1024];
        printf("/root%s> ", getenv("PWD"));

        size_t len = 100;
        char* input = malloc(len * sizeof(char));
        if (fgets(input, len, input_source) == NULL) {
            break; 
        }

        size_t capacity = 10;
        char** words = malloc(capacity * sizeof(char*));
        char* input_file = NULL;
        char* output_file = NULL;
        int append_mode = 0;

        int run_background = 0;
        char* token = strtok(input," \n");
        size_t count = 0;
        
        while (token != NULL){
            if (strcmp(token, "<") == 0) {
                token = strtok(NULL, " \n");
                if (token) input_file = strdup(token);
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " \n");
                if (token) {
                    output_file = strdup(token);
                    append_mode = 0;
                }
            } else if (strcmp(token, ">>") == 0) {
                token = strtok(NULL, " \n");
                if (token) {
                    output_file = strdup(token);
                    append_mode = 1;
                }

            } else {
                if(strcmp(token, "&") == 0){
                    run_background = 1;
                    token = NULL;
                }
                else{
                    words[count] = strdup(token);
                    count++;
                }
            }
            token = strtok(NULL, " \n");
        }
        words[count] = NULL;  

        if(words[0] == NULL){
            printf("please input a command \n");
        }
        else {
            FILE* original_stdout = NULL;
            FILE* new_stdout = NULL;
            
            if (output_file) {
                original_stdout = stdout;
                new_stdout = fopen(output_file, append_mode ? "a" : "w");
                if (new_stdout) {
                    stdout = new_stdout;
                }
            }

            if(strcmp(words[0],"ls") == 0 || strcmp(words[0], "dir") == 0){
                if(words[1]){
                    list_files(words[1]);
                }
                else{
                    list_files(".");
                }
            }
            else if(strcmp(words[0], "cd") == 0) {
                if (words[1] == NULL) {
                    change_directory(NULL);
                }
                else if(strcmp(words[1],"..") == 0){
                    change_directory("..");
                }
                else {
                    change_directory(words[1]);
                }
            }
            else if(strcmp(words[0],"exit") == 0){
                if (output_file) {
                    stdout = original_stdout;
                    fclose(new_stdout);
                    free(output_file);
                }
                if (input_file) free(input_file);
                break;
            }
            else if(strcmp(words[0], "echo") == 0){
                size_t index = 2;
                char *current = words[1];
                while(current != NULL){
                    printf("%s ", current);
                    current = words[index++];
                }
                printf("\n");
            }
            else if(strcmp(words[0],"set") == 0){
                set(words[1], words[2]);
            }
            else if(strcmp(words[0], "environ") == 0){
                environment();
            }
            else if(strcmp(words[0], "pause") == 0){
                printf("please press enter to continue..");
                while(getchar() != '\n'){
                    continue;
                }
            }
            else if(strcmp(words[0], "help") == 0){
                printf(" All commands: \n");
                printf("ls [path] -List files in current directory \n");
                printf("cd [path] -Change current directory \n");
                printf("set VAR VALUE -Set environment variable \n");
                printf("environ -Display all environment variables \n");
                printf("echo [text] -Print text to console \n");
                printf("pause  -Pause execution until enter is pressed\n");
                printf("exit -Exits the shell \n");
                printf("help -Display this help manual \n");

            }
            else {
                pid_t pid = fork();
                if(pid < 0) {
                    perror("fork failed");
                } else if(pid == 0) {
                    if (input_file) {
                        FILE* new_stdin = freopen(input_file, "r", stdin);
                        if (!new_stdin) {
                            perror("Failed to redirect input");
                            exit(EXIT_FAILURE);
                        }
                    }
                    if (output_file) {
                        FILE* new_stdout = freopen(output_file, append_mode ? "a" : "w", stdout);
                        if (!new_stdout) {
                            perror("Failed to redirect output");
                            exit(EXIT_FAILURE);
                        }
                    }
                    if(execvp(words[0], words) == -1) {
                        perror("command execution failed");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    if(run_background == 0){
                         waitpid(pid, NULL, 0);
                    }
                    else{
                        printf("Running in background... \n");
                        signal(SIGCHLD, SIG_IGN);
                    }
                }
            }

            if (output_file) {
                fflush(stdout);
                stdout = original_stdout;
                fclose(new_stdout);
                free(output_file);
            }
        }

        for(size_t i = 0; i < count; i++){
            free(words[i]);
        }
        free(words);
        free(input);
        if (input_file) free(input_file);
    }
}

int main(int argc, char* argv[]){
   
    FILE* file; 
    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("Could not open file");
            exit(EXIT_FAILURE);
        }
        process_input(file);
        printf("\n");
        fclose(file);       
    } else {
        char *home = getenv("HOME");
        if (home) {
            setenv("PWD", home, 1);
            chdir(home);
        } else {
            setenv("PWD", "/", 1);
            chdir("/");
        }
        process_input(stdin);
    }

    return 0;

}
