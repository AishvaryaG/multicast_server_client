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
	struct group_info_node *group_list;
};



/* -------------Client Mgmt API-------------------*/
/* When a new client joins, with list of groups */
void db_client_new(struct client_info_data *data);
void db_client_del(struct client_info_data *data);
struct client_info_data *db_get_client_by_socket(int sock_id);


/* When a client dies, remove it from global list 
 * and also from other group free&busy lists */


/*  
---------------------------------------
| Group1   					          |
| Members -> [client 1] -> [client 2] |
---------------------------------------
			|
			|
			Y
		Next Group
*/


struct group_info_node {
	int grp_id;
	struct client_info_list_node *members;
	struct group_info_node *next;
};

void db_group_new(int group_id);
int db_group_add_member(int group_id, 
				struct client_info_data *client_info);
struct group_info_node *db_get_group_by_grp_id(int grp_id);


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
