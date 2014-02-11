#include "hello_serv.h"


void * worker_main ( void * arg ) {

	int sock = * ( ( int * ) arg );
	int result, code;

	FILE * clnt_write;
	FILE * clnt_read;
	int req_status, i=0;

	clnt_read = fdopen ( sock, "r" );
	
	
	while ( 1 ) {

		// if ( fcntl ( sock, F_GETFL ) == -1 ) {
		// 	printf ( "closed socket\n" );
		// 	break;
		// }

		clnt_write = fdopen ( dup ( sock ), "w" );
		result = request_handler ( &req_status, clnt_read, clnt_write );

		//error 인 경우 response_error호출
		if ( result ) {
			response_error ( clnt_write );

			
			break;
		}
		
		
		// fflush ( clnt_write );
		// fflush ( clnt_read );
		fclose ( clnt_write );

		sleep ( 1 );
		
	}
	
	fclose ( clnt_read );
	
	printf ("thread end\n----------\n\n");
}




void * worker_main2 ( void * arg ) {

	extern int clnt_socks[MAX_CLIENT];
	extern int clnt_cnt;
	extern pthread_mutex_t mutx;

	int clnt_sock = * ( ( int * ) arg );
	int str_len = 0, i, req_result = 0, req_status;
	char msg[BUF_SIZE];

	FILE * clnt_read;
	FILE * clnt_write;

	clnt_read = fdopen ( clnt_sock, "r" );
	clnt_write = fdopen ( dup ( clnt_sock ), "w" );
	
	while ( !req_result ) {
		req_result = request_handler ( &req_status, clnt_read, clnt_write );

		// str_len = strlen ( msg );
		// if ( str_len == 0 )
		// 	break;

		// send_msg ( msg, str_len );
		fflush ( clnt_read );
		fflush ( clnt_write );
	}


	// while ( 1 ) {
	// 	fgets ( msg, BUF_SIZE, clnt_read );


	// 	str_len = strlen ( msg );
	// 	if ( str_len == 0 )
	// 		break;

	// 	send_msg ( msg, str_len );
	// 	fflush ( clnt_read );
	// }
	// while ( ( str_len = read ( clnt_sock, msg, sizeof ( msg ) ) ) != 0 )
	// 	send_msg ( msg, str_len );


	// clnt_socks배열에서 종료된 socket을 삭제 후 위치 조정하는 로직
	pthread_mutex_lock ( &mutx );
	for ( i=0; i<clnt_cnt; i++ ) {
		if ( clnt_sock == clnt_socks[i] ) {
			while ( i++ < clnt_cnt - 1 )
				clnt_socks[i] = clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock ( &mutx );


	// close ( clnt_sock );
	fclose ( clnt_write );
	fclose ( clnt_read );
	printf ( "disconn sock\n" );


	return NULL;
}

// void send_msg ( FILE * clnt_write, char * msg ) {

// 	extern int clnt_socks[MAX_CLIENT];
// 	extern int clnt_cnt;
// 	extern pthread_mutex_t mutx;

// 	int i;

// 	pthread_mutex_lock ( &mutx );
// 	for ( i=0; i<clnt_cnt; i++ )
// 		write ( clnt_socks[i], msg, len );
// 	pthread_mutex_unlock ( &mutx );

// }



void send_msg ( char * msg, int len ) {

	extern int clnt_socks[MAX_CLIENT];
	extern int clnt_cnt;
	extern pthread_mutex_t mutx;

	int i;

	pthread_mutex_lock ( &mutx );
	for ( i=0; i<clnt_cnt; i++ )
		write ( clnt_socks[i], msg, len );
	pthread_mutex_unlock ( &mutx );

}
