#include "httpserver.h"

static void			compute_path(request *req)
{
	// Default value
	req->path = "/";
	char *request_type = get_request_name(req->type);
	char *path = req->header;

	// Unknown request type?
	if (request_type)
		path += strlen(request_type) + 1;
	// If path isn't NULL
	path = strtok(path, " ");
	if (path)
		req->path = path;
}

void				manageConnection(server *data, char *payload)
{
	request *req = memalloc(sizeof(request));
	req->payload = payload;
	req->header = strtok(payload, "\r\n");
	req->type = get_type(req->header);
	//printf("Header: %s\n", req->header);
	//printf("Request Type: %s\n", get_request_name(req->type));

	// Scan other lines
	char *rawData = strtok(NULL, "\r\n");
	while (rawData)
	{
		if (startWith(rawData, "Host: "))
			req->hostname = rawData + 6;
		else if (startWith(rawData, "User-Agent: "))
			req->agent = rawData + 12;
		rawData = strtok(NULL, "\r\n");
	}
	compute_path(req);
	//printf("Request Path: %s\n", req->path);
	//printf("Hostname: %s\n", req->hostname);
	//printf("Agent: %s\n", req->agent);
	//printf("Browser: %s\n", get_browser(req->agent));
	printf("[%s]: %s %s\n", inet_ntoa(data->client_addr.sin_addr), get_request_name(req->type), req->path);
	memdel((void**)&req);
}
