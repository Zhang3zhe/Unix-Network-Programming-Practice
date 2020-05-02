#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/stat.h>


#define SERVER_PORT 80


int get_line(int sock, char* buf, int size);
int do_http_request(int client);
int do_http_response(int client_sock, const char* url);
void serve_file(int client, const char* filename); 
// 异常处理
void bad_request(int client_sock);    // 400
void unimplemented(int client_sock);    // 501
void not_found(int client_sock);    // 404

int main(){  
   
    int sock = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_addr; 
   
    bzero(&server_addr, sizeof(server_addr));
   
    server_addr.sin_family = AF_INET;    // 选择协议族 IPv4    
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 监听本地所有IP地址
    server_addr.sin_port = htons(SERVER_PORT);    // 绑定端口号 
    
    // 将IP地址和端口号绑定到socket描述字
    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    listen(sock, 128);

    printf("Waitting for the connection from a client...\n");

    while(1){
        struct sockaddr_in client_addr;
        socklen_t client_addr_len;
        client_addr_len = sizeof(client_addr);

		int client = accept(sock, (struct sockaddr*)&client_addr, &client_addr_len);
		
		char client_ip[64];
		printf("client ip: %s\t port: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip)), ntohs(client_addr.sin_port));
		
		// 处理HTTP请求
		if(do_http_request(client) < 0){
		    close(client);
		    continue;
		}		

//		len = write(client, buf, len);

//		printf("write finished. len: %d\n", len);

		close(client);       
    }

    return 0;
}


int do_http_request(int client){
	// 读取客户端发送的HTTP请求
	char buf[256];
	int len = 0;
	
	// 读取首行
	char method[16];
	char url[256];
	len = get_line(client, buf, sizeof(buf));
	if(len > 0){
	    // 获取方法
	    int i = 0, j = 0;
	    while(!isspace(buf[j]) && i < sizeof(method) - 1){
		method[i++] = buf[j++];
	    }
	    method[i] = '\0';

	    // 判断方法是否合法
	    if(strcasecmp(method, "GET") == 0){
		printf("method = %s\n", method);
	    } else {
		unimplemented(client);
		return -1;
	    }
	    
	    // 跳过空格
	    while(isspace(buf[++j]));
	    i = 0;

	    // 获取url
	    while(!isspace(buf[j]) && i < sizeof(url) - 1){
		url[i++] = buf[j++];
	    }
	    url[i] = '\0';
	    printf("url = %s\n", url);
		
		if(do_http_response(client, url) < 0)	return -1;
		
	    printf("Request from the client: \n");
	    printf("%s\n", buf);
	} else {    // 出错处理
	    bad_request(client);
	    return -1;
	}		

	do{
	    len = get_line(client, buf, sizeof(buf));
	    printf("%s\n", buf);
	} while (len > 0);
	return 0;
}


int do_http_response(int client_sock, const char* url){
    char path[256];
	// index.html => ./htdocs/index.html
	sprintf(path, "./htdocs%s", url);
	if(url[strlen(url)-1] == '/'){
		strcat(path, "index.html");
	}	
	struct stat st;
	if(stat(path, &st) == -1){    // 文件不存在或者读异常
		not_found(client_sock);
		return 0;
	}
	// 判断是否为目录
	if(S_ISDIR(st.st_mode)){
		strcat(path, "/index.html");
	}
	printf("path: %s\n", path);
	serve_file(client_sock, path);

	return 0;
} 


void serve_file(int client_sock, const char* filename){
	FILE* resource = NULL;
	resource = fopen(filename, "r");
	if(resource == NULL){
		not_found(client_sock);
		return;
	} else {
        const char* main_header = "HTTP/1.0 200 OK\r\nServer: Mini-httpd\r\nContent-Type: text/html\r\ncharset=\"zh-CN\"\r\nConnection: Close\r\n\r\n";
		int len = write(client_sock, main_header, strlen(main_header));
		
		//const char* file_content = "<html lang = \"zh-CN\"><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>Index</title><body><h1>好朋友真好！</h1><h1>好朋友已经领先我两年了！</h1><h1>好朋友人家肯定去阿里算法岗！</h1><h1>好朋友以后当广东省委书记！</h1><h1>唉，什么时候才能像好朋友一样牛逼！</h1></body></html>";
		char file_content[1024];
		/*
		char send_buf[64];
		int wc_len = strlen(file_content);		
		snprintf(send_buf, 64, "Content-Length: %d\r\n\r\n", wc_len);
		len = write(client_sock, send_buf, strlen(send_buf));
		*/
		fgets(file_content, sizeof(file_content), resource);
		while(!feof(resource)){
			len = write(client_sock, file_content, strlen(file_content));
			fgets(file_content, sizeof(file_content), resource);
		}
		/*
		char send_buf[64];
		int wc_len = strlen(file_content);		
		snprintf(send_buf, 64, "Content-Length: %d\r\n\r\n", wc_len);
		len = write(client_sock, send_buf, strlen(send_buf));
		len = write(client_sock, file_content, wc_len);
		//printf("%s", welcome_content);		*/
	}
	fclose(resource);
}


int get_line(int sock, char* buf, int size){
   
    int count = 0;
    char ch = '\0';
    int len = 0;
    
    while(count < size - 1 && ch != '\n'){
	len = read(sock, &ch, 1);
	if(len == 1){
	    if(ch == '\r'){
		continue;
	    } else if(ch == '\n'){
		buf[count] = '\0';
		break;
	    } else {
	        buf[count++] = ch;
	    }
	} else if(len == -1){    // 读取出错
	    perror("read failed.");
	    break;
	} else {    // len == 0, 一般是客户端socket关闭
	    fprintf(stderr, "client closed.\n");
	    break;
	}
    } 
    return count;
}


void bad_request(int client_sock){
    const char* reply = "HTTP/1.0 400 BAD REQUEST\r\nContent-Type: text/html\r\n\r\n<HTML><TITLE>BAD REQUEST</TITLE>\r\n<BODY><P>Browser sent a bad request.\r\n</BODY>\r\n</HTML>";
    int len = write(client_sock, reply, strlen(reply));
    if(len <= 0)    fprintf(stderr, "send reply failled. reason: %s\n", strerror(errno));
}


void unimplemented(int client_sock){
	const char* reply = "HTTP/1.0 501 Method Not Implemented\r\nContent-Type: text/html\r\n\r\n<HTML><TITLE>Method Not Implemented</TITLE>\r\n<BODY><P>HTTP request method not supported.\r\n</BODY>\r\n</HTML>";
    int len = write(client_sock, reply, strlen(reply));
    if(len <= 0)    fprintf(stderr, "send reply failled. reason: %s\n", strerror(errno));
}


void not_found(int client_sock){
	const char* reply = "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n<HTML><TITLE>Not Found</TITLE>\r\n<BODY><P>The server could not fulfill your request because the resource specified is unavailable or nonexistent.\r\n</BODY>\r\n</HTML>";
    int len = write(client_sock, reply, strlen(reply));
    if(len <= 0)    fprintf(stderr, "send reply failled. reason: %s\n", strerror(errno));
}
