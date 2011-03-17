#ifndef ABSSERVER_H
#define ABSSERVER_H
#include "memory.h"
#include "os/socket.h"
#include "log.h"
#include "os/CSS_LOCKEX.H"
#include "clib.h"

#define SERVER_SELECT_FREQ 1000000 
#define SERVER_READ_FREQ   100000

class AbstractServer;
struct REQUEST{
	char host[128];
	int status;
	int in_use;
  //  time_t last_used;
    int socket;
   // int port;
  //  char *buffer;
	AbstractServer* server;
};

class AbstractServer{
public:
	AbstractServer(){
		if (m_StopEvent.Create(NULL, true, false, NULL) != LOCKEX_ERR_OK)
	    	throw new clib::CExp(0, "create stop event failed");
		handler = AbstractServer::default_handler;
	}
	virtual ~AbstractServer(){
		m_StopEvent.Destroy();
	}
	
	bool start(int port);


	    CSS_EVENT	m_StopEvent;	// event to stop server
private:
	
	int stopped;
	bool stop(){
			m_StopEvent.Set();
	}
	thread_func_t* handler;
	
	static void default_handler(void *p){
		if (p == NULL)
			return;
			
		REQUEST *r = (REQUEST*)p;
		char line[1024*4+1] = "";
		OSWriteToSocket(r->socket, "hello this is NanoSever\n");
		fprintf(stderr, "\n socket =  %x \n", r->socket);
//		fprintf(stderr, "enter defaut_handler\n");
	   while (r->server->m_StopEvent.Wait(10) == LOCKEX_ERR_TIMEOUT){
		memset(line, 0, 1024*4+1);
	//	fprintf(stderr,">");
		int ret = 0;
	        if ((ret = OSReadAvailable(r->socket, SERVER_READ_FREQ)) < 1){
	//	fprintf(stderr, "\nselect return %d\n", ret);
	            continue;
			}
		//	fprintf(stderr, "\nselect return %d\n", ret);
			int ret_read = OSReadLineFromSocket(r->socket, line, 1024*4);
			
			// TODO check if it's the right way to judge disconnect
			if (ret_read == 0)
				break;
			fprintf(stderr, "\nrecevied line >> %s\n", line);
		
	}
	fprintf(stderr, "\n socket =  %x \n", r->socket);
	CloseSocket(&(r->socket));
	fprintf(stderr, "leave defaut_handler\n");
	delete r;
}
}



;
#endif
