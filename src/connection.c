#include "httpserver.h"

static void			compute_path(http_request *req)
{
	// Default value
	req->path = "/";
	char *request_type = get_request_name(req->type);
	char *path = req->header;

	// Unknown request type?
	if (request_type)
		path += strlen(request_type) + 1;
	path = strtok(path, " ");

	// If path isn't NULL
	if (path)
		req->path = path;
}

void				send_response(s32 client_id, http_response *resp)
{
	char	header[256];
	memset(header, 0, 256);
	sprintf(header, HTTP_HEADER_TEMPLATE, resp->code, get_http_code_name(resp->code));

	// HTTP/1.1 code message
	send(client_id, header, strlen(header), 0);
	// Headers (TODO: Implement a global list?)
	send(client_id, resp->content_type, strlen(resp->content_type), 0);
	send(client_id, "Server: 3ds-httpd\r\n", 19, 0);
	// End the header section
	send(client_id, "\r\n", 2, 0);

	// Send the payload
	send(client_id, resp->payload, strlen(resp->payload), 0);
}

void				manage_connection(server *data, char *payload)
{
	http_request *req = memalloc(sizeof(http_request));
	req->payload = payload;
	req->header = strtok(payload, "\r\n");
	req->type = get_type(req->header);

	// Scan others lines
	char *rawData = strtok(NULL, "\r\n");
	while (rawData)
	{
		if (startWith(rawData, "Host: "))
			req->hostname = rawData + 6;
		else if (startWith(rawData, "User-Agent: "))
			req->agent = rawData + 12;
		rawData = strtok(NULL, "\r\n");
	}
	// extract the path from the header
	compute_path(req);

	// get request handler
	http_request_handler	*handler = get_request_handler(req);
	http_response		*resp = NULL;

	// Use the handler to try to the response to send
	if (handler)
		resp = handler->get_response(req);

	// If the response is null, we use the default "Not Implemented" page (returning a 501 error code)
	if (resp == NULL)
	{
		// copy default response
		resp = memalloc(sizeof(DEFAULT_PAGE));
		memcpy(resp, &DEFAULT_PAGE, sizeof(DEFAULT_PAGE));
		resp->content_type = strdup(resp->content_type);
		resp->payload = strdup(resp->payload);
	}

	printf("[%d]: %s %s (client: %s)\n", resp->code, get_request_name(req->type), req->path, inet_ntoa(data->client_addr.sin_addr));
	send_response(data->client_id, resp);

	// Clean all data created in this function because we don't need them anymore
	free(resp->content_type);
	free(resp->payload);
	memdel((void**)&resp);
	memdel((void**)&req);
}
