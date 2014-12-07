#include "server_db.h"

void init_global_db()
{
	memset(&db, 0, sizeof(struct global_db));
}

void db_client_new(struct client_info_data *data)
{
	struct client_info_list_node *node = NULL;
	struct client_info_list_node *temp;
	node = malloc(sizeof(*node));
	node->data = data;
	node->next = NULL;

	if (!db.client_list) {
		db.client_list = node;	
	} else {
		temp = db.client_list;
		db.client_list = node;
		node->next = temp;
	}
}

struct client_info_data *
db_get_client_by_socket(int sock_id)
{

	struct client_info_list_node *node = NULL;
	if (!db.client_list) {
		return NULL;	
	}

	node = db.client_list;
	
	while (node) {
		if (node->data->socket == sock_id) {
			return node->data;
		}
		node = node->next;
	}
	return NULL;
}

void db_client_del(struct client_info_data *data)
{
	struct client_info_list_node *temp, *to_be_del;

	if (!db.client_list) {
		perror("Nothing to delete!");
		return;
	} 

	for (temp = db.client_list; temp->next != NULL;
				temp = temp->next) {
		if (temp->next->data == data) {
			to_be_del = temp->next;
			temp->next = temp->next->next;
			
			free(to_be_del->data);
			free(to_be_del);	
			return;
		}
	}
}


