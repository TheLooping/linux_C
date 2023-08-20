#include <netinet/in.h>
/**
 * @sin_family: ��ַ��AF_INET����PF_INET 
 * @sin_port: �˿ںţ������������ֽ�˳�����ʽ
 * @sin_addr: IP��ַ��һ��32λ�޷���������in_addr_t���ͣ��������������ֽ�˳�����ʽ
 * @sin_zero: ռλ�ֽڣ��ճ�16�ֽڣ�Ϊ����sockaddr��sockaddr_in�������ݽṹ���ִ�С��ͬ�������Ŀ��ֽ�
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
