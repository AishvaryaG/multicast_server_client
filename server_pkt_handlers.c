#include "server.h"
#include <pthread.h>
void join_handler(
		const char *payload, const size_t pyld_size, const pkt_type type)
{

	printf("TODO:Client joined!\n");
#if 0
	printf("Client (fd: %d) requests to join group:\"%s\"\n", args->client_sockfd,
			payload);
	/* Apply the lock for num_of_clients*/
	if (payload[0] == '1')
	{
		num_of_clients[0]++;
		args->client_info->group_id = 1;
		printf("No. of clients in group %c : %d\n",payload[0],num_of_clients[0]);
	}
	if (payload[0] == '2')
	{
		num_of_clients[1]++;
		args->client_info->group_id = 2;
		printf("No. of clients in group %c : %d\n",payload[0],num_of_clients[1]);
	}
#endif
}
void hello_handler(
		const char *payload, const size_t pyld_size, const pkt_type type)
{
	printf("TODO:Hello received!\n");
#if 0
	printf("Client (fd: %d) sent a message...\n", args->client_sockfd);
	strncat(args->client_info->client_message,payload, pyld_size);
	strcat(args->client_info->client_message, ", ");
	printf("exiting \n");
#endif
}
void quit_handler()
{
	pthread_exit(NULL);
}
