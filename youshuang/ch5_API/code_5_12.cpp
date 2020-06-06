// ����daytime����
#include<sys/socket.h>
#include<netinet/in.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>

int main(int argc, char** argv){
	assert(argc == 2);
	char* host = argv[1];
	// ��ȡĿ��������ַ��Ϣ
	struct hostent* hostinfo = gethostbyname(host);
	assert(hostinfo);
	// ��ȡdaytime������Ϣ
	struct servent* servinfo = getservbyname("daytime", "tcp");
	assert(servinfo);
	printf("daytime port is %d\n", ntohs(servinfo->s_port));
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = servinfo->s_port;
	// �˴�IP��ַ�����������ֽ��򣬲���Ҫ����ת��
	address.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;

	printf("address is %s\n", inet_ntoa(address.sin_addr));

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int result = connect(sockfd, (struct sockaddr*)&address, sizeof(address));	// ��������daytime����������гɹ�
	assert(result != -1);
	
	char buffer[128];
	result = read(sockfd, buffer, sizeof(buffer));
	assert(buffer > 0);
	buffer[result] = '\0';
	printf("the daytime is: %s\n", buffer);
	
	close(sockfd);
	return 0;
}
