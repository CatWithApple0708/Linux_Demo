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

// 组播地址 D类:224.0.0.0~239.255.255.255,一般于用组播

// IPV4地址分类
/*
A类:1.0.0.0~126.255.255.255,默认子网掩码/8,即255.0.0.0 (其中127.0.0.0~127.255.255.255为环回地址,用于本地环回测试等用途)；

B类:128.0.0.0~191.255.255.255,默认子网掩码/16,即255.255.0.0；

C类:192.0.0.0~223.255.255.255,默认子网掩码/24,即255.255.255.0；

D类:224.0.0.0~239.255.255.255,一般于用组播

E类:240.0.0.0~255.255.255.255(其中255.255.255.255为全网广播地址),E类地址一般用于研究用途..

*/

/*
1)创建UDP socket
2)指定目标地址和端口
3)发送数据包
*/


#define err_log(errlog) \
	do{\
		printf("%s:%s --> %d  ", __FILE__, __func__, __LINE__);\
        perror(errlog);\
		exit(1);\
	}while(0)

#define Multicast "192.168.2.255"
int main(int argc, const char *argv[])
{
	printf("Multicast send\n");

	// 0)检测参数是否正确
	if(argc < 2)
	{
		fprintf(stderr, "usage:%s ip port.\n", argv[0]);
		return -1;
	}

	// 1)创建UDP socket
	int sockfd;
	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		err_log("fail to socket");

	// printf("sockfd = %d\n", sockfd);

	//2)指定目标地址和端口
	struct sockaddr_in broadcastaddr;
	broadcastaddr.sin_family = AF_INET;
	broadcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
	broadcastaddr.sin_port = htons(atoi(argv[2]));

	// 3)发送数据包
	while(1)
	{
		char buf[N] = {0};
		printf("Input:");
		fgets(buf, N, stdin);
		buf[strlen(buf)-1] = '\0';

		if( sendto(sockfd, buf, N, 0, (struct sockaddr *)&broadcastaddr, sizeof(broadcastaddr)) < 0)
		{
			err_log("fail to sendto");
		}

		if(strncmp(buf, "quit", 4) == 0)
			break;

	}

	close(sockfd);

	return 0;
}
