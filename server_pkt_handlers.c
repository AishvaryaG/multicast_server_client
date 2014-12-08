#include "server.h"
#include "server_db.h"
#include "server_pkt_handlers.h"


#include <pthread.h>
#include <netinet/in.h>

void join_handler(struct client_info_data *client_info,
		const void *payload, const size_t pyld_size, const pkt_type type)
{
	struct group_info_node *grp_info;
	const struct msg_join_grp_pld *join_pld = payload;
	int ret;

	int grp_id = ntohs(join_pld->grp_id);
	printf("Client (sock_fd:%d) requests to join group:\"%d\"\n", client_info->socket,
			grp_id);

	grp_info = db_get_group_by_grp_id(grp_id);
	if (!grp_info) {
		printf("Group %d not found! creating...", grp_id);
		db_group_new(grp_id); 
	}

	ret = db_group_add_member(grp_id, client_info);

	if (ret != 0) {
		perror("Error in adding member to group!");
	}
		
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
