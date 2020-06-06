// 9.1.1 select系统调用
#include<sys/select.h>
int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);

// 访问fd_set结构体中的位
#include<sys/select.h>
FD_ZERO(fd_set* fdset);               // 清除fd_set的所有位
FD_SET(int fd, fd_set* fdset);        // 设置fd_set的位fd
FD_CLR(int fd, fd_set* fdset);        // 清除fd_set的位fd
int FD_ISSET(int fd, fd_set* fdset);  // 测试fd_set的位fd是否被设置

// timeval结构体的定义
struct timeval{
    long tv_sec;
    long tv_usec;
};

// 9.2 poll系统调用
#include<poll.h>
int poll(struct pollfd* fds, nfds_t nfds, int timeout);

struct pollfd{
    int fd;          // 文件描述符
    short events;    // 注册的事件
    short revents;   // 实际发生的事件，由内核填充
};

// 9.3.1 内核事件表
// epoll需要一个额外的文件描述符来唯一标识内核中的事件表，
// 使用epoll_create创建
#include<sys/epoll.h>
int epoll_create(int size);

// 操作epoll的内核事件表
#include<sys/epoll.h>
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);

struct epoll_event{
    __uint32_t events;	// epoll事件
    epoll_data_t data;	// 用户数据
};

typedef union epoll_data{
    void* ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

// epoll_wait函数在一段超时时间内等待一组文件描述符上的事件
#include<sys/epoll.h>
int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);



