#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "CBlockingSocket.h"
using namespace std;

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
int __cdecl main(int argc, char *argv[])
{
	
	int num;
	fstream fs;
	char requestfile_name[100];//请求文件名
	char savefile_name[100] = { 0 };
	int filerecvlen = 0;
	//cout << "命令个数：" << argc << endl;
	CBlockingSocket Blocking_socket = CBlockingSocket();
	if (Blocking_socket.Open("127.0.0.1", "2222"))
	{
		//printf("\rConnection established to remote Server at :%s:%s\n", argv[1], argv[2]); 
		printf("\rInput requset save path: ");
		cin >> requestfile_name;
		if (Blocking_socket.Send(requestfile_name, 100))
		{
			printf("\rRequesting file on the server: %s\n" , requestfile_name);
			printf("\r1.save the file to disk!\n" ) ;
			printf("\r2.print out on the console!\n" );
			printf("\rInput your choise:\n");	
			cin >> num;
			cout << "  =========================================" << endl;
			if (num == 1)
			{
				
				cout << "\rInput save path:";
				cin >> savefile_name;
				cout << "\rReceiving file: " << requestfile_name << endl;
				
				 fs.open(savefile_name, ios::out | ios::binary);					

				char recvBuf[1025] = { 0 };
				
				while (filerecvlen != -1)	
				{
				    //接受成功过返回接受的长度
					filerecvlen= Blocking_socket.Recvfile(recvBuf, sizeof(recvBuf));
				
					fs.write(recvBuf, filerecvlen);
				}				
			}
			else if (num == 2) {
				 
				int recvLen = 0;
				
				char recvBuf[1025] = { 0 };
				int len = 0;
				int count=0;
				//收到的len不为-1 循环读取文件
				while ((len = Blocking_socket.Read(recvBuf, 1024)) != -1)
				{
					count++;
					recvLen += len;
					cout << recvBuf;
				}
				cout << endl << "  =========================================" << endl;
			}
		}
	}
	
	return 0;
}


