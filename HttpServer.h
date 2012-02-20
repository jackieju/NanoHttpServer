#ifndef _HTTPSERVER_NANO
#define _HTTPSERVER_NANO
#include "vector"
struct CSHANDLER{
	char pattern[256];
	char className[256];
	CSHANDLER(){
		memset(pattern, 0, 256);
		memset(className, 0, 256);
	}
	
};
class HttpServer:public AbstractServer{

	std::vector<CSHANDLER*> cs_handler;
public:

	virtual ~HttpServer(){
			 for( vector <CSHANDLER*>::size_type i=cs_handler.size(); i>0; --i ) 
    		{
				if (cs_handler[i])
					delete cs_handler[i];
    		}
	}
	#define ISspace(x) isspace((int)(x))

	static void http_handler(void *p);
	void addCSHandler(char* p, char* n);
	

};


#endif