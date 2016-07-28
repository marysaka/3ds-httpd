#include "utils.h"
#include "httpserver.h"

extern server *app_data;

// --------------------------------------------------------------------------
// memory utils
void			*memalloc(size_t size)
{
	void *res = linearAlloc(size);
	if (!res)
		failExit("alloc: cannot allocate %d bytes!\n", size);
	else
		memset(res, 0, size);
	return (res);
}

void			memdel(void **data)
{
	linearFree(*data);
	*data = NULL;
}
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// string utils
int				startWith(char *str, char *start)
{
	if (!str || !start)
		return (0);
	int startlen = strlen(start);
	return startlen <= strlen(str) && strncmp(str, start, startlen) == 0;
}
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// others utils
void			failExit(const char *fmt, ...)
{

	if(app_data->server_id > 0) close(app_data->server_id);
	if(app_data->client_id > 0) close(app_data->client_id);

	va_list ap;

	printf(CONSOLE_RED);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf(CONSOLE_RESET);
	printf("\nPress B to exit\n");

	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_B) exit(0);
	}
}

// HTTP utils
request_type		get_type(char *str)
{
	if (startWith(str, "GET"))
		return GET;
	else if (startWith(str, "HEAD"))
		return HEAD;
	else if (startWith(str, "POST"))
		return POST;
	else if (startWith(str, "OPTIONS"))
		return OPTIONS;
	else if (startWith(str, "CONNECT"))
		return CONNECT;
	else if (startWith(str, "TRACE"))
		return TRACE;
	else if (startWith(str, "PUT"))
		return PUT;
	else if (startWith(str, "PATCH"))
		return PATCH;
	else if (startWith(str, "DELETE"))
		return DELETE;
	return UNKNOWN;
}

char				*get_request_name(request_type type)
{
	switch (type)
	{
		case GET:
			return "GET";
		case HEAD:
			return "HEAD";
		case POST:
			return "POST";
		case OPTIONS:
			return "OPTIONS";
		case CONNECT:
			return "CONNECT";
		case TRACE:
			return "TRACE";
		case PUT:
			return "PUT";
		case PATCH:
			return "PATCH";
		case DELETE:
			return "DELETE";
		default:
			return NULL;
	}
}

char				*get_browser(char *agent)
{
	char *browser = "Unknown";
	if (strstr(agent, "Netscape"))
		browser = "Netscape";
	if (strstr(agent, "Firefox"))
		browser = "Mozilla Firefox";
	if (strstr(agent, "MSIE") && !strstr(agent, "Opera"))
		browser = "Internet Explorer";
	if (strstr(agent, "Safari"))
		browser = "Apple Safari";
	if (strstr(agent, "Chrome"))
		browser = "Google Chrome";
	if (strstr(agent, "OPR") || strstr(agent, "Opera"))
		browser = "Opera";
	return browser;
}
// --------------------------------------------------------------------------
