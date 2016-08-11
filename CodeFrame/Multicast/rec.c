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


// 组播地址 D类:224.0.0.0~239.255.255.255,一般于用组播



/*
1)创建UDPsocket
2)加入多播组
2)绑定地址和端口 只需绑定端口无需绑定IP 当然信息是从广播 如果绑定地址后就只接收广播信息
3)接收数据包
*/




// #define BROADCAST_IP "192.168.2.255"
#define HOST_IP "0.0.0.0"
int main(int argc, const char *argv[])
{

	printf("Multicast rec\n");
	int sockfd;
	char buf[N] = {};
	if(argc < 2)
	{
		fprintf(stderr, "usage:%s ip port.\n", argv[0]);
		return -1;
	}


	// 1)创建UDPsocket
	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		err_log("fail to socket");



	// 2)加入多播组
	struct ip_mreq mrep;
	bzero(&mrep,sizeof(mrep));
	mrep.imr_multiaddr.s_addr = inet_addr(argv[1]); //绑定一个组播地址 地址范围是 224.0.0.1  239.255.255.255
	mrep.imr_interface.s_addr = htonl(INADDR_ANY);

	if( setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mrep,sizeof(mrep)) < 0)
		perror("fail to setsockopt"),exit(-1);





	// 3)绑定地址和端口
	struct sockaddr_in myaddr;
	bzero(&myaddr,sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(argv[1]);
	myaddr.sin_port = htons(atoi(argv[2]));

	if(bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
		err_log("fail to bind");


	// 4)接收数据包
	struct sockaddr_in clientaddr;
	socklen_t  addrlen = sizeof(clientaddr);
	while(1)
	{

		if(recvfrom(sockfd, buf, N, 0, (struct sockaddr*)&clientaddr, &addrlen) < 0)
			err_log("fail to recvfrom");

		printf("%s --> %d:%s\n", inet_ntoa(clientaddr.sin_addr), htons(clientaddr.sin_port),buf);

	}

	close(sockfd);

	return 0;
}


// IPV4地址分类
/*
A类:1.0.0.0~126.255.255.255,默认子网掩码/8,即255.0.0.0 (其中127.0.0.0~127.255.255.255为环回地址,用于本地环回测试等用途)；

B类:128.0.0.0~191.255.255.255,默认子网掩码/16,即255.255.0.0；

C类:192.0.0.0~223.255.255.255,默认子网掩码/24,即255.255.255.0；

D类:224.0.0.0~239.255.255.255,一般于用组播

E类:240.0.0.0~255.255.255.255(其中255.255.255.255为全网广播地址),E类地址一般用于研究用途..

*/

