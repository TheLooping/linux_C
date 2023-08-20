#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
/**
 * @param sockfd: 调用socket()函数返回的套接字描述符
 * @param servaddr: 准备连接的服务器的地址结构体指针
 * @param addrlen: 服务器地址结构体的长度
 * @return: 成功返回0，失败返回-1
 * @note: 客户端调用connect()函数向服务器发起连接请求，即启动TCP3次握手。
 *        如果连接成功，客户端就可以通过套接字描述符sockfd与服务器通信；
 *        如果连接失败，客户端可以根据errno的值判断错误原因：
 *          1. ETIMEDOUT: 客户端在发出首个SYN包后，等待服务器的SYN+ACK包时超时，大约75秒后返回此错误；
 *          2. ECONNREFUSED: 服务器主动拒绝客户端的连接请求，即向客户端发送RST包，表示服务器上没有进程在监听客户端指定端口，
 *                       此时客户端TCP可以向用户进程返回一个ECONNREFUSED错误；
 *          3. ENETUNREACH/EHOSTUNREACH: 客户端TCP发出的SYN分段到达路由器时，发现不可达，此时路由器向客户端发送ICMP报文，
 *                                   客户端重发SYN分段，时间超过一定值（75s）后，客户端TCP向应用返回此错误；
 **/

//bind函数
int bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);
/**
 * @brief: 服务器调用bind()函数将套接字描述符sockfd与本地地址绑定
 * @param sockfd: 调用socket()函数返回的套接字描述符
 * @param myaddr: 本地地址结构体指针
 * @param addrlen: 本地地址结构体的长度
 * @return: 成功返回0，失败返回-1
 * @note: 1. 对于TCP套接字，bind()函数可以同时指定IP地址和端口号，也可以只指定端口号或者IP地址，也可以不指定IP地址和端口号：
 *          1.1 如果不指定端口号，则内核会为套接字选择一个合适的端口号，（客户端通常不需要指定端口号，服务端需要明确指定端口号）
 *          1.2 如果只指定端口号，则内核会为套接字选择一个合适的IP地址；
 *        2. 应用程序可以指定套接字绑定到某个特定的IP地址/端口号，也可以让内核自动选择；
 *        3. TCP客户端并不显式绑定到特定的IP地址，服务端需要显式绑定到特定的IP地址/端口号；
 **/

//listen函数
int listen(int sockfd, int backlog);
/**
 * @brief: 服务器调用listen()函数将套接字描述符sockfd转换为监听套接字，使得内核可以接收来自客户端的连接请求
 * @param sockfd: 调用socket()函数返回的套接字描述符
 * @param backlog: 内核为相应套接字排队的最大连接个数，默认值为128
 * @return: 成功返回0，失败返回-1
*/

//accept函数
int accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
/**
 * @brief: 服务器调用accept()函数从已完成连接队列中取出一个已完成连接，如果已完成连接队列为空，则阻塞等待，直到有新的连接到来
 * @param sockfd: 用于监听的套接字描述符
 * @param cliaddr: 用于存放客户端地址结构体的指针
 * @param addrlen: 指向接收的套接字地址缓存最大长度的指针，作为输入：指定cliaddr指向的缓存的长度；作为输出：返回实际接收到的套接字地址缓存的长度
 * @return: 成功返回连接套接字描述符，服务端可以通过连接套接字描述符与客户端通信；失败返回-1
 * @note: 1. 连接套接字描述符是一个全新的套接字描述符，与监听套接字描述符sockfd不同，它只用于与客户端通信；在通信结束后，需要关闭连接套接字；
 *        2. 原有监听套接字sockfd仍然可以继续接收新的连接请求，直到应用结束，调用close()函数关闭监听套接字；
*/

