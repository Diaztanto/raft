#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>
#include <pthread.h>
#include <curl/curl.h>

#define MAX_NODE 11
#define SERVER_COUNT 5

#define LEADER 0
#define CANDIDATE 1
#define FOLLOWER 2

#define VOTE_REQ 100

/*
#define VOTE 100 + vote
#define	SET 100 + server number + actual value
*/

typedef struct {
	int* has_read;
	int msg;
} message;

typedef struct {
	int* serverload;
	int flag;
	int has_voted;

	int id;
	CURL *node_curl;
} node;

int node_count;
message msg_pool;

void init_node(node* n, int idx);

void destroy_node(node* n);

void init_nodes(node** n, int count);

void destroy_nodes(node** n, int count);

void* node_main(void* node);

int main (int argc, char* argv[]) {
	pthread_t node_thread[MAX_NODE];
	node* nodes[MAX_NODE];
	int node_stat[MAX_NODE];
	int i;

	//proses argv sebagai init node count
	//skrg pake inisialisasi hard code
	node_count = 5;
	init_nodes(nodes, node_count);

	for (i = 0; i < node_count; i++) {
		printf("%d", i);
		init_node(nodes[i], i);
		node_stat[i] = pthread_create(&node_thread[i], NULL, node_main, (void*) nodes[i]);
		if (node_stat[i]) {
			printf("Error\n");
			exit(-1);
		}
	}

	for (i = 0; i < node_count; i++) {
		destroy_node(nodes[i]);
		pthread_join(node_thread[i], NULL);
	}
	destroy_nodes(nodes, node_count);


	return 0;
}

void init_node(node* n, int idx) {
	n->serverload = (int*) malloc (sizeof(int) * SERVER_COUNT);
	n->flag = FOLLOWER;
	n->id = idx;
	n->node_curl = curl_easy_init();
	n->has_voted = 0;
}

void destroy_node(node* n) {
	free(n->serverload);
	curl_easy_cleanup(n->node_curl);
}

void init_nodes(node** n, int count) {
	int i;
	while (i < count) {
		n[i] = (node*) malloc (sizeof(node));

		i++;
	}
}

void destroy_nodes(node** n, int count) {
	int i;
	while (i < count) {
		free(n[i]);

		i++;
	}
}

void* node_main(void* n) {
	clock_t time_elapsed, start, end;
	int timer;
	int has_recv_vote_req = 0;
	int id;

	id = ((node*)n)->id;

//election
	timer = rand_r(&id) % 1000 + 1;
	printf("election timer: %d\n", timer);

	start = clock();
	end = start;
	//tambahin kondisi kalo nerima vote request
	while ((time_elapsed = (end - start) * 1000 / CLOCKS_PER_SEC) <= timer && has_recv_vote_req == 0) {
		//receive vote request / heartbeat
		printf("%d\n", time_elapsed);
		end = clock();
	}


	if (has_recv_vote_req == 0) {
		((node*)n)->flag = CANDIDATE;
	} else {
		//vote
		//set has vote to 1
	}

	printf("I'm node %d, I'm a %d\n", ((node*)n)->id, ((node*)n)->flag);

/*
	if (node->flag == CANDIDATE) {
		//start election
		//send vote request
		//receive vote
	}
*/

//heartbeat
}