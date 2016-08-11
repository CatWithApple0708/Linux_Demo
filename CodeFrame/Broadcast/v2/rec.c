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

#define err_log(errlog) \
	do{\
		printf("%s:%s --> %d  \n", __FILE__, __func__, __LINE__);\
        perror(errlog);\
		exit(1);\
	}while(0)

/*
1)创建UDPsocket
2)绑定地址和端口 只需绑定端口无需绑定IP 当然信息是从广播 如果绑定地址后就只接收广播信息
3)接收数据包
*/




// #define BROADCAST_IP "192.168.2.255"
#define BROADCAST_IP "0.0.0.0"
int main(int argc, const char *argv[])
{

	int sockfd;
	struct sockaddr_in broadcastaddr, clientaddr;
	char buf[N] = {};

	if(argc < 2)
	{
		fprintf(stderr, "usage:%s port.\n", argv[0]);
		return -1;
	}

	// 1)创建UDPsocket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		err_log("fail to socket");
	}

	// printf("sockfd = %d\n", sockfd);

	// 2)绑定地址和端口
	broadcastaddr.sin_family = AF_INET;
	broadcastaddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
	broadcastaddr.sin_port = htons(atoi(argv[1]));
	if(bind(sockfd, (struct sockaddr *)&broadcastaddr, sizeof(broadcastaddr)) < 0)
	{
		err_log("fail to bind");
	}



	socklen_t  addrlen = sizeof(struct sockaddr);

	while(1)
	{
		// 3)接收数据包
		if(recvfrom(sockfd, buf, N, 0, (struct sockaddr*)&clientaddr, &addrlen) < 0)
		{
			err_log("fail to recvfrom");
		}

		printf("%s\n", buf);
		printf("%s --> %d\n", inet_ntoa(clientaddr.sin_addr), htons(clientaddr.sin_port));

		if(strncmp(buf, "quit", 4) == 0)
			break;

	}

	close(sockfd);

	return 0;
}
