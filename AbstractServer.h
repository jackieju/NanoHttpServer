#ifndef ABSSERVER_H
#define ABSSERVER_H
#include "memory.h"
#include "os/socket.h"
#include "log.h"
#include "os/CSS_LOCKEX.H"
#include "clib.h"

#define SERVER_SELECT_FREQ 1000000 
#define SERVER_READ_FREQ   100000
int get_line(int sock, char *buf, int size);

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
    unsigned char* data;
	int data_len;
	int data_used;
				char method[100];
			char path[256];
			char query_string[256];
	void setData(unsigned char* p, long len){
		data = new unsigned char[len];
		memset(data, 0, len);
		memcpy(data, p, len);
		data_len = len;
		data_used = len;
	}
	void addData(unsigned char* d, long len){
		if (data_used+len > data_len){
			unsigned char* p = new unsigned char[data_len+4096];
			memset(p, 0, data_len+4096);
			memcpy(p, data, data_used);
		
			if (data) 
				delete data;
			data=p;
			data_len = data_len+4096;
		}
		memcpy(data+data_used, d, len);
		data_used += len;
		
	}
	REQUEST(){
		fprintf(stderr, "create request");
		data = new unsigned char[1024*4];
		data_len = 4096;
		data_used = 0;
		memset(data, 0, 1024*4);
	}
	~REQUEST(){
		fprintf(stderr, "delete request");
		if (data)
			delete data;
		data= NULL;
	}
};

class AbstractServer{
public:
	AbstractServer(){
		if (m_StopEvent.Create(NULL, true, false, NULL) != LOCKEX_ERR_OK)
	    	throw new clib::CExp(0, "create stop event failed");
		handler = AbstractServer::default_handler;
		memset(hs, 0, 10);
		handler_number = 0;
		stopped = true;
	}
	virtual ~AbstractServer(){
		stop();
		while(!stopped){
			JUJU::sleep(100);
		}
		m_StopEvent.Destroy();
	}
	
	bool start(int port);


	    CSS_EVENT	m_StopEvent;	// event to stop server
	
		thread_func_t* hs[10];
	int handler_number;
	
	void addHandler(thread_func_t* p){
		if (handler_number >= 10)
			return;
		
		hs[handler_number] = p;
		handler_number++;
	}
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
	
		fprintf(stderr, "\n socket =  %x \n", r->socket);
//		fprintf(stderr, "enter defaut_handler\n");
	   while (r->server->m_StopEvent.Wait(10) == LOCKEX_ERR_TIMEOUT){
		memset(line, 0, 1024*4+1);
	//	fprintf(stderr,">");
	
		int ret = 0;
	        if ((ret = OSReadAvailable(r->socket, SERVER_READ_FREQ)) < 1){
		fprintf(stderr, "\nselect return %d\n", ret);
	        //    continue;
			break;
			}
			fprintf(stderr, "\nselect return %d\n", ret);
			fprintf(stderr, "recv...\n");
			//char *l;int ll;
			//int ret_read = OSReadSocketToEof(r->socket, &l, (size_t*)&ll);
			int ret_read = OSReadLineFromSocket(r->socket, line, 1024*4);
	
				//ret = get_line(r->socket, line, 4096);
				fprintf(stderr, "recv %d char", ret_read);
			//	if (ret == 1)
			//		fprintf(stderr, "recv %x", line[0]);
			//	if (ret == 0)
			//		break;
			// TODO check if it's the right way to judge disconnect
			
				if (ret_read == 1)
					fprintf(stderr, "recv %x", line[0]);
			if (ret == 1 && ret_read == 0 && errno!=EINTR)
				break;
			
			fprintf(stderr, "\nrecevied line >> %s", line);
			//fprintf(stderr, "\ndata >> %s\n", r->data->c_str());
			r->addData((unsigned char*)line, ret_read);
	}
	//JUJU::sleep(2);
	//	int ret_read = OSReadLineFromSocket(r->socket, line, 1024*4);
	//			fprintf(stderr, "\nrecevied line >> %s", line);
	int i =0;
	fprintf(stderr, "hander number = %d\n", r->server->handler_number);

		for (i = 0; i < r->server->handler_number; i++){
			fprintf(stderr, "exec handler %d\n", i);
			r->server->hs[i](r);
		}
		if (i == 0){
			OSWriteToSocket(r->socket, "hello this is NanoSever\n");
	//	OSWriteToSocket(r->socket, "ContentType: text/html;charset=gb2312'\n<form action='/' method='post'><input name='dd' value='dffas' /><input type='submit'/></form>");
		}
		
	//fprintf(stderr, "\n socket =  %x \n", r->socket);
	CloseSocket(&(r->socket));
	fprintf(stderr, "leave defaut_handler\n");
	delete r;
}
}



;
#endif
