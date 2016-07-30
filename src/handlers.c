#include "handlers.h"

void				init_handlers()
{
	register_handler(GET, is_defaut_page, get_default_page, NULL, NULL);
	register_handler(GET, is_sdcard_handler, get_sdcard_response, NULL, NULL);
	register_handler(GET, is_system_request, get_system_handler_response, pre_system_response, NULL);
}
