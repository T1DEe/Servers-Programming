
#include <iostream>
#include <string>
#include <charconv>

#include "Winsock2.h"                // заголовок  WS2_32.dll
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll
#pragma warning(disable:4996) 

using namespace std;

string SetErrorMsgText(string msgText);


int main()
{
	WSADATA wsaData;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup: ");

		//===========================================================================

		SOCKET  clientSocket;                          // клиентский сокет
		if ((clientSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket: ");

		SOCKADDR_IN serverSocketConfig;									// параметры  сокета сервера
		serverSocketConfig.sin_family = AF_INET;						// используется IP-адресация  
		serverSocketConfig.sin_port = htons(2000);						// TCP-порт 2000
		serverSocketConfig.sin_addr.s_addr = inet_addr("127.0.0.1");			// адрес сервера

		if ((connect(clientSocket, (sockaddr*)& serverSocketConfig, sizeof(serverSocketConfig))) == SOCKET_ERROR)
			throw  SetErrorMsgText("connect: ");

		char outBuffer[50];							//буфер вывода 
		int  outBufferLegth = 0;					//количество отправленных байт

		for (int i = 0; i < 1000; i++) {
			string message = "\n Message " + to_string(i);
			strcpy(outBuffer, message.c_str());
			outBufferLegth = strlen(outBuffer);

			if (i == 999) {
				if (send(clientSocket, outBuffer, outBufferLegth + 1, NULL) == SOCKET_ERROR)
					throw  SetErrorMsgText("send: ");
			}
			else {
				if (send(clientSocket, outBuffer, outBufferLegth, NULL) == SOCKET_ERROR)
					throw  SetErrorMsgText("send: ");
			}
		}


		//===========================================================================

		if (closesocket(clientSocket) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket: ");


		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup: ");
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}


	return 0;
}

string SetErrorMsgText(string msgText)
{
	return  msgText + to_string(WSAGetLastError());
};
