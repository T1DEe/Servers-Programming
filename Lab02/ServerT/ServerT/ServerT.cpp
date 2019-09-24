
#include <iostream>
#include <string>

#include "Winsock2.h"                // заголовок  WS2_32.dll
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll
#pragma warning(disable:4996)

using namespace std;

string SetErrorMsgText(string msgText);


int main()
{
	SOCKET  serverSocket;           // дескриптор сокета 
	WSADATA wsaData;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup: ");

		//===========================================================================

		if ((serverSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket: ");

		SOCKADDR_IN serverSocketConfig;								// параметры  сокета serverSocket
		serverSocketConfig.sin_family = AF_INET;					// используется IP-адресация  
		serverSocketConfig.sin_port = htons(2000);					// порт 2000
		serverSocketConfig.sin_addr.s_addr = inet_addr("127.0.0.1");		// собственный IP-адрес 

		if (bind(serverSocket, (LPSOCKADDR)& serverSocketConfig, sizeof(serverSocketConfig)) == SOCKET_ERROR)
			throw  SetErrorMsgText("bind: ");

		if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
			throw  SetErrorMsgText("listen: ");
		
		SOCKET clientSocket;											// сокет для обмена данными с клиентом 
		SOCKADDR_IN clientSocketConfig;									// параметры  сокета клиента
		memset(&clientSocketConfig, 0, sizeof(clientSocketConfig));		// обнулить память
		int clientSocketConfigLength = sizeof(clientSocketConfig);		// размер SOCKADDR_IN

		if ((clientSocket = accept(serverSocket, (sockaddr*)& clientSocketConfig, &clientSocketConfigLength)) == INVALID_SOCKET)
			throw  SetErrorMsgText("accept: ");



		cout << "IP: " << inet_ntoa(clientSocketConfig.sin_addr) << endl;
		cout << "Port: " << clientSocketConfig.sin_port << endl;

		char inBuffer[50];				           //буфер ввода 
		int  inBufferLength = 0;                   //количество принятых байт

		if ((inBufferLength = recv(clientSocket, inBuffer, sizeof(inBuffer), NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv: ");

		cout << "Message from client: " << inBuffer << endl;

		//===========================================================================

		if (closesocket(serverSocket) == SOCKET_ERROR)
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
