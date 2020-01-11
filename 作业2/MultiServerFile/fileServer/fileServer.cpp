
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <process.h>
#include "CBlockingSocket.h"


using namespace std;

BYTE filename[100] = { "C:\\Users\\zhang\\Desktop\\123.txt" };
char serverPORT[100] = { 0 };
int i;
int serverNum;
int num = 0;
char clientNum[100] = { 0 };
HANDLE ghMutex;
int fileLength;


DWORD WINAPI serverthread(LPVOID lpParameter)
{
	CBlockingSocket *cs = (CBlockingSocket *)lpParameter;
	CBlockingSocket ClientSocket = *cs;//�þֲ����������̴߳������ĵ�ַ���ݵĲ�������ֹ���߳���socket����д��
	cout << "���뷢���߳�..." << endl;
	cout << "  File requested from the client: " << filename << endl;
	ClientSocket.Read(clientNum, 100);
	cout << "clientNum��ֵ�ǣ�" << clientNum << endl;
	serverNum = atoi(clientNum);
	//serverNum = (int)clientNum[0];
	cout << "serverNum��ֵ�ǣ�" << serverNum << endl;

	ifstream infile((char *)filename, ios::in | ios::binary);
	infile.seekg(0, ios::end);
	fileLength = infile.tellg();
	fileLength = fileLength / 3;

	num = serverNum;
	infile.seekg(0 + num * fileLength, ios::beg);

	num++;

	bool flag = true;
	if (!infile)
	{
		cout << "Open file failed!" << endl;
	}
	else
	{
		BYTE file[101] = { 0 };
		while (fileLength >= 100)
		{
			infile.read((char *)file, 100);
			file[100] = '\0';
			flag = ClientSocket.Send(file, 100);
			fileLength -= 100;
		}
		infile.read((char *)file, fileLength);
		file[fileLength] = '\0';
		flag = ClientSocket.Send(file, fileLength);
	}
	if (flag)
	{
		cout << "  Send file back to the client: " << filename << endl;
	}
	//}
	//cout << "û�����ж�..." << endl;
	ClientSocket.Close();
	return 0;
}

DWORD WINAPI cserverthread(LPVOID lpParameter) {
	CBlockingSocket ListenSocket;
	CBlockingSocket::Initialize();
	ListenSocket.HintsAndResult(NULL, serverPORT);
	ListenSocket.Open();
	ListenSocket.Bind();
	ListenSocket.Listen(serverPORT);
	cout << "FileServer is listening on port:" << serverPORT << endl;

	while (1)
	{
		cout << "Waiting for a connection��" << endl;

		struct sockaddr their_addr;
		struct sockaddr_in their_addrin;
		CBlockingSocket ClientSocket = ListenSocket.Accept(their_addr);
		memcpy(&their_addrin, &their_addr, sizeof(their_addr));
		cout << "Accepted connection from " << inet_ntoa(their_addrin.sin_addr) << endl;

		HANDLE hThread;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)serverthread, &ClientSocket, 0, NULL);

	}
	ListenSocket.Close();
	CBlockingSocket::Cleanup();
}

int main(int argc, char *argv[])
{
	HANDLE aThread;
	DWORD ThreadID;

	cout << "����˿ںţ���ֱ�����Ĭ�϶˿ں�7777��7778��7779�е�һ������" << endl;
	cin >> serverPORT;
	
	aThread = CreateThread(
		NULL,       // default security attributes
		0,          // default stack size
		(LPTHREAD_START_ROUTINE)cserverthread,
		NULL,       // no thread function arguments
		0,          // default creation flags
		&ThreadID); // receive thread identifier

	if (aThread == NULL)
	{
		printf("CreateThread error: %d\n", GetLastError());
		return 1;
	}
	WaitForMultipleObjects(1, &aThread, TRUE, INFINITE);
	printf("�߳̽���");
	return 0;
}
