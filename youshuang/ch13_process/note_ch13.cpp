// 13.1 fork系统调用
#include<sys/types.h>
#include<unistd.h>
pid_t fork(void);

// 13.2 exec系列系统调用
#include<unistd.h>
extern char** environ;

// 以下简写，省略形参
int execl();
int execlp();
int execle();
int execv();
int execve();
int execvp();

// 13.3处理僵尸进程 
#include<sys/types.h>
#include<sys/wait.h>
pid_t wait(int* stat_loc);
pid_t waitpid(pid_t pid, int* stat_loc, int options);

// code_13_1 SIGCHLD信号的典型处理函数
static void handle_child(int sig){
    pid_t pid;
    int stat;
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0){
        // 对结束的子进程进行善后处理
    }
}

// 13.5 信号量
// 13.5.2 semget系统调用
// 用于创建一个新的信号量集，或者获取一个已经存在的信号量集
#include<sys/sem.h>
int semget(key_t key, int num_sems, int sem_flags);

// 13.5.3 semop系统调用
// 执行PV操作
#include<sys/sem.h>
int semop(int sem_id, struct sembuf* sem_ops, size_t num_sem_ops);

// 13.5.4 semctl系统调用
#include<sys/sem.h>
int semctl(int sem_id, int sem_num, int command, ...);

// 13.6 共享内存
// 13.6.1 shmget系统调用
#include<sys/shm.h>
int shmget(key_t key, size_t size, int shmflg);

// 13.6.2 shmat和shmdt系统调用
#include<sys/shm.h>
void* shmat(int shm_id, const void* shm_addr, int shmflg);
int shmdt(const void* shm_addr);

// 13.6.3 shmctl系统调用
#include<sys/shm.h>
int shmctl(int shm_id, int command, struct shmid_ds* buf);

// 13.6.4 共享内存的POSIX方法
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
int shm_open(const char* name, int oflag, mode_t mode);
int shm_unlink(const char* name);

// 13.7 消息队列
#include<sys/msg.h>
int msgget(key_t key, int msgflg);
int msgsnd(int msgid, const void* msg_ptr, size_t msg_sz, int msgflg);
int msgrcv(int msgid, void* msg_ptr, size_t msg_sz, long int msgtype, int msgflg);
int msgctl(int msgid, int command, struct msgid_ds* buf);


