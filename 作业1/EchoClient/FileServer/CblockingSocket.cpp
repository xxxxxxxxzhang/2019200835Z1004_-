#include "CBlockingSocket.h"
CBlockingSocket::CBlockingSocket(void)
{
	s = INVALID_SOCKET;
	ip4addr = NULL;
}
CBlockingSocket::CBlockingSocket(SOCKET socket)
{
	s = socket;
	ip4addr = NULL;
}
CBlockingSocket::~CBlockingSocket(void)
{
	if (ip4addr)
		freeaddrinfo(ip4addr);
}

BOOL CBlockingSocket::Socket()
{
	//s = socket(ip4addr->ai_family, ip4addr->ai_socktype, ip4addr->ai_protocol);
	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "open failed!" << endl;
		return FALSE;
	}

	return TRUE;
}

BOOL CBlockingSocket::Close()
{
	closesocket(s);
	return TRUE;
}

BOOL CBlockingSocket::Bind()
{
	int iResult;
	iResult = bind(s, ip4addr->ai_addr, (int)ip4addr->ai_addrlen);
	//cout << "iResult �˿�bind:" << iResult << endl;
	//iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR) {//-1
		printf("bind failed with error: %d\n", WSAGetLastError());
		//closesocket(s);
		//WSACleanup();
		return FALSE;
	}
	return TRUE;

}
BOOL CBlockingSocket::Listen(const char *port)
{

	int iResult = listen(s, 1);
	printf("iResult listen״̬:%d\n", iResult);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return FALSE;
	}
	return TRUE;


}
BOOL CBlockingSocket::Connect()
{
	int iResult;


	iResult = connect(s, ip4addr->ai_addr, (int)ip4addr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("Unable to connect to server!\n");
		return FALSE;
	}

	return TRUE;
}

CBlockingSocket CBlockingSocket::Accept(struct sockaddr &their_addr)
{
	int addr_size = sizeof(their_addr);
	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;


	ClientSocket = accept(s, &their_addr, &addr_size);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return INVALID_SOCKET;
	}

	return CBlockingSocket(ClientSocket);
}

BOOL CBlockingSocket::Send(char *str, UINT count)
{
	// Echo the buffer back to the sender
	//�ͻ�����һ����send������������������󣬶���������ͨ����send��������ͻ�������Ӧ��
	//��һ������ָ�����Ͷ��׽������������ڶ�������ָ��һ�����Ӧ�ó���Ҫ�������ݵĻ�����������������ָ��ʵ��Ҫ���͵����ݵ��ֽ���;���ĸ�����һ����0��
	int iSendResult = send(s, str, count, 0);
	//printf("iResult ��ͻ��˷���Ӧ������:%d\n", iSendResult);
	if (iSendResult != SOCKET_ERROR) {
				
		printf("Bytes sent: %d\n", iSendResult);
		return TRUE;
	}
	else
	{
		
		printf("Send failed!\n");
		return FALSE;
		closesocket(s);
		WSACleanup();
	}


}

INT CBlockingSocket::Read(char *str, UINT count)
{
	int recvLen = recv(s, str, count, 0);
	if (recvLen == SOCKET_ERROR || recvLen == 0)
	{
		cout << "receive failed!" << endl;
		return -1;
	}
	return recvLen;
}
int CBlockingSocket::Initialize()
{
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	printf("iResult:%d\n", iResult);

	return 0;
}
void CBlockingSocket::Cleanup()
{
	WSACleanup();
}

int CBlockingSocket::HintsAndResult(const char *ip, const char *port)
{
	int iResult;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (ip == NULL)
		hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(ip, port, &hints, &ip4addr);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		return 1;
	}

	return 0;
}