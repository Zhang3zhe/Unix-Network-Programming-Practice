// epoll��LT��ETģʽ
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

// ���ļ����������óɷ�������
int setnonblocking(int fd){
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

// ���ļ�������fd�ϵ�EPOLLINע�ᵽepollfdָʾ��epoll�ں��¼�����
// ����enable_etָ���Ƿ��fd����ETģʽ
void addfd(int epollfd, int fd, bool enable_et){
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	if(enable_et){
		event.events |= EPOLLET;
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

// LTģʽ�Ĺ�������
void lt(epoll_event* events, int number, int epollfd, int listenfd){
	char buf[BUFFER_SIZE];
	for(int i = 0; i < number; ++i){
		int sockfd = events[i].data.fd;
		if(sockfd == listenfd){
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof(client_address);
			int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
			addfd(epollfd, connfd, false);	// ��connfd����ETģʽ
		} else if (events[i].events & EPOLLIN){
			// ֻҪsocket�������л���δ���������ݣ���δ���ͱ�����
			printf("event trigger once.\n");
			memset(buf, '\0', BUFFER_SIZE);
			int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
			if(ret <= 0){
				close(sockfd);
				continue;
			}
			printf("get %d bytes of content: %s\n", ret, buf);
		} else {
			printf("something else happened.\n");
		}	
	}
}

//ETģʽ�Ĺ�������
void et(epoll_event* events, int number, int epollfd, int listenfd){
	char buf[BUFFER_SIZE];
	for(int i = 0; i < number; ++i){
		int sockfd = events[i].data.fd;
		if(sockfd == listenfd){
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof(client_address);
			int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
			addfd(epollfd, connfd, true);	// ��connfd����ETģʽ
		} else if(events[i].events & EPOLLIN){
			printf("event trigger once.\n");
			// ��δ��벻�ᱻ�ظ���������������ѭ����ȡ���ݣ�
			// ��ȷ��socket�������е��������ݶ���
			while(1){
				memset(buf, '\0', BUFFER_SIZE);
				int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
				if(ret < 0){
					// ���ڷ�����IO�����������������ʾ�����Ѿ�ȫ����ȡ���
					// �˺�epoll�����ٴγ���sockfd�ϵ�EPOLLIN�¼�����������һ�ζ�����
					if(errno == EAGAIN || errno == EWOULDBLOCK){
						printf("read later.\n");
						break;
					}
					close(sockfd);
					break;
				} else if(ret == 0){
					close(sockfd);
				} else {
					printf("get %d bytes of content: %s\n", ret, buf);
				}
			}
		} else {
			printf("something else happened.\n");
		}
	}
} 

int main(int argc, char** argv){
	if(argc <= 3){
		printf("usage: %s ip_address port_number trigger_mode\n", basename(argv[0]));
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);
	int trigger_mode = atoi(argv[3]);
	
	int ret = 0;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);
	
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);
	
	ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);
	
	ret = listen(listenfd, 5);
	assert(ret != -1);
	
	epoll_event events[MAX_EVENT_NUMBER];
	int epollfd = epoll_create(5);
	assert(epollfd != -1);
	addfd(epollfd, listenfd, true);
	
	while(1){
		int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
		if(ret < 0){
			printf("epoll failed.\n");
			break;
		}
		if(trigger_mode == 0){
			lt(events, ret, epollfd, listenfd);
		} else if(trigger_mode == 1){
			et(events, ret, epollfd, listenfd);
		} else {
			printf("trigger_mode error.\n");
			break;
		}
	}
	
	close(listenfd);
	return 0;
}
