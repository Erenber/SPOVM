#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

int cnt = 0;
int flag = 0;

volatile struct pair{
	int a;
	int b;
} memory;
static struct pair zeros = {0, 0}, ones = {1, 1};
static struct pair mas[4];

void handler(int sig){
	if  (sig == SIGALRM)
	{
        if (flag == 4)
        {
            kill(getppid(), SIGUSR1);
            flag = 0;
            alarm(3);
        }
        else
        {
            mas[flag++] = memory;
            alarm(1);
        }
	}

	//if (sig == SIGUSR1) printf("\nPPID: %d PID: %d\n", getppid(), getpid());
	if (sig == SIGUSR2) {
        printf("\nPPID: %d PID: %d Numbers:", getppid(), getpid());
        for (int i = 0; i < 4; i++) printf(" %d%d", mas[i].a, mas[i].b);
        printf("\n");
    }
	if (sig == SIGTERM) exit(0);
}


int main(int argc, char* argv[])
{
	printf("Child process was created (mod %s)\n", argv[0]);

	struct sigaction signal_struct;

    signal_struct.sa_handler = handler;
    sigemptyset(&signal_struct.sa_mask);
    signal_struct.sa_flags = 0;
    if (sigaction(SIGALRM, &signal_struct, NULL) == -1) {
        perror("CHILD: Error in sigaction (SIGALRM)!\n");
        exit(errno);
    }
    if (sigaction(SIGUSR1, &signal_struct, NULL) == -1) {
        perror("CHILD: Error in sigaction (SIGUSR1)!\n");
        exit(errno);
    } 
    if (sigaction(SIGUSR2, &signal_struct, NULL) == -1) {
        perror("CHILD: Error in sigaction (SIGUSR2)!\n");
        exit(errno);
    } 
    if (sigaction(SIGTERM, &signal_struct, NULL) == -1) {
        perror("CHILD: Error in sigaction (SIGTERM)!\n");
        exit(errno);
    } 
	memory = zeros;
	alarm(1);
	while(1)
	{
		memory = zeros;
		memory = ones;
	}
}
