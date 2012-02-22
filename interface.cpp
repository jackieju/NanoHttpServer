#include "AbstractServer.h"
#include "HttpServer.h"
#include "interface.h"

AbstractServer* g_server;
void start_server(void *data){
	
	STARTSERVER *pd = (STARTSERVER*) data;
			
	pd->server->addHandler(HttpServer::http_handler);
			
	pd->server->start(pd->port);
	
}
#ifdef _MACOS
#define __stdcall 
#endif
extern "C"{
	
	AbstractServer* __stdcall create_server(long port){
	
		STARTSERVER *data = new STARTSERVER;
		data->port = port;
		g_server = data->server = new HttpServer();
		
		if (thread_create_real(start_server, (void*)data, "log_daemon")==-1)
		{
            ERR("Failed to start a new thread to start server");
			delete data;
			return NULL;
        }
		fprintf(stderr, "server %d(0x%x) started\n", data->server, data->server);
		fprintf(stderr, "server %d(0x%x) started\n", g_server, g_server);
	//	((HttpServer*)data->server)->addCSHandler("pattern", "className");
		
		return data->server;
	}

	void __stdcall addHandler(HttpServer* server, char* pattern, char* className){
		fprintf(stderr, "server=%d(0x%x), pattern=%s, class=%s\n", server, server, pattern, className);
		//((HttpServer*)server)->test();
		//((HttpServer*)server)->addCSHandler(pattern, className);
		printf("g_server=%x, server=%x\n", g_server, server);
	//	HttpServer *pServer = (HttpServer*)server;
		// NOTICE strange thing: has to use global variable instead of passed-int parameter, although  they are equal value
	//	pServer->addCSHandler(pattern, className);
		((HttpServer*)g_server)->addCSHandler(pattern, className);
		//((HttpServer*)server)->addCSHandler(pattern, className);
	}

}