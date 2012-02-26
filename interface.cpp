#include "AbstractServer.h"
#include "HttpServer.h"
#include "interface.h"

AbstractServer* g_server = NULL;
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
		data->server = new HttpServer();
		
		if (thread_create_real(start_server, (void*)data, "log_daemon")==-1)
		{
            ERR("Failed to start a new thread to start server");
			delete data;
			return NULL;
        }
		fprintf(stderr, "server %d(0x%x) started\n", data->server, data->server);
		fprintf(stderr, "server %d(0x%x) started\n", g_server, g_server);
	//	((HttpServer*)data->server)->addCSHandler("pattern", "className");
		g_server = data->server;
		return data->server;
	}

	void __stdcall addHandler(HttpServer* server, char* pattern, char* className){
		fprintf(stderr, "server=%d(0x%x), pattern=%s, class=%s\n", server, server, pattern, className);
		//((HttpServer*)server)->test();
		//((HttpServer*)server)->addCSHandler(pattern, className);
		printf("g_server=%lx, server=%lx\n", g_server, server);
	//	HttpServer *pServer = (HttpServer*)server;
		// NOTICE strange thing: has to use global variable instead of passed-int parameter, although  they are equal value
	//	pServer->addCSHandler(pattern, className);
	//	((HttpServer*)g_server)->addCSHandler(pattern, className);
		((HttpServer*)server)->addCSHandler(pattern, className);
	}
		
	AbstractServer* __stdcall destroy_server(HttpServer* server){
		if (g_server)
			delete g_server;
		g_server  = NULL;
	}
	
	
	char* __stdcall request_path(REQUEST* r){
		printf("request=%lx, path=%lx(%ld)， %d, %d\n", r, r->path, r->path, sizeof(long), sizeof(int));
		return r->path;
	}
//	char* __stdcall request_querystring(REQUEST* r){
//		return r->query_string;
//	}

}