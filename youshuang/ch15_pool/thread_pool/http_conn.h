// http_conn.h
#ifndef HTTPCONNECTION_H_
#define HTTPCONNECTION_H_

#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<stdarg.h>
#include<errno.h>
#include"locker.h"

class http_conn{
 public:
	static const int FILENAME_LEN = 200;		// 文件名的最大长度
	static const int READ_BUFFER_SIZE = 2048;	// 读缓冲区大小
	static const int WRITE_BUFFER_SIZE = 1024;	// 写缓冲区大小
	// HTTP 请求方法
	enum METHOD {GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATCH};
	// 主状态机的两种可能状态，
	// 分别表示：当前正在分析请求行，当前正在分析头部字段，当前正在分析内容
	enum CHECK_STATE {
		CHECK_STATE_REQUESTLINE = 0,
		CHECK_STATE_HEADER,
		CHECK_STATE_CONTENT
	};
	// 服务器处理HTTP请求的结果
	enum HTTP_CODE {
		NO_REQUEST,				// 请求不完整，需要继续读取客户数据
		GET_REQUEST,			// 表示获得了一个完整的客户请求
		BAD_REQUEST,			// 客户请求有语法错误
		NO_RESOURCE,			// 没有资源
		FORBIDDEN_REQUEST,		// 客户对资源没有足够的访问权限
		FILE_REQUEST,			// 文件请求
		INTERNAL_ERROR,			// 服务器内部错误
		CLOSED_CONNECTION		// 客户端已关闭连接
	};
	// 从状态机的三种可能状态，即行的读取状态，
	// 分别表示：读取到一个完整的行、行出错和行数据尚且不完整
	enum LINE_STATUS {
		LINE_OK = 0,
		LINE_BAD,
		LINE_OPEN
	};
	
 public:
	http_conn(){}
	~http_conn(){}
	
 public:
	void init(int sockfd, const sockaddr_in& addr);	// 初始化新接受的连接
	void close_conn(bool real_close = true);			// 关闭连接
	void process();										// 处理客户请求
	bool read();										// 非阻塞读
	bool write();										// 非阻塞写
	
 private:
	void init();						// 初始化连接
	HTTP_CODE process_read();			// 解析HTTP请求
	bool process_write(HTTP_CODE ret);	// 填充HTTP应答
	
	// 下面这一组函数被process_write调用以分析HTTP请求
	HTTP_CODE parse_request_line(char* text);
	HTTP_CODE parse_headers(char* text);
	HTTP_CODE parse_content(char* text);
	HTTP_CODE do_request();
	char* get_line(){return m_read_buf + m_start_line;}
	LINE_STATUS parse_line();
	
	// 下面这一组函数被process_write调用以填充HTTP应答
	void unmap();
	bool add_response(const char* format, ...);
	bool add_content(const char* content);
	bool add_status_line(int status, const char* title);
	bool add_headers(int content_length);
	bool add_content_length(int content_length);
	bool add_linger();
	bool add_blank_line();
	
 public:
	static int m_epollfd;
	static int m_user_count;
	
 private:
	// 该HTTP连接的socket和对方的socket地址
	int m_sockfd;
	sockaddr_in m_address;
	
	char m_read_buf[READ_BUFFER_SIZE];
	int m_read_idx;							// 标记已读入的客户数据的最后一个字节的下一位置
	int m_checked_idx;						// 当前正在分析的字符在缓冲区中的位置
	int m_start_line;						// 当前正在解析的行的起始位置
	char m_write_buf[WRITE_BUFFER_SIZE];
	int m_write_idx;						// 写缓冲区中待发送的字节数
	
	CHECK_STATE m_check_state;				// 主状态机当前所处的状态
	METHOD m_method;
	
	char m_real_file[FILENAME_LEN];			// 客户请求的目标文件的完整路径，即doc_root + m_url，doc_root是网站根目录
	char* m_url;							// 客户请求的目标文件的文件名
	char* m_version;						// 仅支持HTTP/1.1
	char* m_host;							// 主机名
	int m_content_length;					// HTTP请求的消息体的长度
	bool m_linger;							// HTTP请求是否要求保持连接
	
	char* m_file_address;					// 客户请求的目标文件被mmap到内存中的起始位置
	struct stat m_file_stat;				// 目标文件的状态
	struct iovec m_iv[2];					// 采用writev来执行写操作
	int m_iv_count;							// 被写内存块的数量
};

#endif // HTTPCONNECTION_H_
