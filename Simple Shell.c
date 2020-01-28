#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

FILE* log;
void sig_handler(int signal);

int main(){
    char* command;
    char* ccommand;
    char pwd[PATH_MAX];
    char** cmdtok;
    char* tok;
    int i, j;
    int z = 0;
    pid_t chpid;
    int stat;
    int ampflag;
    //scanf("%m[^\n]s", &command);
    //printf("%s", command);
    log = fopen("./log.txt", "w");
    fclose(log);

    while(1){
        /**Printing the current directory*/
        if(getcwd(pwd, sizeof(pwd)) != NULL){
            printf("%s: ", pwd);
        }
        else{
            /**There is an error in getcwd()*/
            perror("getcwd() error");
            return 1;
        }
        /**Users hits enter without typing a command*/
        while(fscanf(stdin, "%s") == "\n");
        scanf("%m[^\n]s", &command);
        getchar();
        //printf("%s, %d\n", command, strlen(command));
        if(!strcmp(command, "exit")){
            //printf("I'm exiting");
            fclose(log);
            exit(0);
        }else{
            /**Tokenizing the Command Input*/
            ampflag = 0;
            ccommand = malloc(strlen(command) + 1);
            strcpy(ccommand, command);
            ccommand[strlen(command)] = '\0';
            i = 0;
            tok = strtok(ccommand, " ");
            while(tok != NULL){
                if(!strcmp(tok, "&")){
                    ampflag = 1;
                }else{
                    i++;
                    //printf("%s %d\n", tok, i);
                }
                tok = strtok(NULL, " ");
            }
            cmdtok = malloc(sizeof(char*) * (i + 1));
            i = 0;
            tok = strtok(command, " ");
            while(tok != NULL){
                if(!strcmp(tok, "&")){
                    ampflag = 1;
                    //printf("No wait\n");
                }else{
                    cmdtok[i] = malloc(strlen(tok) + 1);
                    strcpy(cmdtok[i], tok);
                    cmdtok[i][strlen(tok)] = '\0';
                    //printf("%s %s\n", cmdtok[i], tok);
                    i++;
                }
                tok = strtok(NULL, " ");
            }
            cmdtok[i] = malloc(sizeof(char*));
            cmdtok[i] = NULL;
            /**Executing the command input*/
            signal(SIGCHLD, sig_handler);
            chpid = fork();
            if(chpid == 0){
                /**Child's executing*/
                execvp(cmdtok[0], cmdtok);
                /**Execution failed*/
                perror("Unknown Command\n");
                _exit(1);
            }else if(chpid > 0){
                /**Parent's executing and has to wait*/
                if(!ampflag){
                    while(wait(&stat) != chpid);
                }
            }else{
                perror("Can't execute process\n");
                _exit(1);
            }
            /**Free memory*/
            for(j = 0; j < i; j++){
                free(cmdtok[j]);
            }
            free(cmdtok);
            free(ccommand);
        }
        free(command);
    }
    return 0;
}

void sig_handler(int signal){
    if(signal == SIGCHLD){
        /**Reopening the log file*/
        log = fopen("./log.txt", "a");
        fprintf(log, "Child terminated\n");
        fclose(log);
    }
}
