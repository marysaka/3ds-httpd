#include "handlers.h"

static void		system_reboot()
{
	aptInit();
	APT_HardwareResetAsync(); // reboot
	aptExit();
}

int				is_system_request(http_request *request)
{
	return startWith(request->path, "/system/") && strlen(request->path) > 7;
}

http_response	*get_system_handler_response(http_request *request)
{
	http_response *response = memalloc(sizeof(http_response));
	response->code = 200;
	response->content_type = strdup("Content-Type: text/html\r\n");
	char	*payload = memalloc(1024 * sizeof(char));
	sprintf(payload, "OK");
	response->payload = payload;
    response->payload_len = strlen(response->payload);
	return response;
}

void 			pre_system_response(http_server *server, http_request *request)
{
	if (!strcmp(request->path, "/system/exit"))
	{
		printf("Remote app exit triggered!");
		server->running = 0;
	}
	else if (!strcmp(request->path, "/system/reboot"))
	{
		printf("System reboot asked!");
		system_reboot();
	}
}
