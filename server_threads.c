#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#include "server.h"
#include "server_db.h"


void *periodic_print_thread_fn(void *t_args)
{
	struct tm *tm_info;
	time_t timer;
	char time_buf[MAXDATEBUF];

	struct client_info_list_node *node;

	while(1)
	{
		sleep(10);
		printf("\n\n10 seconds elapsed. Printing clients status\n");

		node = db.client_list;

		time(&timer);
		tm_info = localtime(&timer);
		strftime(time_buf, MAXDATEBUF, "%Y-%m-%d %H:%M:%S", tm_info);
				
		while(node)
		{
			printf("[%s] Client (%s,%d) buffer:[%s]\n",
					time_buf,
					node->data->hostname,
					node->data->port,
					node->data->buffer);
			node = node->next;
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
