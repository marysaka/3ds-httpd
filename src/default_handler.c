#include "handlers.h"

int				is_defaut_page(http_request *request)
{
	return strcmp(request->path, "/") == 0;
}

http_response	*get_default_page(http_request *request)
{
	http_response *response = memalloc(sizeof(http_response));
	response->code = 200;
	response->content_type = strdup("Content-Type: text/html\r\n");
	char	*payload = memalloc(1024 * sizeof(char));
	sprintf(payload, "<html><title>Homepage</title><h1>Hello from your 3DS</h1><p>I'm running firmware %lu</p></html>", osGetFirmVersion());
	response->payload = payload;
	return response;
}
