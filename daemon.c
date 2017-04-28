#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>

#define server_timeout 5000

int godlike;

int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms);

void* server_update(void* undecided);

size_t processResponse(void* ptr, size_t size, size_t nmemb, void* stream);

int main () {
	CURL *curl;
	CURLcode res;
	int intLength;
 	int NthPrime;
 	int thread_stat;
 	void* ActualPrime;
 	void* not_decided;
 	char* URL;
 	pthread_t thread_serv_upd;

//pisah di sini
 	thread_stat = pthread_create(&thread_serv_upd, NULL, server_update, not_decided);
 	if (thread_stat) {
 		printf(stderr,"Error - pthread_create() return code: %d\n", thread_stat);
        exit(EXIT_FAILURE);
 	}

//request client
 	//request client dianggap masukan user
	printf("Bilangan prima ke-: ");
	scanf("%d", &NthPrime);

	intLength = (floor(log10(abs(NthPrime)))) + 1;

	URL = (char*)malloc(sizeof(char) * (strlen("http://localhost:13337/") + intLength + 1));
	
//kontak node
	//insert code here


//dapet respon
	//insert code here

//kontak server
	sprintf(URL, "http://localhost:13337/%d", NthPrime);
	//yg di atas ini ntar alamatnya diganti dari hasil konsensus

	curl = curl_easy_init();
	if (curl) {
	    curl_easy_setopt(curl, CURLOPT_URL, URL);
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, processResponse);	 
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, ActualPrime);

	    res = curl_easy_perform(curl);
	    if(res != CURLE_OK) {
	    	fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	    }
	 
		curl_easy_cleanup(curl);
	}	
	printf("respon: %d\n", godlike);	
	    
	free(URL);
	
	pthread_join(thread_serv_upd, NULL);

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

void* server_update(void* undecided) {
//tunggu update
	//insert code here
	CURL *upd_curl;
	CURLcode resu;
	curl_socket_t sock_curl;

	upd_curl = curl_easy_init();
	if (upd_curl) {
		curl_easy_setopt(upd_curl, CURLOPT_URL, "http://localhost:13337/");
		curl_easy_setopt(upd_curl, CURLOPT_CONNECT_ONLY, 1L);

	    resu = curl_easy_perform(upd_curl);
	 
	    if(resu != CURLE_OK) {
	      printf("Error: %s\n", curl_easy_strerror(resu));
	      exit (-1);
	    }

	    resu = curl_easy_getinfo(upd_curl, CURLINFO_ACTIVESOCKET, &sock_curl);

//starts reading
	    char buf[1024];
		size_t nread;
		do {
			nread = 0;
			resu = curl_easy_recv(upd_curl, buf, sizeof(buf), &nread);

			if(resu == CURLE_AGAIN && !wait_on_socket(sock_curl, 1, 3000L)) {
				printf("Error: timeout.\n");
				printf("Still waiting...\n");
				curl_easy_cleanup(upd_curl);
				exit (-1);
			}

		} while(resu == CURLE_AGAIN);

		if(resu != CURLE_OK) {
			printf("Error: %s\n", curl_easy_strerror(resu));
			exit (-1);
		}

		printf("Received %" CURL_FORMAT_CURL_OFF_T " bytes.\n",
		(curl_off_t)nread);
	}	

	curl_easy_cleanup(upd_curl);

//kirim data ke node leader
	//insert code here
}

size_t processResponse(void* ptr, size_t size, size_t nmemb, void* stream) {
	godlike = atoi(ptr);

	return size*nmemb;
}
