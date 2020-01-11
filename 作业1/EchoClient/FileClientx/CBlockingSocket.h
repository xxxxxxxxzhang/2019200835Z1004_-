
//winsock 2.2
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class CBlockingSocket
{
private:
	SOCKET s;
	struct addrinfo *ip4addr;
public:
	CBlockingSocket(void);  // create socket
	CBlockingSocket(SOCKET socket);
	virtual		~CBlockingSocket(void);
	BOOL		Close();  // close socket
	BOOL		Bind();  // connect socket
	BOOL		Listen(const char *port); 
	BOOL        Connect();
	BOOL		Open(const char *ip, const char *port);
	CBlockingSocket Accept(struct sockaddr &their_addr);
	BOOL		Send(char *s, UINT count);  
	INT			Read(char *s, UINT count);  
	INT			Recvmassage(char *recvbuf, int recvbuflen);
	INT			Recvfile(char *recvbuf, int recvbuflen);
	static int Initialize();
	static void Cleanup();
};
