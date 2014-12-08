#include "client_pkt_handlers.h"
#include "client.h"

#include <pthread.h>

void 
get_avail_handler(
			const void *payload, const size_t pyld_size, 
			const pkt_type type)
{

	int numbytes;

	if (busy_state) {
		numbytes = pkt_send(sockfd, 
			MSG_CHK_AVAIL_RESP_NOK, NULL, 0);
   	} else {
		numbytes = pkt_send(sockfd, 
			MSG_CHK_AVAIL_RESP_OK, NULL, 0);
	}
	if (-1 == numbytes) {
            perror("send");
    }   

}


void *client_recv_thread_fn(void *t_args)
{
	void *rx_buffer = NULL;
	size_t rx_sz;
	pkt_type type;
	int ret;


	while (1) {
		printf ("recv from server socket");
		ret = pkt_recv (sockfd, &rx_buffer, &rx_sz, &type);
		if (-1 == ret || !rx_buffer) {
			perror ("recv");
		}


		switch (type) {
		case MSG_CHK_AVAIL:
				get_avail_handler(rx_buffer, rx_sz, type);
				pthread_exit(NULL);
		default:
			printf("\nUnexpected message!!!");
		}
		free(rx_buffer);
	}
	pthread_exit(NULL);

}


