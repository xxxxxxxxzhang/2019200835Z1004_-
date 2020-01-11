#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include"CBlockingSocket.h"
using namespace std;
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
DWORD WINAPI start_server_thread(LPVOID lpParameter);
//#define DEFAULT_PORT "27015"
int __cdecl main(int argc, char *argv[])
{
	/*
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <PORT>" << endl;
		return 1;
	}
	*/
	CBlockingSocket Listen_socket;
	//初始化套接字的接口
	CBlockingSocket::Initialize();
	Listen_socket.HintsAndResult(NULL,"2222");
	Listen_socket.Socket();
	Listen_socket.Bind();
	Listen_socket.Listen("2222");
	cout<<"fileserver is listrning port:2222"<<endl;
	while (1)
	{
		struct sockaddr client_addr;//用于做参数
		struct sockaddr_in client_addrin;//用于赋值
		CBlockingSocket ClientSocket = Listen_socket.Accept(client_addr);
		memcpy(&client_addrin, &client_addr, sizeof(client_addr));
		cout << "Accepted connection from " << inet_ntoa(client_addrin.sin_addr) << endl;
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_server_thread, &ClientSocket, 0, NULL);
	}
	Listen_socket.Close();
	CBlockingSocket::Cleanup();
	return 0;
}
DWORD WINAPI start_server_thread(LPVOID lpParameter)
{
	CBlockingSocket *cs = (CBlockingSocket *)lpParameter;
	//设置
	CBlockingSocket Client_socket = *cs;
	char requestfile_name[100] = { 0 };
	if (Client_socket.Read(requestfile_name, 100) != -1)
	{
		printf("File requested from the client：%s\n", requestfile_name);
		//二进制方式打开文件
		ifstream infile(requestfile_name, ios::in | ios::binary);
		if(!infile)
		{
			cout << "Open file failed!" << endl;
	
			return 0;
		}
		infile.seekg(0, ios::end);//定位到文件尾部
		int file_len = infile.tellg();
		infile.seekg(0, ios::beg);//定位到文件首部
		bool send_status = true;//发送状态
		char file[1025] = { 0 };
		while (file_len >= 1024)
		{
			infile.read(file, 1024);
			file[1024] = '\0';
			send_status = Client_socket.Send(file, 1024);
			file_len -= 1024;
		}
		infile.read(file, file_len);
		file[file_len] = '\0';
		send_status = Client_socket.Send(file, file_len);
		if (send_status)
		{
			cout << "  Send file back to the client: " << requestfile_name << endl;
		}
	}
	Client_socket.Close();
	return 0;
}