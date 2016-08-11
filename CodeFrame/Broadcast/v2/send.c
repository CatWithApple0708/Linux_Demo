#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define  N  128

/*
1)创建UDP socket
2)指定目标地址和端口
3)设置socket选项允许发送广播包
4)发送数据包
*/


#define err_log(errlog) \
	do{\
		printf("%s:%s --> %d  ", __FILE__, __func__, __LINE__);\
        perror(errlog);\
		exit(1);\
	}while(0)

#define BROADCAST_IP "192.168.2.255"
int main(int argc, const char *argv[])
{

	// 0)检测参数是否正确
	if(argc < 2)
	{
		fprintf(stderr, "usage:%s port.\n", argv[0]);
		return -1;
	}

	// 1)创建UDP socket
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		err_log("fail to socket");
	}

	// printf("sockfd = %d\n", sockfd);

	//2)指定目标地址和端口
	struct sockaddr_in broadcastaddr;
	broadcastaddr.sin_family = AF_INET;
	broadcastaddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
	broadcastaddr.sin_port = htons(atoi(argv[1]));


	// 3)设置socket选项允许发送广播包
	int on = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(int)) < 0)
	{
		err_log("fail to setsockopt.");
	}


	// 4)发送数据包
	while(1)
	{
		char buf[N] = {0};
		printf("Input:");
		fgets(buf, N, stdin);
		buf[strlen(buf)-1] = '\0';

		if(sendto(sockfd, buf, N, 0, (struct sockaddr *)&broadcastaddr, sizeof(broadcastaddr)) < 0)
		{
			err_log("fail to sendto");
		}

		if(strncmp(buf, "quit", 4) == 0)
			break;

	}

	close(sockfd);

	return 0;
}
