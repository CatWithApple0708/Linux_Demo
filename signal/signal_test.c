#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

void handler(int sig)
{
	printf("I get alarm\n");
}

int main(int argc, const char *argv[])
{


	struct sigaction act;

	sigaction(SIGALRM, NULL, &act);

	act.sa_handler = handler;
	act.sa_flags &= ~SA_RESTART; //取消某位
	//act.sa_flags |=  SA_RESTART; //设定某位

	sigaction(SIGALRM, &act, NULL);

	while(1)
	{
		char buff[1024];
		alarm(5);
		fgets(buff,sizeof(buff),stdin);

	}


}
