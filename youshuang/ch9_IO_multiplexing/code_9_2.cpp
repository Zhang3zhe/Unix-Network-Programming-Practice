// poll和epoll在使用上的差别

// 如何索引poll返回的文件描述符
int ret = poll(fds, MAX_EVENT_NUMBER, -1);
// 必须遍历所有已注册文件描述符
for(int i = 0; i < MAX_EVENT_NUMBER; ++i){
	if(fds[i].revents & POLLIN){
		int sockfd = fds[i].fd;
		// process...
	}
}

// 如何索引epoll返回的文件描述符
int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
// 仅遍历ret个文件描述符
for(int i = 0; i < ret; ++i){
	int sockfd = events[i].data.fd;
	// process...
}
