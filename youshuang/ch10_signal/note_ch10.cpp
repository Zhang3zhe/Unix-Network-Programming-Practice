// 10.1.1 发送信号
#include<sys/types.h>
#include<signal.h>
// 该函数把信号sig发送给目标进程，目标进程由pid参数指定
int kill(pit_t pid, int sig);

// 10.1.2 信号处理方式
#include<signal.h>
typedef void (*__sighandler_t) (int);

// 忽略目标信号与信号的默认处理方式
#include<bits/signum.h>
#define SIG_DEF ((__sighandler_t) 0)
#define SIG_IGN ((__sighandler_t) 1)

// 10.2.1 signal系统调用
#include<signal.h>
_sighandler_t signal(int sig, _sighandler_t _handler);

// 设置信号处理函数的更为健壮的接口
#include<signal.h>
int sigaction(int sig, const struct sigaction* act, struct sigaction* oact);

struct sigaction{
    _sighandler_t sa_handler;
    _sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer) (void);
};

// 信号集函数
#include<bits/sigset.h>
#define _SIGSET_NWORDS (1024 / (8 * sizeof(unsigned long int)))
typedef struct{
    unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t;

#define <signal.h>
int sigemptyset(sigset_t* _set);		// 清空信号集
int sigfillset(sigset_t* _set);			// 在信号集中设置所有信号
int sigaddset(sigset_t* _set, int _signo);	// 将_signo添加至信号集中
int sigdelset(sigset_t _set, int _signo);	// 将信号_signo从信号集中删除
int sigismember(_const sigset_t* _set, int _signo);  // 测试_signo是否在信号集中

// 10.3.2 进程信号掩码
#include<signal.h>
int sigprocmask(int _how, _const sigset_t* _set, sigset_t* _oset);

// 10.3.3 获取被挂起的信号集
#include<signal.h>
int sigpending(sigset_t* set);





