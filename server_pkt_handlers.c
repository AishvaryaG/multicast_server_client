#include "server.h"
#include "server_db.h"
#include "server_pkt_handlers.h"


#include <pthread.h>
#include <netinet/in.h>


void join_handler(struct client_info_data *client_info,
		const void *payload, const size_t pyld_size, const pkt_type type)
{
	const struct msg_join_grp_pld *join_pld = payload;
	int grp = ntohs(join_pld->grp_id);
	printf("Client (sock_fd:%d) requests to join group:\"%d\"\n", client_info->socket,
			grp);
}
void hello_handler(struct client_info_data *client_info,
		const void *payload, const size_t pyld_size, const pkt_type type)
{
	strncat(client_info->buffer, payload, pyld_size);
	strcat(client_info->buffer, ", ");
}
void quit_handler(struct client_info_data *client_info)
{
	pthread_exit(NULL);
}
