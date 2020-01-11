#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include "CBlockingSocket.h"
#include <windows.h>
#include <stdio.h>

using namespace std;


char filename2[100] = { "C:\\Users\\zhang\\Desktop\\123.txt" };
char serverIP[100] = { "222.31.79.201" };
char serverPORT[100] = { 0 };
int serverPORT2 = 7777;
char conNum[100] = { 0 };
int m = 0;
int i;
HANDLE ghMutex;
fstream fs;
int port = 7777;
DWORD WINAPI clientthread(LPVOID);
DWORD WINAPI clientthreadNoMutex(LPVOID);

#define ThreadFunction clientthread

//使用互斥信号量
DWORD WINAPI clientthread(LPVOID lpParameter) {

	_itoa(serverPORT2, serverPORT, 10);
	cout << "Connection established to remote Server at " << serverIP << ":" << serverPORT << endl;//7777端口
	CBlockingSocket BlockingSocket = CBlockingSocket();
	BlockingSocket.Open(serverIP, serverPORT);
	cout << "Receiving file... " << endl;
	_itoa(i, conNum, 10);//
	BlockingSocket.Send(conNum, 100);
	WaitForSingleObject(
		ghMutex,    // handle to mutex
		INFINITE);
	char recvBuf[101] = { 0 };
	int filerecvd = 0;
	while (filerecvd != -1)
	{
		filerecvd = BlockingSocket.Recv(recvBuf, sizeof(recvBuf));
		fs.write(recvBuf, filerecvd);

	}
	return 0;

}
//未使用互斥信号量
DWORD WINAPI clientthreadNoMutex(LPVOID lpParameter) {

	_itoa(serverPORT2, serverPORT, 10);
	cout << "Connection established to remote Server at " << serverIP << ":" << serverPORT << endl;
	CBlockingSocket BlockingSocket = CBlockingSocket();
	BlockingSocket.Open(serverIP, serverPORT);
	cout << "Receiving file... " << endl;
	_itoa(i, conNum, 10);
	cout << "客户端发送的数字："<<conNum << endl;
	BlockingSocket.Send(conNum, 100);

	char recvBuf[101] = { 0 };
	int filerecvd = 0;
	cout << "data from" << GetCurrentThreadId() << endl;
	while (filerecvd != -1)
	{
		filerecvd = BlockingSocket.Recv(recvBuf, sizeof(recvBuf));
		cout << "  Received " << filerecvd << " bytes." << endl;
		fs.write(recvBuf, filerecvd);
	}
	return 0;
}

int main(void) {
	HANDLE aThread[3];
	DWORD ThreadID;

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	}
	fs.open(filename2, ios::out | ios::binary);
	for (i = 0; i < 3; i++)
	{
		aThread[i] = CreateThread(
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE)ThreadFunction,//使用带互斥信号量的
			NULL,       // no thread function arguments
			0,          // default creation flags
			&ThreadID); // receive thread identifier

		if (aThread[i] == NULL)
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
		Sleep(200);
		serverPORT2++;
	}
	WaitForMultipleObjects(3, aThread, TRUE, INFINITE);
	printf("文件下载完毕...");
	Sleep(5000);
	for (i = 0; i < 3; i++)
		CloseHandle(aThread[i]);
	CloseHandle(ghMutex);
}