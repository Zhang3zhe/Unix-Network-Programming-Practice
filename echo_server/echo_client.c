#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define SERVER_PORT 666
#define SERVER_IP "127.0.0.1"

int main(int argc, char** argv){

    if(argc !=2 ){
	fputs("Usage: ./client message \n", stderr);
	exit(1);
    }
       
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(SERVER_PORT);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	char* message = argv[1];
    write(sockfd, message, strlen(message));
    
    char buf[256];
    int n = read(sockfd, buf, sizeof(buf)-1);
    
    if(n > 0){
		buf[n] = '\0';
		printf("receive: %s\n", buf);
    } else {
		perror("error!");
    }

    printf("finished.\n");

    close(sockfd);

    return 0;
}
