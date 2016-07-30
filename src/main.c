#include "httpserver.h"

int		main(int ac, char **av)
{
	init();
	do
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;
	}
	while (aptMainLoop() && loop());
	destroy();
	return 0;
}
