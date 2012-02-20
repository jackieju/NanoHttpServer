#include "AbstractServer.h"
#include "HttpServer.h"
#include "interface.h"

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
		data->server = new HttpServer();;
		if (thread_create_real(start_server, (void*)data, "log_daemon")==-1)
		{
            ERR("Failed to start a new thread to start server");
			delete data;
			return NULL;
        }
		fprintf(stderr, "server %d started", data->server);
		((HttpServer*)data->server)->addCSHandler("pattern", "className");
		return data->server;
	}

	void __stdcall addHandler(AbstractServer* server, char* pattern, char* className){
		fprintf(stderr, "server=%d, pattern=%s, class=%s\n", server, pattern, className);
		((HttpServer*)server)->addCSHandler(pattern, className);
	}

}