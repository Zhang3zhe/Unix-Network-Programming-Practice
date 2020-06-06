// 5.1.1
// Linux 提供了4个函数来完成主机字节序和网络字节序的转换
#include<netinet/in.h>
unsigned long int htonl(unsigned long int hostlong);
unsigned short int htons(unsigned short int hostshort);
unsigned long int ntohl(unsigned long int netlong);
unsigned short int ntohs(unsigned short int netshort);

// 通用socket地址 sockaddr
#include<bits/socket.h>
struct sockaddr{
	sa_family_t sa_family;
	char sa_data[14];
};

// Linux定义的新的socket地址结构体
#include<bits/socket.h>
struct sockaddr_storage{
	sa_family_t sa_family;
	unsigned long int __ss_align;
	char __ss_padding[128 - sizeof(__ss_align)];
};

// Unix本地协议域专用socket地址结构体
#include<sys/un.h>
struct sockaddr_un{
	sa_family_t sa_family;	// 地址族：AF_UNIX
	char sun_path[108];		// 文件路径名
};

// 用于IPv4和IPv6的socket地址结构体
struct sockaddr_in{
	sa_family_t sa_family;  // AF_INET
	u_int16_t sin_port;	   // 端口号，要用网络字节序表示
	struct in_addr sin_addr;
};

struct in_addr{
	u_int32_t s_addr;	// IPv4地址，要用网络字节序表示
};

struct sockaddr_in6{
	sa_family_t sa_family;	// AF_INET6
	u_int16_t sin6_port;
	u_int32_t sin6_flowinfo;
	struct in6_addr sin6_addr;
	u_int32_t sin6_scope_id;
};

struct int6_addr{
	unsigned char sa_addr[16];	// IPv6地址，要用网络字节序表示
};

// IP地址转换函数 点分十进制<-->网络字节序
#include<arpa/inet.h>
in_addr_t inet_addr(const char* strptr);
int inet_aton(const char* cp, struct in_addr* inp);
char* inet_ntoa(struct in_addr in);

// 同时适用于IPv4和IPv6的地址转换函数
#include<arpa/inet.h>
int inet_pton(int af, const char* src, void* dst);
const char* inet_ntop(int af, const void* src, char* dst, socklen_t cnt);

// 5.2 创建socket
#include<sys/types.h>
#include<sys/socket.h>
int socket(int domain, int type, int protocol);	//AF_INET, SOCK_STREAM / SOCK_UGRAM, 0
 
// 5.3 命名socket
#include<sys/types.h>
#include<sys/socket.h>
int bind(int sock_fd, const struct sockaddr* my_addr, socklen_t addrlen);

// 5.4 监听socket
#include<sys/socket.h>
int listen(int sockfd, int backlog);

// 5.5 接受连接
#include<sys/types.h>
#include<sys/socket.h>
int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);

// 5.6 发起连接
#include<sys/types.h>
#include<sys/socket.h>
int connect(int sockfd, const struct sockaddr* serv_addr, socklen_t addrlen);

// 5.7 关闭连接
#include<unistd.h>
int close(int fd);

// 立即终止连接
#include<sys/socket.h>
int shutdown(int sockfd, int howto);

// TCP数据读写
#include<sys/types.h>
#include<sys/socket.h>
ssize_t recv(int sockfd, void* buf, size_t len, int flags);
ssize_t send(int sockfd, const void* buf, size_t len, int flags);

// UDP数据读写
#include<sys/types.h>
#include<sys/socket.h>
ssize_t recvform(int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen);
ssize_t sento(int sockfd, const void* buf, size_t len, int flags, const struct aockaddr* dest_addr, socklen_t addrlen);

// 通用数据读写函数
// 既能用于TCP也能用于UDP
#include<sys/socket.h>
ssize_t recvmsg(int sockfd, struct msghdr* msg, int flags);
ssize_t sendmsg(int sockfd, struct msghdr* msg, int flags);

struct msghdr{
    void* msg_name;    // socket地址
    socklen_t msg_namelen;    // socket地址的长度
    struct iovec* msg_iov;    // 分散的内存块
    int msg_iovlen;    // 分散内存块的数量
    void* msg_control;    // 指向辅助数据的起始位置
    socklen_t msg_controllen;    // 辅助数据的大小
    int msg_flags;    // 复制函数中的flags参数，并在调用过程中更新
};

struct iovec{
    void* iov_base;    // 内存起始地址
    size_t iov_len;
};

// 5.9 带外标记
#include<sys/socket.h>
int sockmark(int sockfd);

// 5.10 地址信息函数
// 获取连接socket的本端socket地址与对端socket地址
#include<sys/socket.h>
int getsockname(int sockfd, struct sockaddr* address, socklen_t* address_len);
int getpeername(int sockfd, struct sockaddr* address, socklen_t* address_len);

// 5.11 socket选项
// 读取和设置socket文件描述符属性的函数
#include<sys/socket.h>
int getsockopt(int sockfd, int level , int option_name, void* option_value, socklen_t* restrict, socklen_t option_len);
int setsockopt(int sockfd, int level, int option_name, const void* option_value, socklen_t option_len);

// 网络信息API
// 根据主机名称或IP地址获取主机的完整信息
#include<netdb.h>
struct hostent* gethostbyname(const char* name);
struct hostent* gethostbyaddr(const void* addr, size_t len, int type);

struct hostent{
    char* hname;         // 主机名
    char** h_aliases;    // 主机别名，可能有多个
    int h_addrtype;      // 地址族
    int h_length;        // 地址长度
    char** h_addr_list;  // 按网络字节序列出的主机IP地址列表
};

// 根据名称或端口号获取服务的完整信息
#include<netdb.h>
struct servent* getservbyname(const char* name, const char* proto);
struct servent* getservbyport(int port, const char* proto);

