#include "hello_serv.h"

void response_conn_close ( FILE * fp ) {

	char protocol [] = "HTTP/1.0 200 OK\r\n";
	char server [] = "Server:Linux Web Server\r\n";
	char cnt_type [] = "Connection:close\r\n\r\n";

	fputs ( protocol, fp );
	fputs ( server, fp );
	fputs ( cnt_type, fp );

}

void response_error ( FILE * fp ) {

	char protocol [] = "HTTP/1.0 400 Bad Request\r\n";
	char server [] = "Server:Linux Web Server\r\n";
	char cnt_len [] = "Content-length:2048\r\n";
	char cnt_type [] = "Content-type:text/html\r\n\r\n";
	char content [] = "<html><head><title>Network</title></head><body><font size = 5><br>error! not found</font></body></html>";

	fputs ( protocol, fp );
	fputs ( server, fp );
	fputs ( cnt_len, fp );
	fputs ( cnt_type, fp );
	fputs ( content, fp );

}

void response_data ( FILE * fp, char * ct, char * file_name ) {

	int protocol_status_code = 200;
	char protocol [ SMALL_BUF ];
	char server [] = "Server:Linux Web Server\r\n";
	char cnt_len [ SMALL_BUF ];
	char cnt_type [ SMALL_BUF ];
	char buf [ BUF_SIZE ];
	char content [ CONTENT_SIZE ];

	FILE * send_file;
	int send_file_len = 0;

	sprintf ( protocol, "HTTP/1.0 %d OK\r\n", protocol_status_code );
	sprintf ( cnt_type, "Content-type:%s\r\n\r\n", ct );

	send_file = fopen ( file_name, "r" );
	if ( send_file == NULL ) {
		response_error ( fp );
		return ;
	}

	fflush ( fp );
	while ( fgets ( buf, BUF_SIZE, send_file ) != NULL ) {
		strcat ( content, buf );
		fflush ( fp );
	}
	fflush ( fp );


	send_file_len = strlen ( content );
	sprintf ( cnt_len, "Content-length:%d\r\n", send_file_len );

	fputs ( protocol, fp );
	fputs ( server, fp );
	fputs ( cnt_len, fp );
	fputs ( cnt_type, fp );
	fputs ( content, fp );

	content [ 0 ] = '\0';

	printf ( "complete response_data \n\n" );


}

char * content_type ( char * file ) {
	char extenstion [ SMALL_BUF ];
	char file_name [ SMALL_BUF ];

	strcpy ( file_name, file );
	strtok ( file_name, "." );
	strcpy ( extenstion, strtok ( NULL, "." ) );

	if ( !strcmp ( extenstion, "html" ) || !strcmp ( extenstion, "htm" ) )
		return "text/html";
	else if ( !strcmp ( extenstion, "png" ) )
		return "image/png";
	else
		return "text/plain";
}