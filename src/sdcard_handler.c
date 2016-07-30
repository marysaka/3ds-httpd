#include "handlers.h"

int 				is_sdcard_handler(http_request *request)
{
	return startWith(request->path, "/sdcard/") && strlen(request->path) > 8;
}

http_response		*get_sdcard_response(http_request *request)
{
	return NULL;
}
