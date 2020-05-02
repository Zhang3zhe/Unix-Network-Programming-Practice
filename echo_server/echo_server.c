#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<ctype.h>
#include<arpa/inet.h>

#define SERVER_PORT 666

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
		
		// 读取客户端发送的数据
		char buf[256];
		int len = read(client, buf, sizeof(buf)-1 );
		
		if(len < 0){
			fputs("read error!\n", stderr);
			exit(1);
		}
		
		printf("read finished. len: %d\n", len);
		buf[len] = '\0';	
		printf("recive: %s\n", buf);

		// 转换为大写字母
		int i = 0;
		for(i = 0; i < len; ++i){
			buf[i] -= (buf[i] >= 'a' && buf[i] <= 'z' ? 32 : 0);
		}

		len = write(client, buf, len);

		printf("write finished. len: %d\n", len);

		close(client);       
    }

    return 0;
}
