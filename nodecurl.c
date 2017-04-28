#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>
#include <curl/curl.h>

#define LDR 0
#define CDT 1
#define FLWR 2

#define ADDR_SIZE 50

char* ldr_addr;
char** node_addr;

int* serverload;
int server_count;
int node_count;

int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms);

int main (int argc, char* argv[]) {
	CURLM* curl_m;
	CURL* curl_daemon;
	CURL** curl;
	int port;

	int timeout, timer;
	clock_t start, end;

	int role;

	int program_status;
	int i;

//init
	//proses argumen program
	//format: ./node portdaemon

	//curl
	//init curl ke daemon
	curl_daemon = curl_easy_init();
	if (curl_daemon) {
		printf("Error initializing curl to daemon.\n");
		return 1;
	}

	//minta alamat semua node

	//init multi curl
	curl_m = curl_multi_init();
	if (curl_m) {
		printf("Error initializing curlm.\n");
		return 1;
	}

	//init semua curl
	//init memori CURL**
	curl = (CURL**) malloc (sizeof(CURL*));
	
	for(i = 0; i < node_count; i++) {
		//init easy
		curl[i] = curl_easy_init();
		if (curl[i]) {
			printf("Error initializing curl #%d.\n", i);
			return 1;
		}

		//konek ke alamat
		//insert code here
	}

	//init array
	serverload = (int*) malloc (sizeof(int) * server_count);
	ldr_addr = (char*) malloc (sizeof(char) * ADDR_SIZE);
	node_addr = (char**) malloc (sizeof(char*) * node_count);

	program_status = 1;

//main loop
	//start timer
	start = clock();
	end = start;
	while (program_status == 1) {
		//timer going on
		//ada kemungkinan taro pthread di sini
		//jalanin wait_on_socket bareng2
		//buat dengerin semua node secara simultan
		//insert code here

		//langsung join begitu terima something
		//insert code here

		//what is life?

		//insert kondisi node keluar
	}

//bersih-bersih
	free(serverload);
	free(ldr_addr);

	//free tiap node address
	//insert code here

	//free the whole node address
	free(node_addr);

	curl_easy_cleanup(curl);

	return 0;
}

int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
	struct timeval tv;
	fd_set infd, outfd, errfd;
	int res;

	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec= (timeout_ms % 1000) * 1000;

	FD_ZERO(&infd);
	FD_ZERO(&outfd);
	FD_ZERO(&errfd);

	FD_SET(sockfd, &errfd); /* always check for error */ 

	if(for_recv) {
		FD_SET(sockfd, &infd);
	}
	else {
		FD_SET(sockfd, &outfd);
	}

	/* select() returns the number of signalled sockets or -1 */ 
	res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
	return res;
}