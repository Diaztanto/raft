#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#define PORT 8888

#define LDR 0
#define CDT 1
#define FLWR 2

#define VOTEREQ 9
#define SERVUPD 3
#define PNREQ 4
#define END -1

#define SERVNUM 5
#define ADDRSIZE 20

/*
  request formats
  -1/ end node
  3/x/y is server update, x is server id, y is load
  4/x is prime number request, x is the request for the server

  9/x 9 is vote req, x is the id number of the request sender
*/

int id;
int role;
int timer;
int status; // 0 keeps running, 1 kills self
clock_t start, end;

typedef struct {
  int id;
  char* address;
  int load;
} server_load;

typedef struct {
  server_load* server;
} server_list;

int answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
//reset timer
  start = end;

//proses request
  if (strcmp(url, END) >= 0) {
    status = 1;
    const char *page  = "Farewell.";
  } else if (url[0] == '9' && url[1] == '/') {
    //parse the 8 and / out of the request and add the x for the response
    //tambah kondisi kalo dia udah vote ato belum
    const char *page = (char*) malloc (sizeof(char) * (sizeof(url)-2));

    int i = 0;
    for (i = 0; i < sizeof(page); i++) {
      page[0] = url[2];
    }
  } if (url[0] == '3' && url[1] == '/') {
    //update server load
    const char *page  = "<insert appropriate response here>";
  } else 

//need appropriate response tergantung itu request apa dan dari siapa (?)
  struct MHD_Response *response;
  int ret;

//create response
  response = MHD_create_response_from_buffer (strlen (page),
                                            (void*) page, MHD_RESPMEM_PERSISTENT);

//send response
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);

//clear response
  MHD_destroy_response (response);

  return ret;
}

int main (int argc, char* argv[]){
  struct MHD_Daemon *daemon;
  int timeout;
  int randr;

  status = 0;

//get assigned id
  if (argc < 2) {
    printf("Usage: \"/.node <id>\n");
    return 1;
  }

  id = argv[1];

//init serverlist
  server_list list;
  list.server = (server*) malloc (sizeof(server) * SERVNUM);

  int i;
  for (i = 0; i < SERVNUM; i++) {
    list.server[i]->address = (char*) malloc (sizeof(char) * ADDRSIZE);
  }

//init role
  role = FLWR;

//init randomizer
  randr = clock()/CLOCKS_PER_SEC;

//init timer with randomizer
  timeout = rand_r(&randr) % 1000 + 1;

  printf("timeout = %d\n", timeout);
	
//start timer election
  start = clock();
  end = start;  
  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_END);

  if (NULL == daemon) return 1;

//main loop
  while (status == 0) {
//election/heartbeat timeout
//di reset di fungsi answer cuz timernya var global
    while (timer < timeout) {
//update time
      timer = (end-start) * 1000 / CLOCKS_PER_SEC;
      end = clock();

      printf("%d\n", timer);
    }

//election!
    //insert code here about sending http request ke node lain using curl (yeah masih pake curl)


    //insert code about ngitung vote


    //insert code about sending append entry


    //insert code about starting heartbeat
  }


  getchar (); //<- ini bisa dibuang kalo dah ga perlu

  for (i = 0; i < SERVNUM; i++) {
    free(list.server[i]->address)
  }

  free(list.server);

  MHD_stop_daemon (daemon);
  return 0;
}