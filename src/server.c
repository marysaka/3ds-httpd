#include "httpserver.h"

const static char http_200[] = "HTTP/1.1 200 OK\r\n";
static u32		*socket_buffer = NULL;
static server	data;
server			*app_data = &data;
static int		ret;
static char		payload[1026];

void			socShutdown()
{
	printf("waiting for socExit...\n");
	socExit();
}

void			init()
{
	hidInit(); // input
	gfxInitDefault(); // graphics
	consoleInit(GFX_TOP, NULL); // default console
	consoleDebugInit(debugDevice_CONSOLE);
	init_handlers();
	socket_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
	if (socket_buffer == NULL)
		failExit("Socket buffer allocation failed!\n");


	// Init soc:u service
	if ((ret = socInit(socket_buffer, SOC_BUFFERSIZE)) != 0)
    	failExit("Service initialization failed! (code: 0x%08X)\n", (unsigned int)ret);

	// Make sure the struct is clear
	memset(&data, 0, sizeof(data));
	data.client_id = -1;
	data.server_id = -1;
	data.server_id = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	// Is socket accessible?
	if (data.server_id < 0)
		failExit("socket: %s (code: %d)\n", strerror(errno), errno);


	// Init server_addr on default address and port 8081
	data.server_addr.sin_family = AF_INET;
	data.server_addr.sin_port = htons(8081);
	data.server_addr.sin_addr.s_addr = gethostid();
	data.client_length = sizeof(data.client_addr);

	// Print network info
	printf("Server is running http://%s/\n", inet_ntoa(data.server_addr.sin_addr));

	if ((ret = bind(data.server_id, (struct sockaddr *) &data.server_addr, sizeof(data.server_addr))))
	{
		close(data.server_id);
		failExit("bind: %s (code: %d)\n", strerror(errno), errno);
	}

	// Set socket non blocking so we can still read input to exit
	fcntl(data.server_id, F_SETFL, fcntl(data.server_id, F_GETFL, 0) | O_NONBLOCK);

	if ((ret = listen(data.server_id, 5)))
		failExit("listen: %s (code: %d)\n", strerror(errno), errno);
	printf("Ready...\n");
}

void			loop()
{
	data.client_id = accept(data.server_id, (struct sockaddr *) &data.client_addr, &data.client_length);
	if (data.client_id < 0 && errno != EAGAIN)
		failExit("accept: %d %s\n", errno, strerror(errno));
	else
	{
		// set client socket to blocking to simplify sending data back
		fcntl(data.client_id, F_SETFL, fcntl(data.client_id, F_GETFL, 0) & ~O_NONBLOCK);
		// reset old payload
		memset(payload, 0, 1026);

		// Read 1024 bytes (FIXME: dynamic size)
		ret	= recv(data.client_id, payload, 1024, 0);

		// HTTP 1.1?
		if (strstr(payload, "HTTP/1.1"))
			manage_connection(&data, payload);

		// End connection
		close(data.client_id);
		data.client_id = -1;
	}
}

void			destroy()
{
	close(data.server_id);
	socShutdown();
	gfxExit();
	hidExit();
}
