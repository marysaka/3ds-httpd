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
} request_type;

typedef struct
{
	request_type		type;
	char				*agent;
	char				*header;
	char				*hostname;
	char				*path;
	char				*payload;
} request;


typedef struct
{
	int					code;
	char				*content_type;
	char				*payload;
} response;

// silence the unused warning because we use it!
__attribute__((unused))
static response DEFAULT_PAGE = {.code = 501, .content_type = "Content-Type: text/html\r\n", .payload = "<html><title>501 - Not Implemented</title><h1>501 - Not Implemented</h1></html>"};
typedef int (*isCorrectHandler)(request*);
typedef response *(*computeResponse)(request*);

typedef struct
{
	request_type		type;
	isCorrectHandler	check;
	computeResponse		get_response;
} request_handler;

void			init();
void			loop();
void			destroy();
void			manageConnection(server *data, char *payload);
request_handler	*get_request_handler(request *req);
request_type	get_type(char *str);
char			*get_browser(char *agent);
char			*get_request_name(request_type type);
char			*get_http_code_name(int code);
#endif
