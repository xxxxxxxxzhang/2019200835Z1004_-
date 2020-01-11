
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
	CBlockingSocket(void);  
	CBlockingSocket(SOCKET socket);
	virtual		~CBlockingSocket(void);
	BOOL		Socket();  
	BOOL		Close();  
	BOOL		Bind();  
	BOOL		Listen(const char *port);  
	BOOL        Connect();
	CBlockingSocket Accept(struct sockaddr &their_addr);
	BOOL		Send(char *s, UINT count);  // send file
	INT			Read(char *s, UINT count);  

	int HintsAndResult(const char *ip, const char *port);
	static int Initialize();
	static void Cleanup();
};


