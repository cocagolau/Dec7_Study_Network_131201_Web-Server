#include "hello_serv.h"
	

int main ( int argc, char * argv[] ) {

	int serv_sock, clnt_sock;

	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_size;
	char buf[BUF_SIZE];

	int i, state, thread_status;

	pid_t pid;
	pthread_t t_id;
	int num_of_worker = 0;

	extern int clnt_socks[MAX_CLIENT];
	extern int clnt_cnt;
	extern pthread_mutex_t mutx;


	// initialization
	clnt_cnt = 0;
	for ( i=0; i<MAX_CLIENT; i++ ) {
		clnt_socks[i] = 0;
	}


	if ( argc != 2 ) {
		error_less_argc ( argc, argv );
	}

	pthread_mutex_init ( &mutx, NULL );
	serv_sock = socket ( PF_INET, SOCK_STREAM, 0 );

	memset ( &serv_adr, 0, sizeof ( serv_adr ) );
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl ( INADDR_ANY );
	serv_adr.sin_port = htons ( atoi ( argv[1] ) );

	if ( bind ( serv_sock, ( struct sockaddr* ) &serv_adr, sizeof ( serv_adr ) ) == -1 ) {
		error_handling ( "bind() error" );
	}

	if ( listen ( serv_sock, 5 ) == -1 ) {
		error_handling ( "listen() error" );
	}


	// while ( 1 ) {
	// 	clnt_adr_size = sizeof ( clnt_adr );
	// 	clnt_sock = accept ( serv_sock, (struct sockaddr* ) &clnt_adr, &clnt_adr_size );

	// 	if ( clnt_sock == -1 )
	// 		continue;

	// 	// else if ( num_of_worker < MAX_CLIENT ) {
	// 		printf ( "connected client: %s:%d \n", inet_ntoa ( clnt_adr.sin_addr ), ntohs ( clnt_adr.sin_port ) );

	// 		// create worker thread
	// 		thread_status = pthread_create ( &t_id, NULL, worker_main, ( void * ) &clnt_sock );
	// 		if ( thread_status != 0 ) {
	// 			puts ( "pthread_create() error" );
	// 			close ( clnt_sock );
	// 			continue;
	// 		}

	// 		pthread_detach ( t_id );
	// 		num_of_worker++;
	// 		printf ( "num_of_worker: %d \n", num_of_worker );

	// 		// sleep ( 3 );
	// 	// }

	// 	// // over connection
	// 	// else {
	// 	// 	printf ( "error: connection denied \n");
	// 	// 	close ( clnt_sock );	
	// 	// }
	// }
	while ( 1 ) {
		clnt_adr_size = sizeof ( clnt_adr );
		clnt_sock = accept ( serv_sock, ( struct sockaddr* ) &clnt_adr, &clnt_adr_size );

		pthread_mutex_lock ( &mutx );
		clnt_socks [ clnt_cnt++ ] = clnt_sock;
		pthread_mutex_unlock ( &mutx );

		pthread_create ( &t_id, NULL, worker_main2, ( void * ) &clnt_sock );
		pthread_detach ( t_id );
		printf ( "Connected client IP: %s \n", inet_ntoa ( clnt_adr.sin_addr ) );
	}


	close ( serv_sock );

	return 0;
}
