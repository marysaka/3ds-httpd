#include "httpserver.h"

int		main(int ac, char **av)
{
	init();
	while (aptMainLoop())
	{
		hidScanInput();

		// socket!
		loop();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;
	}
	destroy();
	return 0;
}
