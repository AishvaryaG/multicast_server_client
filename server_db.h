#ifndef _SERVER_DB_H
#define _SERVER_DB_H


#include <time.h>
#include "server.h" 

/* Global data structure */
extern struct global_db db; 

void init_global_db();

/*  
--------------------    --------------------
| Client 1 info ptr| -> | Client 2 info ptr |
--------------------    --------------------
*/

struct client_info_data {
	char info[MAXDESC];
	int socket;
	char hostname[MAXHOSTNAME];
	int port; 
	int pkt_count;
	char buffer[MAXBUFFER];
	/* TODO: Add a list of pointers to group it belongs to. 
		so that we don't have to go through all groups in order to 
		delete a node */
};

struct client_info_list_node {
	struct client_info_data *data;
	struct client_info_list_node *next;
};

struct global_db {
	struct client_info_list_node *client_list;
};



/* -------------Client Mgmt API-------------------*/
/* When a new client joins, with list of groups */
void db_client_new(struct client_info_data *data);
void db_client_del(struct client_info_data *data);


struct client_info_data *db_get_client_by_socket(int sock_id);


/* When a client moves from free list to busy list */
int db_client_make_busy(struct client_info_data *data, int grp_id);
int db_client_make_free(struct client_info_data *data, int grp_id);

/* When a client dies, remove it from global list 
 * and also from other group free&busy lists */

/*  
---------------------------------
| Group1   					 |
| Task-A   					 |
| free_clients -> [cl1]->[cl2]-# |
| busy_clients -> [cl3]->[cl4]-# |
---------------------------------
			|
			|
			Y
		Next Group
*/


struct group_info {
	int group_id;
	char desc[MAXDESC];
	task_type_t task_type;
	struct client_info_list_node *free_clients;
	struct client_info_list_node *busy_clients;
};


/* Task management */
struct running_task_info {
	int task_id;
	task_type_t task_type;
	char desc[MAXDESC];
	struct timeval start_time;
	struct timeval end_time;
	void *input_dataset;
};


#endif
