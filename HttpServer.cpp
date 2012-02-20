#include "AbstractServer.h"
#include "HttpServer.h"
#include "cscript.h"


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
	//cs = new CS();
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
		
	//	cs->loadobj(target);
		
			 for( vector <CSHANDLER>::size_type i=server->cs_handler.size(); i>0; --i ) 
    		{
				fprintf(stderr, "load object %s\n", server->cs_handler[i-1]->className);
				char* p = server->cs_handler[i-1]->pattern;
			//	cs->loadobj(server->cs_handler[i-1]->className, (void*)r);
				
    		}
			OSWriteToSocket(r->socket, "hello this is NanoHttpSever\n");
	}
	void HttpServer::addCSHandler(char* p, char* n)
	{
		CSHANDLER *h = new CSHANDLER();
		strcpy(h->pattern, p);
		strcpy(h->className, n);
		cs_handler.push_back(h);
	}