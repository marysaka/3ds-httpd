#include "handlers.h"

void				init_handlers()
{
	register_handler(GET, is_defaut_page, get_default_page);
	register_handler(GET, is_sdcard_handler, get_sdcard_response);
}
