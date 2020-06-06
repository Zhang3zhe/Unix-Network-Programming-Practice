// ʹ��Libevent�Ķ�ʱ������
#include<iostream>
#include<event.h>

using namespace std;

// ��ʱ���¼��ص�����
void OnTime(int sock, short event, void* arg){
	cout << "Game over!" << endl;
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	// ������Ӷ�ʱ�¼�����ʱ�¼�������Ĭ���Զ�ɾ����
	event_add((struct event*)arg, &tv);
}

int main(int argc, char** argv){
	// ��ʼ��
	event_init();
	
	struct event evTimer;
	// ���ö�ʱ�¼�
	evtimer_set(&evTimer, OnTime, &evTimer);
	
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	// ��Ӷ�ʱ�¼�
	event_add(&evTimer, &tv);
	
	// �¼�ѭ��
	event_dispatch();
	
	return 0;
}
