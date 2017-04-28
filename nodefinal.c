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

int role;
int timer;
clock_t start, end;

int answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
//reset timer
  start = end;

//need appropriate response tergantung itu request apa dan dari siapa (?)
  const char *page  = "INSERT RESPONSE HERE";
  struct MHD_Response *response;
  int ret;

  response = MHD_create_response_from_buffer (strlen (page),
                                            (void*) page, MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}

int main (){
  struct MHD_Daemon *daemon;
  int timeout;
  int randr;

  role = FLWR;
  randr = clock()/CLOCKS_PER_SEC;

  timeout = rand_r(&randr) % 1000 + 1;

  printf("%d\n", timeout);
	
  start = clock();
  end = start;  
  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_END);

  if (NULL == daemon) return 1;

//election/heartbeat timeout
//di reset di fungsi answer cuz timernya var global
  while (timer < timeout) {
  	timer = (end-start) * 1000 / CLOCKS_PER_SEC;
  	end = clock();

  	printf("%d\n", timer);
  }

//election!
  //insert code here about sending http request ke node lain using curl (yeah masih pake curl)

  getchar (); //<- ini bisa dibuang kalo dah ga perlu

  MHD_stop_daemon (daemon);
  return 0;
}