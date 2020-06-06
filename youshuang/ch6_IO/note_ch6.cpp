// 6.1 管道
// pipe函数可用于创建一个管道，以实现进程间通信
#include<unistd.h>
int pipe(int fd[2]);

// socketpair函数用于创建只能在本地使用的双向管道
#include<sys/types.h>
#include<sys/socket.h>
int socketpair(int domain, int type, int protocol, int fd[2]);

//6.2 复制文件描述符的函数
#include<unistd.h>
int dup(int file_descriptor);
int dup2(int file_descriptor_one, int file_descriptor_two);

// 6.3 分散读readv, 集中读writev
#include<sys/uio.h>
ssize_t readv(int fd, const struct iovec* vector, int count);
ssize_t writev(int fd, const struct iovec* vector, int count);

// 6.4 sendfile函数直接在两个文件描述符之间传递数据
#include<sys/sendfile.h>
ssize_t sendfile(int out_fd, int in_fd, off_t* offset, size_t count);

// 共享内存的申请和释放
// mmap函数和munmap函数
void* mmap(void* start, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void* start, size_t length);

// 6.6 splice函数用于两个文件描述符之间移动数据，其中一个为管道描述符
#include<fcntl.h>
ssize_t splice(int fd_in, loff_t* off_in, int fd_out, loff_t* off_out, size_t len, unsigned int flags);

// 6.7 tee函数用于两个管道描述符之间移动数据
#include<fcntl.h>
ssize_t tee(int fd_in, int fd_out, size_t len, unsigned int flags);

// 6.8 fcntl函数用于对文件描述符的各种控制操作
#include<fcntl.h>
int fcntl(int fd, int cmd, ...);


