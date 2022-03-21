#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>

char permission = '1';
int pcnt = 0;

struct CHILD{
    pid_t pid;
    char permission;
}children[15] = {};

int getNum(char* str)
{
    int num = 0;
    for (int i = 1; str[i] && str[i] != '\n'; i++)
    {
        num = num * 10 + (str[i] - '0');
    }
    return num;
}

void SetPermission(char mod)
{
    permission = mod;
    printf("Now permission is set to %c\n", mod);
    for (int i =0; i < pcnt; i++) children[i].permission = mod;
}

void killProcesses()
{
    for (int i = 0; i < pcnt; i++) kill(children[i].pid, SIGTERM);
    pcnt = 0;
    printf("There no more processes (%d)\n", pcnt);
}

void handler(int sig, siginfo_t *si, void *context){
    if (sig == SIGALRM){
        if (permission == '0'){
            SetPermission('1');
            printf("Processes enabled again\n");
        }
	} 
    if (sig == SIGUSR1)
    {
        for (int i = 0; i < pcnt; i++)
        {
            if(children[i].pid == si->si_pid)
            {
                if (children[i].permission == '1') kill(si->si_pid, SIGUSR2);
                break;
            }
        }
    }
}

int main ()
{ 
    struct sigaction siganlStruct;
    siganlStruct.sa_sigaction = handler;
	sigemptyset(&siganlStruct.sa_mask);
    siganlStruct.sa_flags = SA_SIGINFO;
    if (sigaction(SIGALRM, &siganlStruct, NULL) == -1) {
        perror("PARENT: Error in sigaction (SIGALRM)!\n");
        exit(errno);
    }
    if (sigaction(SIGUSR1, &siganlStruct, NULL) == -1) {
        perror("PARENT: Error in sigaction (SIGUSR1)!\n");
        exit(errno);
    } 
    
    char* program = "/home/dima/Documents/LAB4/child";  
    pid_t pid;
    printf("Ready to work:\n");
    while(1)
    {
        char choise[10] = {};
        fgets(choise, 10, stdin);
        
        if (strlen(choise) - 1 == 1){
            switch(choise[0])
            {
                case '+':{
                    pid  = fork();
                    if (pid == -1) printf("\nError");
                    if (pid == 0){
                        if (execl(program, &permission, NULL) == -1) {
                            perror("PARENT: Error in execl\n");
                            exit(errno);
                        }
                    }
                    else 
                    {
                        children[pcnt].pid = pid;
                        children[pcnt++].permission = permission;
                    }
                    break;
                }
                case '-': {
                    if (pcnt > 0){
                        kill(children[pcnt - 1].pid, SIGTERM);
                        pcnt--;
                        printf("Child process was closed, there are %d more process(es)\n", pcnt);
                    }
                    break;
                }
                case 'k':{
                    killProcesses();
                    break;
                }
                case 's':{
                    SetPermission('0');
                    break;
                }
                case 'g': {
                    SetPermission('1');
                    break;
                }
                case 'q':{
                    killProcesses();
                    return 0;
                }
                default: break;
            }
        }
        else
        {
            int num = getNum(choise);
            if (num > pcnt) continue;
            switch(choise[0]){
                case 's':{
                    children[num - 1].permission = '0'; //disable child output
                    break;
                }
                case 'g': {
                    children[num - 1].permission = '1'; //enable child output
                    break;
                }
                case 'p':{
                    SetPermission('0');
                    kill(children[num - 1].pid, SIGUSR2);   
                    alarm(5);
                    break;
                }
                default: break;
            }
        }
    }
}