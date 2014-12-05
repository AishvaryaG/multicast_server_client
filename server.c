#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#include "common_hdr.h"
#include "server.h"


/* global variables */
group_info *g_head = null;
int num_of_clients[2] = { 0, 0 };
pthread_t periodic_thread;
int server_sockfd;

void list_add (group_info ** head, group_info ** new_node);
static void init_periodic_print();
static int init_server_socket(int argc, char *argv[]);

static void new_connection(fd_set *master_fds, int *fdmax);
static void handle_client_recv(fd_set *master, int sock_id);

int
main (int argc, char *argv[])
{

    int i = 0;
    fd_set master;
    fd_set read_fds;
    int fdmax;
    int ret;

    /* thread which will run periodically and print the output */

    init_periodic_print();
    ret = init_server_socket(argc, argv);
    if (0 != ret) {
        fprintf(stderr, "unable to create socket!\n");
        exit(1);    
    }

    fd_zero (&master);
    fd_zero (&read_fds);
    // add the listener to the master set
    fd_set (server_sockfd, &master);
    fdmax = server_sockfd;
    g_head = null;

    while (1)
    {
        read_fds = master;
        if (select (fdmax + 1, &read_fds, null, null, null) == -1)
        {
            perror ("select");
            exit (4);
        }
        for (i = 0; i <= fdmax; i++) {
            if (fd_isset (i, &read_fds)) {
                if (i == server_sockfd) {
                    new_connection(&master, &fdmax);
                } else {
                    handle_client_recv(&master, i);
                }
            }            // end got new incoming connection
        }
    }
    return 0;
}

void
list_add (group_info ** head, group_info ** new_node)
{
    group_info *temp;
    if (*head == null)
    {
        *head = *new_node;
        return;
    }

    /* add at head */
    temp = *head;
    (*new_node)->next = temp;
    *head = *new_node;
}

static void 
init_periodic_print()
{
    pthread_create (&periodic_thread, null, periodic_print_thread,
            null);
}

static void 
handle_client_recv(fd_set *master, int sock_id)
{
    char *rx_buffer = null;
    size_t rx_sz;
    pkt_type type;
    int ret;

    printf ("recv from the client socket: %d\n", sock_id);
    ret = pkt_recv (sock_id, &rx_buffer, &rx_sz, &type);
    if (-1 == ret || !rx_buffer) {
        perror ("recv");
        close (sock_id);
        fd_clr (sock_id, master);    // remove from master set
    } else {

        /* if recv() returns -1 then some error occured while receiving,
         * if returns 0 client has disconnected,
         * else it will returns the number of bytes received
         */
        if (-1 == ret || !rx_buffer)
        {
            perror ("recv");
            close (sock_id);
            return;
        }
        //printf("one packet received!. pkt type: %d payload size: %zu\n", type, rx_sz);

        switch (type)
        {
            case msg_join:
                join_handler(rx_buffer, rx_sz, type);
                break;

            case msg_hello:
                hello_handler(rx_buffer, rx_sz, type);
                break;

            case msg_quit:
                quit_handler(); 
                break;

            default:
                printf ("unknown packet!!!!");
        }
        printf ("buffer = %s\n", rx_buffer);
        free (rx_buffer);
    }
}


static int 
init_server_socket(int argc, char *argv[])
{
    struct sockaddr_in server;
    if (argc != 2) {
        fprintf (stderr, "usage: %s <port-num>\n", argv[0]);
        return -1;
    }

    int server_port_num = atoi (argv[1]);
    server_sockfd = socket (af_inet, sock_stream, 0);
    if (server_sockfd < 0)
    {
        perror ("server: socket");
        return -1;
    }

    server.sin_family = af_inet;
    server.sin_addr.s_addr = inaddr_any;
    server.sin_port = htons (server_port_num);
    memset (&(server.sin_zero), 0, sizeof (server.sin_zero));

    if (-1 == bind (server_sockfd, (struct sockaddr *) &server,
                sizeof (struct sockaddr)))
    {
        close (server_sockfd);
        perror ("server: bind");
        return -1;
    }

    /* listen */
    if (-1 == listen (server_sockfd, max_num_of_clients))
    {
        perror ("listen");
        return -1;    
    }

    printf ("server: waiting for connections... \n");
    return 0;
}


static void 
new_connection(fd_set *master_fds, int *fdmax)
{
    int newfd;
    struct sockaddr_in client_addr;
    group_info *new_node = g_head;
    socklen_t sin_size;

    sin_size = sizeof (client_addr);
    newfd =    accept (server_sockfd, (struct sockaddr *) &client_addr,
            &sin_size);
    if (newfd == -1)
    {
        perror ("accept");
        return;
    }
    else
    {
        fd_set (newfd, master_fds);    // add to master set
        if (newfd > *fdmax) {
            /* update the max fd */
            *fdmax = newfd;
        }
        new_node = (group_info *) malloc (sizeof (group_info));
        new_node->client_sockfd = newfd;
        new_node->client_message[0] = '\0';
        new_node->group_id = 0;
        list_add (&g_head, &new_node);
        printf ("\ngot a connection from (%s , %d)\n",
                inet_ntoa (client_addr.sin_addr),
                ntohs (client_addr.sin_port));
    }
}
