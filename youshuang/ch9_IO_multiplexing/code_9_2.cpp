// poll��epoll��ʹ���ϵĲ��

// �������poll���ص��ļ�������
int ret = poll(fds, MAX_EVENT_NUMBER, -1);
// �������������ע���ļ�������
for(int i = 0; i < MAX_EVENT_NUMBER; ++i){
	if(fds[i].revents & POLLIN){
		int sockfd = fds[i].fd;
		// process...
	}
}

// �������epoll���ص��ļ�������
int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
// ������ret���ļ�������
for(int i = 0; i < ret; ++i){
	int sockfd = events[i].data.fd;
	// process...
}
