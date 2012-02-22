#include "clib.h"
#include "stdio.h"

#include "findfile.h"
#include "mem.h"

#include "os/osutils.h"
#include "AbstractServer.h"
#include "HttpServer.h"

static int nport=3001;
void applyOption(char* p1, char* p2)
{
	if (!stricmp("-p", p1)){
		 nport = atoi(p2);
		printf("Set port to %d", nport);
	}
}
/*void http_handler(void *p){
		fprintf(stderr, "http server handler");
	}*/
int main(int num, char** args){


	if (num == 1){
		printf("NanoServer 0.0.1 Deveopled by jackie.ju@gmail.com\n");
		printf("NanaServer is a real small server only support following protocol:\n");
		printf("http - only support static file and cgi\n");
		printf("Usage: server {options} \n");
		printf("Example: ./server -p 9999\n");
		printf("www.cs-soft.com\n");
	//	return 0;
	}
	
	bool bOption = false;
	char* target = NULL;
	for (int i = 1; i < num; i++){
		if (args[i][0]=='-'){
			applyOption(args[i], args[i+1]);
			i++;
		}else{
			target = args[i];
		}
			
	} 

	LOG1p("Starting server from port %d", nport);
/*	AbstractServer * p = new AbstractServer();	
	p->start(nport);
	p->addHandler(HttpServer::http_handler);
		fprintf(stderr, "-->%d", p->handler_number);
*/
	HttpServer * server = new HttpServer();	
		server->addHandler(HttpServer::http_handler);
			server->addCSHandler("pattern", "className");
	server->start(nport);





}
