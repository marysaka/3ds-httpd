#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

#define HTTP_HEADER_TEMPLATE "HTTP/1.1 %d %s\r\n"

typedef struct
{
	struct sockaddr_in	client_addr;
	struct sockaddr_in	server_addr;
	u32					client_length;
	s32					server_id;
	s32					client_id;
} server;

typedef enum
{
	GET,
	HEAD,
	POST,
	OPTIONS,
	CONNECT,
	TRACE,
	PUT,
	PATCH,
	DELETE,
	UNKNOWN
} http_request_type;

typedef struct
{
	http_request_type	type;
	char				*agent;
	char				*header;
	char				*hostname;
	char				*path;
	char				*payload;
} http_request;


typedef struct
{
	int					code;
	char				*content_type;
	char				*payload;
} http_response;

// silence the unused warning because we use it!
__attribute__((unused))
static http_response DEFAULT_PAGE = {.code = 501, .content_type = "Content-Type: text/html\r\n", .payload = "<html><title>501 - Not Implemented</title><h1>501 - Not Implemented</h1></html>"};
typedef int (*is_handler)(http_request*);
typedef http_response *(*compute_response)(http_request*);

typedef struct
{
	http_request_type	type;
	is_handler			check;
	compute_response	get_response;
} http_request_handler;

typedef struct s_handler_array
{
	http_request_handler	*data;
	struct s_handler_array	*next;
} http_request_handler_array;

void					init();
void					loop();
void					destroy();
void					manage_connection(server *data, char *payload);
void					register_handler(http_request_type type, is_handler check, compute_response get_response);
http_request_handler	*get_request_handler(http_request *request);
http_request_type		get_type(char *str);
char					*get_browser(char *agent);
char					*get_request_name(http_request_type type);
char					*get_http_code_name(int code);
#endif
