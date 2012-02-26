#ifndef _HTTPSERVER_NANO
#define _HTTPSERVER_NANO
#include "array.h"

struct CSHANDLER{
	char pattern[256];
	char className[256];
	CSHANDLER(){
		memset(pattern, 0, 256);
		memset(className, 0, 256);
	}
	
};

class HttpServer:public AbstractServer{

	JUJU::Array cs_handler;
public:
	
	virtual ~HttpServer(){
		fprintf(stderr, "\n1\n");
				fprintf(stderr, " handlers size %d\n", cs_handler.size());
		for( int i =0; i< cs_handler.size(); i++ ) 
    		{
		fprintf(stderr, "delete handler %d\n", i);
				if (cs_handler[i])
					delete cs_handler[i];
				cs_handler[i] = NULL;
    		}
			fprintf(stderr, "deleted handlers\n");
	}
	#define ISspace(x) isspace((int)(x))

	static void http_handler(void *p);
	void addCSHandler(char* p, char* n);
	

};


#endif