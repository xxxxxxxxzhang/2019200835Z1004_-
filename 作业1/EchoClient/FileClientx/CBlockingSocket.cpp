#include "CBlockingSocket.h"


CBlockingSocket::CBlockingSocket(void)
{
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}
}
CBlockingSocket::CBlockingSocket(SOCKET socket)
{
	socket = socket;
	ip4addr = NULL;
}

CBlockingSocket::~CBlockingSocket(void)
{
}

BOOL CBlockingSocket::Open(const char *ip, const char *port)
{
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;

	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr(ip);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(atoi(port));
	int iResult = connect(s, (SOCKADDR *)&addrClient, sizeof(SOCKADDR));

	if (iResult == 0)
	{
		cout << "��������" << endl;
		return TRUE;
	}
	else
	{
		cout << "Open socket Failed!" << endl;
		return FALSE;
	}
}
BOOL CBlockingSocket::Close()
{
	closesocket(s);
	return TRUE;
}

BOOL CBlockingSocket::Bind()
{
	int iResult;
	iResult = bind(s, (sockaddr*)&ip4addr, sizeof(ip4addr));
	cout << "iResult �˿�bind:" << iResult << endl;
	//iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {//-1
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return false;
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
		closesocket(s);
		WSACleanup();
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
		return 1;
	}

	return CBlockingSocket(ClientSocket);
}
BOOL CBlockingSocket::Send(char *str, UINT count)
{
	// Echo the buffer back to the sender
				//�ͻ�����һ����send������������������󣬶���������ͨ����send��������ͻ�������Ӧ��
				//��һ������ָ�����Ͷ��׽������������ڶ�������ָ��һ�����Ӧ�ó���Ҫ�������ݵĻ�����������������ָ��ʵ��Ҫ���͵����ݵ��ֽ���;���ĸ�����һ����0��
	int iSendResult = send(s, (char*)str, count, 0);
	//	printf("iResult ��ͻ��˷���Ӧ������:%d\n", iSendResult);
	if (iSendResult != SOCKET_ERROR) {

		printf("Bytes sent: %d\n", iSendResult);
		return TRUE;
	}
	else
	{
		printf("Send failed!\n");
		return FALSE;
	}
}

INT CBlockingSocket::Read(char *str, UINT count)
{
	int recvLen = recv(s, (char *)str, count, 0);
	//cout << "��������" << recvLen << endl;
	if (recvLen == 0)
	{
		return -1;
	}
	if (recvLen == SOCKET_ERROR)
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
	//	printf("iResult:%d\n", iResult);

	return 0;
}
void CBlockingSocket::Cleanup()
{
	WSACleanup();
}

INT CBlockingSocket::Recvmassage(char *recvbuf, int recvbuflen)
{
	int iResult;
	//��tcp����һ�˽����ļ�
	iResult = recv(s, recvbuf, recvbuflen, 0);//char *recvbuf
	if (iResult > 0)//���ܵĳ���
	{
		cout<<"recvbuf::"<<recvbuf<<endl;
		if(&recvbuf-'0')
		cout << "don't have this file! connect fialed..." << endl;
		
		return 1;
		
	}
	else if (iResult == 0)
	{
		//closesocket(s);
		printf("find your request file!\n");
		return 0;
	}
	else
	{
			
		return 0;
	}
}
INT CBlockingSocket::Recvfile(char *recvbuf, int recvbuflen)
{

	int iResult;
	//��tcp����һ�˽����ļ�
	iResult = recv(s, recvbuf, recvbuflen, 0);//char *recvbuf
	if (iResult > 0)//���ܵĳ���
	{
		return iResult;
	}
	else if (iResult == 0)
	{
		
		printf("Connection closed\n");
		return -1;
	}
	else
	{
		printf("receive failed: %d\n", WSAGetLastError());
		return -1;
	}
}
