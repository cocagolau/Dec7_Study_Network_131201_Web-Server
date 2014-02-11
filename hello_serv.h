#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define CONTENT_SIZE 102400
#define BUF_SIZE 1024
#define SMALL_BUF 100
#define EPOLL_SIZE 1


#define MAX_CLIENT 1000


#define CR '\r'
#define LF '\n'
#define CRLF "\r\n"


// request
// void * request_handler ( void * arg );
int request_handler ( int * req_status, FILE * clnt_read, FILE * clnt_write );



// response
void response_error ( FILE * fp );
void response_data ( FILE * fp, char * ct, char * file_name );

void response_conn_close ( FILE * fp );

char * content_type ( char * file );

// error
void error_less_argc ( int argc, char * argv[] );
void error_handling ( char * message );


// worker thread
// void * worker_handler ( void * arg );
// int worker_main ( int sock );
void * worker_main ( void * arg );


// disconnection
void * disconnect_worker ( void * arg );


void * worker_main2 ( void * arg );
void send_msg ( char * msg, int len );


int clnt_socks[MAX_CLIENT];
int clnt_cnt;
pthread_mutex_t mutx;