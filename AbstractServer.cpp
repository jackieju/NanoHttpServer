#include "AbstractServer.h"
/*
 * 	NOTICE: port must > 999
 */
bool AbstractServer::start(int port){
	LOG1p("create server socket from port %d", port);
	int server_socket =OSMakeServerTCPSocket(port);

	if (server_socket < 0){
		ERR2p("failed to create server stocket  on port %d, error no. %d", port, server_socket);
		return false;
	}
	LOG1p("=>create server socket, %d", server_socket);
	fprintf(stdout, ".\n====\n");
	stopped = false;
	while (m_StopEvent.Wait(10) == LOCKEX_ERR_TIMEOUT)
	{
		fprintf(stderr, ".");
        if (OSReadAvailable(server_socket, SERVER_SELECT_FREQ) < 1)
            continue;
// accept connection
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
#ifdef WIN32
int addrlen;
#else
socklen_t addrlen;
#endif
	 	 addrlen = sizeof(addr);
		
   		 int s = accept(server_socket, (struct sockaddr *) & addr, &addrlen);
   		if (s == -1) 
		{
       		ERR1p("HTTP: Error accepting a client. errno: ", errno);
#ifdef WIN32
			error(errno, "Accept error:%d.", WSAGetLastError());
#endif
       		 continue;
   		}
		REQUEST* r = new REQUEST();
		fprintf(stderr, "r->data=%s",r->data);
		memset(r, 0, sizeof(REQUEST));
		char* host = inet_ntoa(addr.sin_addr);
		fprintf(stderr, "\nget connection from %s\n", host);
		
		strncpy(r->host, host, 127);
		r->socket = s;
		r->server = this;

		if (thread_create_real(handler, (void*)r, "log_daemon")==-1)
		{
            ERR("Failed to start a new thread to handle request");
            CloseSocket((int*)s);
        }
    }
	CloseSocket(&server_socket);
	LOG0("nanoserver exited");
    stopped = true;
}
int get_line(int sock, char *buf, int size)
{
 int i = 0;
 char c = '\0';
 int n;

 while ((i < size - 1) && (c != '\n'))
 {
  n = recv(sock, &c, 1, 0);
  /* DEBUG printf("%02X\n", c); */
  if (n > 0)
  {
   if (c == '\r')
   {
    n = recv(sock, &c, 1, MSG_PEEK);
    /* DEBUG printf("%02X\n", c); */
    if ((n > 0) && (c == '\n'))
     recv(sock, &c, 1, 0);
    else
     c = '\n';
   }
   buf[i] = c;
   i++;
  }
  else
   c = '\n';
 }
 buf[i] = '\0';
 
 return(i);
}