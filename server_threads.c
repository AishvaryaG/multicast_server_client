#include "server.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

void *periodic_print_thread_fn(void *t_args)
{
	group_info *head = g_head;
	struct tm *tm_info;
	time_t timer;
	char time_buf[MAXDATEBUF];

	while(1)
	{
		head = g_head;
		sleep(10);

		time(&timer);
		tm_info = localtime(&timer);
		strftime(time_buf, MAXDATEBUF, "%Y-%m-%d %H:%M:%S", tm_info);
				
		while(head)
		{
			printf("[%s] Client id:%d, message buff:[%s], group:%d\n",
					time_buf,
					head->client_sockfd,
					head->client_message,
					head->group_id);
			head = head->next;
		}
	}
	pthread_exit(NULL);
}


void *user_interactor_thread_fn(void *t_args)
{

	while(1)
	{
		sleep(10);
		task_menu();	

	}
	pthread_exit(NULL);
}
