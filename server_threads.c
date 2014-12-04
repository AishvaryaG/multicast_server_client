#include "server.h"
#include <pthread.h>
#include <unistd.h>

void *periodic_print_thread(void *t_args)
{
		group_info *head = g_head;
		while(1)
		{
				head = g_head;
				sleep(10);
				while(head)
				{
						printf("Client id:%d, message buff:[%s], group:%d \n",
										head->client_sockfd,
										head->client_message,
										head->group_id);
						head = head->next;
				}
		}
		pthread_exit(NULL);
}
