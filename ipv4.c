#include <netinet/in.h>
/**
 * @sin_family: 地址族AF_INET或者PF_INET 
 * @sin_port: 端口号，必须是网络字节顺序的形式
 * @sin_addr: IP地址，一个32位无符号整数（in_addr_t类型），必须是网络字节顺序的形式
 * @sin_zero: 占位字节，凑成16字节，为了让sockaddr与sockaddr_in两个数据结构保持大小相同而保留的空字节
**/
struct sockaddr_in {
    sa_family_t    sin_family; /* address family: AF_INET */
    in_port_t      sin_port;   /* port in network byte order */
    struct in_addr sin_addr;   /* IP address */
    unsigned char sin_zero[8]; /* pad to sizeof(struct sockaddr) */
};
struct in_addr {
    uint32_t       s_addr;     /* IP address */
};
