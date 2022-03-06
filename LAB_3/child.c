#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

extern char** environ;

int main(int argc, char* argv[], char* envp[])
{
	system("clear");
	printf("CHILD: NAME = %s%s\n", argv[0], argv[1]);
	printf("CHILD: PID = %d\n",getpid()); 
	printf("CHILD: Parent PID = %d\n",getppid());

	char PATH[]= "/home/dima/Documents/info.txt";
	FILE* f;
	char param[15] = {};
	if (!(f = fopen(PATH, "r"))) printf("Can not open file\n");
	while(!feof(f))
	{
		fscanf(f, "%14s", param);
		if (feof(f)) break;
		if (argv[2][0] == '2')
		{
			char* inf = getenv(param);
			printf("%s=%s\n", param, inf);
		}
		if (argv[2][0] == '3')
		{
			for (int i = 0; environ[i] != NULL; i++)
            {
                if(!strncmp(environ[i], param, strlen(param)))
                {
                    printf("%s\n", environ[i]);
                    break;
                }
            }
		}
		if (argv[2][0] == '4')
		{
			for (int i = 0; envp[i] != NULL; i++)
            {
                if(!strncmp(envp[i], param, strlen(param)))
                {
                    printf("%s\n", envp[i]);
                    break;
                }
            }
		}

	}
	fclose(f);
	return 0;
}
