//includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//defining constants
#define maxBuffSize 255
#define maxArgsSize 255

//Method Declarations
char* readLine(); //this function will read input from user
char** parseArgs(); //this function will parse input and extract command and its args
int execute(char**); //this function will execute command, command and args will be passed as args
void runShell(); //this function will run shell, and perform its tasks


//Main function, this will call runShell() method and starts execution of it.
int main(int argc, char **argv){

    runShell();
    return 0;
}

/*
    Method's Definition.
    In below section of code, all of the methods which were declared above are defined.
*/
void runShell(){
    char* line;
    char **args;
    int status;
    do{
        printf("~:");
        line = readLine();
        args = parseArgs(line);
        status = execute(args);
        free(line);
        free(args);
    }while(1);
}

char* readLine(){
    char *line;
    int bufferSize = maxBuffSize;
    int index = 0;
    int character;
    
    //lets allocate memory to line
    line = malloc(sizeof(char)*bufferSize);
    //checking if memory allocated or not
    if(!line){
        fprintf(stderr, "Failed to allocate Memory");
        exit(EXIT_FAILURE);
    }
    while(1){
        character = getchar();
        if(character == EOF || character == '\n'){
            line[index] = '\0';
            return line;
        }
        else{
            line[index] = character;
        }
        index+=1;

        //if command exceeds maxBuffSize, we will reallocate memory
        bufferSize+=maxBuffSize;
        line = realloc(line, sizeof(char)*bufferSize);
        //if allocation fails
        if(!line){
            fprintf(stderr, "Failed to reallocate memory. ");
            exit(EXIT_FAILURE);
        }
    }
    return line;

}

char** parseArgs(char *line){
    char **args;
    char* token;
    int index = 0;
    int argsBufferSize = maxArgsSize;
    //allocating memory to args
    args = malloc(sizeof(char)*argsBufferSize);
    if(!args){
        fprintf(stderr, "Failed to allocate Memory");
        exit(EXIT_FAILURE);
    }
    //now using strtok() to fetch tokens
    token  = strtok(line, " ");
    while(token!=NULL){
        args[index] = token;
        index+=1;

        //checking if we have space in args buffer or not
        if(index >= maxArgsSize){
            argsBufferSize+=maxArgsSize;
            args = realloc(args, sizeof(char)*argsBufferSize);
            if(!args){
                fprintf(stderr, "Failed to allocate Memory");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " ");
    }
    args[index] = NULL;    
    return args;
}

int execute(char** args){
    pid_t waitPID;
    pid_t childProcess = fork();
    int status;
    if(childProcess < 0){
        fprintf(stderr, "Failed to create process.");
        exit(EXIT_FAILURE);
    }
    else if(childProcess == 0){
        char* command = args[0];
        //Child Process
        execvp(args[0], args);
    }
    else{
        //Main proces
        do {
        waitPID = waitpid(childProcess, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

