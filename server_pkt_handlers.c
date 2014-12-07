#include "server.h"
#include "server_db.h"
#include "server_pkt_handlers.h"

#include <pthread.h>


void join_handler(struct client_info_data *client_info,
		const char *payload, const size_t pyld_size, const pkt_type type)
{

	printf("TODO:Client joined! Add Groups!!!\n");
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
void hello_handler(struct client_info_data *client_info,
		const char *payload, const size_t pyld_size, const pkt_type type)
{
	strncat(client_info->buffer, payload, pyld_size);
	strcat(client_info->buffer, ", ");
}
void quit_handler(struct client_info_data *client_info)
{
	pthread_exit(NULL);
}
