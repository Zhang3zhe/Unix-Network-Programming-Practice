// 升序定时器链表
#ifndef LST_TIMER
#define LST_TIMER

#include<time.h>
#include<stdio.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 64

class util_timer;	// 前向声明

struct client_data{
	sockaddr_in address;
	int sockfd;
	char buf[BUFFER_SIZE];
	util_timer* timer;
};

// 定时器类
class util_timer{
 public:
	util_timer() : prev(NULL), next(NULL){}
	
 public:
	time_t expire;	  					// 任务超时时间，这里使用绝对时间
	void (*cb_func) (client_data*);	    // 任务回调函数
	client_data* user_data;
	util_timer* prev;
	util_timer* next;
};

// 定时器链表
class sort_timer_lst{
 public:
	sort_timer_lst() : head(NULL), tail(NULL){}
	~sort_timer_lst();						// 销毁链表，删除所有定时器 
	void add_timer(util_timer* timer);	  	// 将目标定时器timer添加到链表中
	void adjust_timer(util_timer* timer);	// 定时任务发生变化时，调整对应的定时器在链表中的位置
	void del_timer(util_timer* timer);		// 将目标定时器timer从链表中删除
	void tick();							// 处理链表上到期的任务
	
 private:
	void add_timer(util_timer* timer, util_timer* lst_head);	// 一个重载的辅助函数，将timer添加到lst_header之后的部分链表中
	
 private:
	util_timer* head;
	util_timer* tail;
};

sort_timer_lst::~sort_timer_lst(){
	util_timer* tmp = head;
	while(tmp){
		head = tmp->next;
		delete tmp;
		tmp = head;
	}
}

void sort_timer_lst::add_timer(util_timer* timer){
	if(timer == NULL)	return;
	if(head == NULL){
		head = tail = timer;
		return;
	}
	if(timer->expire < head->expire){
		timer->next = head;
		head->prev = timer;
		head = timer;
		return;
	}
	add_timer(timer, head);
}

void sort_timer_lst::adjust_timer(util_timer* timer){
	if(timer == NULL)	return;
	util_timer* tmp = timer->next;
	if(tmp == NULL || timer->expire < tmp->expire)	  return;
	if(timer == head){
		head = head->next;
		head->prev = NULL;
		timer->next = NULL;
		add_timer(timer, head);
	} else {
		timer->prev->next = timer->next;
		timer->next->prev = timer->prev;
		add_timer(timer, timer->next);
	}
}

void sort_timer_lst::del_timer(util_timer* timer){
	if(timer == NULL)	return;
	if(timer == head && timer == tail){
		delete timer;
		head = NULL;
		tail = NULL;
		return;
	}
	if(timer == head){
		head = head->next;
		head->prev = NULL;
		delete timer;
		return;
	}
	if(timer == tail){
		tail = tail->prev;
		tail->next = NULL;
		delete timer;
		return;
	}
	timer->prev->next = timer->next;
	timer->next->prev = timer->prev;
	delete timer;
}

void sort_timer_lst::tick(){
	if(head == NULL)	return;
	printf("timer tick.\n");
	time_t cur = time(NULL);	// 获得系统当前时间
	util_timer* tmp = head;
	while(tmp){
		if(cur < tmp->expire)	break;
		tmp->cb_func(tmp->user_data);
		head = tmp->next;
		if(head != NULL)	head->prev = NULL;
		delete tmp;
		tmp = head;
	}
}

void sort_timer_lst::add_timer(util_timer* timer, util_timer* lst_head){
	util_timer* prev = lst_head;
	util_timer* tmp = prev->next;
	while(tmp){
		if(timer->expire < tmp->expire){
			prev->next = timer;
			timer->next = tmp;
			tmp->prev = timer;
			timer->prev = prev;
			break;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL){
		prev->next = timer;
		timer->prev = prev;
		timer->next = NULL;
		tail = timer;
	}
}

#endif // LST_TIMER
