#include "AbstractServer.h"
#include "HttpServer.h"
#include "cscript.h"

extern AbstractServer* g_server;
	 void HttpServer::http_handler(void *p){
			REQUEST *r = (REQUEST*)p;
			HttpServer *server = (HttpServer*)r->server;
			fprintf(stderr, "http server handler\n");
		
			char* buf = (char*)r->data;
			char method[100] ="";
			char path[256] = "";
			int i=0;
			int j=0;
			
			// get method 
			while (!isspace((int)(buf[j])) && (i < sizeof(method) - 1))
 			{
  				method[i] = buf[j];
  				i++; j++;
 			}
 			method[i] = '\0';
 			i = 0;
			// eat space after method and before path
 			while (ISspace(buf[j]) && (j < sizeof(buf)))
			  j++;
			while (!ISspace(buf[j]) && (i < sizeof(path) - 1) && (j < r->data_used))
			 {
			  path[i] = buf[j];
			  i++; j++;
			 }
			 path[i] = '\0';
			
			
			// call cs
			CS* cs;
	cs = new CS();
	char target[256] = "";
	char * pt = (char*)path;
	while (pt[0] == '/' && *pt != 0){
		pt++;
	}
	int k = 0;
	while (*pt!=0 && *pt!='?'){
		target[k] =*pt;
		pt++;k++;
	}
		fprintf(stderr, "buf=%s, method=%s path=%s, target=%s\n", buf, method, path, target);
		strcpy(r->path, path);
		strcpy(r->method, method);
		
	//	cs->loadobj(target);
			printf("size=%d\n", server->cs_handler.size());
			 for( int i = 0; i < server->cs_handler.size(); i++ ) 
    		{
				printf("*********execute handler %d********\n", i);
			//
					printf("fasdjfsdaaaaa\n");
//	fprintf(stderr, "-----> hander address = %x handler[%d] = %x\n", ((HttpServer*)g_server)->cs_handler, i, ((HttpServer*)g_server)->cs_handler[0]);
//				fprintf(stderr, "load object %s\n", ((CSHANDLER*)server->cs_handler[i])->className);
				//	printf("fasdjfsdaaaaa\n");
				//char* p = ((CSHANDLER*)server->cs_handler[i])->pattern;
					printf("===>r=%x,%d", r, r);
				cs->loadobj(  ((CSHANDLER*)server->cs_handler[i])->className, (void*)r);
				
    		}
	printf("*********end execute handlers********\n");
			OSWriteToSocket(r->socket, "hello this is NanoHttpSever\n");
	}
	
	void HttpServer::addCSHandler(char* p, char* n)
	{
		printf("=------>0\n");
	
//		int a;
//				cs_handler.add(&a);
//	fprintf(stderr, "-----> ggg = %x, %x\n", cs_handler[0] , &a);
//		*(cs_handler[0])= &a;
//		fprintf(stderr, "-----> ssss = %x, %x\n", *cs_handler[0] , &a);
		CSHANDLER *h = new CSHANDLER();
		strcpy(h->pattern, p);
		
		strcpy(h->className, n);
		
		printf("=------>1\n");
		cs_handler.add(h);
			printf("=------>2\n");
		fprintf(stderr, "-----dd> address = %x handler[0] = %x\n", &cs_handler, cs_handler[0]);
	}