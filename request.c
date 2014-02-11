#include "hello_serv.h"

// int request_handler ( int * req_status, FILE * clnt_read, FILE * clnt_write ) {
	
// 	int i = 0;
// 	char req_line [ SMALL_BUF ];
	

// 	char method [ 10 ];
// 	char ct [ 15 ];
// 	char file_name [ 30 ];


// 	// request 상태 초기화
// 	*req_status = 0;
	
// 	// header의 첫번째 라인 읽기
// 	fgets ( req_line, SMALL_BUF, clnt_read );
// 	if ( req_line == NULL ) {
// 		return 1;
// 	}

// 	printf ( "req_line:%s \n", req_line );
// 	printf ( "str_len: %ld \n", strlen ( req_line ) );
// 	// confirm HTTP request
// 	if ( strstr ( req_line, "HTTP/" ) == NULL ) {
// 		*req_status = 1;
// 		printf ( "req_line: %s \n", req_line );		

// 		return 0;
// 	}

// 	printf ( "second line \n");


// 	strcpy ( method, strtok ( req_line, " /" ) );
// 	strcpy ( file_name, strtok ( NULL, " /" ) );
// 	strcpy ( ct, content_type ( file_name ) );

// 	// get이 아닌 경우 오류
// 	if ( strcmp ( method, "GET" ) != 0 ) {
// 		return 1;
// 	}

// 	// 2번째 라인부터 헤더 읽기
// 	printf ( "\n\n" );
// 	while ( fgets ( req_line, BUF_SIZE, clnt_read ) ) {

// 		printf ( "req_line: %s", req_line );

// 		if ( strcmp ( req_line, CRLF ) == 0 )
// 			break;
// 	}

// 	printf ("read header\n\n");

// 	response_data ( clnt_write, ct, file_name );

// 	return 0;
	
// }

int request_handler ( int * req_status, FILE * clnt_read, FILE * clnt_write ) {
	
	int i = 0;
	char req_line [ SMALL_BUF ];
	

	char method [ 10 ];
	char ct [ 15 ];
	char file_name [ 30 ];


	// request 상태 초기화
	*req_status = 0;
	
	// header의 첫번째 라인 읽기
	fgets ( req_line, SMALL_BUF, clnt_read );
	if ( req_line == NULL || strlen ( req_line ) == 0 ) {
		return 1;
	}

	printf ( "req_line:%s \n", req_line );
	printf ( "str_len: %ld \n", strlen ( req_line ) );
	// confirm HTTP request
	if ( strstr ( req_line, "HTTP/" ) == NULL ) {
		*req_status = 1;
		printf ( "req_line: %s \n", req_line );		

		return 0;
	}

	printf ( "second line \n");


	strcpy ( method, strtok ( req_line, " /" ) );
	strcpy ( file_name, strtok ( NULL, " /" ) );
	strcpy ( ct, content_type ( file_name ) );

	// get이 아닌 경우 오류
	if ( strcmp ( method, "GET" ) != 0 ) {
		return 1;
	}

	// 2번째 라인부터 헤더 읽기
	printf ( "\n\n" );
	while ( fgets ( req_line, BUF_SIZE, clnt_read ) ) {

		printf ( "req_line: %s", req_line );

		if ( strcmp ( req_line, CRLF ) == 0 )
			break;
	}

	printf ("read header\n\n");

	response_data ( clnt_write, ct, file_name );

	return 0;
	
}