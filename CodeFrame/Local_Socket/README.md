#生成一个echo_socke  套接字文件

#Unix本地socket

Linux和UNIX都拥有一个非常实用的工具--UNIX套接字，或称为本地套接字，它可以被用在进程间通讯（IPC）当中。UNIX套接字的运转机制和Internet套接字类似，主要的区别UNIX套接字只能用在一台计算机中，而Internet套接字则可以在不同的计算机之间使用。UNIX套接字定址的方式是作为本地文件系统里的一个文件。

你可能会奇怪为什么要使用UNIX套接字而不使用常规的Internet套接字呢？或许最大的原因就是安全和速度了。无论何时，当你打开任何Internet套接字的时候，你可能就帮远程的黑客打开了入侵之门。


```c
#define UNIX_PATH_MAX    108

struct sockaddr_un {
   sa_family_t sun_family;               /* AF_UNIX */
   char        sun_path[UNIX_PATH_MAX];  /* pathname */
};
```
