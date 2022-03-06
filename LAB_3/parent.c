#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#define true 1

extern char** environ;

int qsortComp(const void* A, const void* B){
    return strcmp(*(const char**)(A), *(const char**)(B));
}

void sortEnv()
{
    int size = 0;
    char** env = environ;
    for (;*env; env++, size++);
    qsort(environ, size, sizeof(char*), qsortComp);
}

void printEnv()
{  
    system("clear");
    char** env = environ;
    for(; *env; env++) printf("%s\n", *env);
}

void menu()
{
    printf("\nChoose option:\n");
    printf("1 - print environ again\n");
    printf("2 - child proccess by getenv()\n");
    printf("3 - child proccess by Command Line\n");
    printf("4 - child proccess by environ\n");
    printf("5 - end programm\n");
}

int main(int argc, char* argv[], char* envp[])
{
    sortEnv();
    printEnv();
    char choise;
    int count = -1;
    while(true)
    {
        menu();
        scanf("%c", &choise);
        while (getchar() != '\n');
        char *name = "child_";
        if (choise != '1') count++;
        char num[] = "00";
        if (count < 10)
        {
            num[1] = (count + '0');
        }
        else
        {
            num[1] = (count % 10) + '0';
            num[0] = (count / 10) + '0';
        }

        switch(choise){
            case '1': {
                printEnv(); 
                break;
            }
            case '2':{
                pid_t pid = fork();
                if (pid == 0)
                {
                    char sym[] = "2";
                    char *newarg[] = { name, num, sym, NULL };
                    char *program = getenv("CHILD_PATH");
                    if (execve(program, newarg, environ) == -1) printf("\nERROR\n");
                    printf("%s\n", program);
                    break;
                }
                else{
                    int state;
                    waitpid(pid, &state, 0);
                    break;
                }
            }
            case '3': { 
                pid_t pid = fork();
                char* find = "CHILD_PATH";
                if (pid == 0)
                {
                    char sym[] = "3";
                    char *newarg[] = { name, num, sym, NULL };
                    for (int i = 0; environ[i] != NULL; i++)
                    {
                        if(!strncmp(environ[i], find, strlen(find)))
                        {
                            char* program = environ[i];
                            while(*program != '/') program++;
                            if (execve(program, newarg, environ) == -1) printf("\nERROR\n");
                            break;
                        }
                    }
                }
                else{
                    int state;
                    waitpid(pid, &state, 0);
                    break;
                }
                break;
            }
            case '4': { 
                pid_t pid = fork();
                char* find = "CHILD_PATH";
                if (pid == 0)
                {
                    char sym[] = "4";
                    char *newarg[] = { name, num, sym, NULL };
                    for (int i = 0; envp[i] != NULL; i++)
                    {
                        if(!strncmp(envp[i], find, strlen(find)))
                        {
                            char* program = envp[i];
                            while(*program != '/') program++;
                            if (execve(program, newarg, environ) == -1) printf("\nERROR\n");
                            break;
                        }
                    }
                }
                else{
                    int state;
                    waitpid(pid, &state, 0);
                    break;
                }
                break;
            }
            case '5': system("clear"); return 0;
            default: break;
        }
    }
}