//套接字IO操作
/**
 * 流式套接字的IO操作与文件IO操作类似，可以使用read()/write()函数进行读写，也可以使用readv()/writev()函数进行读写；
*/
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
/**
 * @brief: 从套接字中读取数据
 * @param fd: 套接字描述符
 * @param buf: 用于存放读取数据的缓存
 * @param nbytes: 缓存的长度
 * @return: 成功返回读取到的字节数，失败返回-1；读到EOF返回0；当既未读到数据，也未读到EOF时，read()函数会阻塞等待，直到有数据到来；
*/
ssize_t write(int fd, const void *buf, size_t nbytes);
/**
 * @brief: 向套接字中写入数据
 * @param fd: 套接字描述符
 * @param buf: 用于存放写入数据的缓存
 * @param nbytes: 缓存的长度
 * @return: 成功返回写入的字节数，失败返回-1
*/
int close(int fd);
int shutdown(int fd, int howto);
/**
 * @brief: 关闭套接字
 * @param howto: 关闭方式, SHUT_RD: 不允许读取；SHUT_WR: 不允许写入；SHUT_RDWR(=close()): 不允许读写
 * @note: 1. close()函数只是减少套接字描述符的引用计数，即-1，只有当引用计数为0时，才会真正关闭套接字；
 *       2. shutdown()函数可以立刻指定关闭套接字的读端、写端或者读写端，但是并不会减少套接字描述符的引用计数
*/

//数据报套接字IO操作：sendto()/recvfrom()
#include <sys/socket.h>
#include <sys/types.h>
int sendto(int sockfd, const void *buf, size_t nbytes, int flags, const struct sockaddr *to, socklen_t addrlen);
/**
 * @brief: 向指定的目的地址发送数据报
 * @param sockfd: 套接字描述符
 * @param buf: 用于存放写入数据的缓存
 * @param nbytes: 缓存的长度
 * @param flags: 传输标志，0表示默认传输方式
 * @param to: 目的套接字地址结构体指针
 * @param addrlen: 目的地址结构体的长度
 * @return: 成功返回写入的字节数，失败返回-1
 * @note: 1. 无连接：不能保证数据报一定能够到达目的地，也不能保证数据报一定能够按照发送顺序到达目的地；
*/
int recvfrom(int sockfd, void *buf, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen);
/**
 * @brief: 从指定的源地址接收数据报
 * @param sockfd: 套接字描述符
 * @param buf: 用于存放读取数据的缓存
 * @param nbytes: 缓存的最大长度
 * @param flags: 传输标志，0表示默认传输方式
 * @param from: 源套接字地址结构体指针
 * @param addrlen: 源地址结构体的长度   
 * @return: 成功返回读取到的字节数，失败返回-1
*/

//x86_64架构下，字节序为小端序；网络字节序为大端序(直观上看，高位前，低位后)
//字节序转换函数
#include <netinet/in.h>
/*小端转大端，即主机字节序转网络字节序*/
unsigned short htons(unsigned short host16bitvalue);
unsigned long htonl(unsigned long host32bitvalue);
/*大端转小端，即网络字节序转主机字节序*/
unsigned short ntohs(unsigned short net16bitvalue);
unsigned long ntohl(unsigned long net32bitvalue);

//地址转换函数
//点分十进制字符串 和 网络字节序的32位二进制IP地址 之间相互转换
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
/*点分十进制字符串转网络字节序的32位二进制IP地址*/
int inet_aton(const char *strptr, struct in_addr *addrptr);
/*网络字节序的32位二进制IP地址转点分十进制字符串*/
char *inet_ntoa(struct in_addr inaddr);
/**
 * @param strptr: 点分十进制字符串
 * @param addrptr: 用于存放转换后的网络字节序的32位二进制IP地址的结构体指针
 * @param inaddr: 网络字节序的32位二进制IP地址
 * 
 * @note: 1. inet_ntoa()函数返回一个指向点分十进制字符串的指针，该指针指向的字符串是inet_ntoa()函数内部静态分配的，因此不要将该指针作为返回值返回；
 *        2. inet_ntoa()再次调用时，会覆盖上一次调用时的结果，因此不要在多线程环境下使用；
*/

//主机信息函数
#include <netdb.h>
struct hostent {
    char *h_name;       /* 主机的正式名称 */
    char **h_aliases;   /* 主机的别名列表 */
    int h_addrtype;     /* 主机地址类型 */
    int h_length;       /* 主机地址长度 */
    char **h_addr_list; /* 主机地址列表，每个地址都是网络字节序的32位二进制IP地址 */
};
/*保持向后兼容*/
define h_addr h_addr_list[0];
struct hostent *gethostbyname(const char *hostname);
/**
 * @brief: 根据主机名获取主机信息
 * @param hostname: 主机名
 * @return: 成功返回主机信息结构体指针，失败返回NULL
 * @note: 1. 返回指针是静态分配的，因此不要将该指针作为返回值返回；
 * 该函数是线程不安全的，多线程环境下，可以使用gethostbyname_r()函数；
 * 
*/