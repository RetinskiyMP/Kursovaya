#include "pch.h"
#include <iostream>
#include <WinSock2.h> //сокеты 
#include <WS2tcpip.h> //функции для сервера 
#pragma comment(lib, "ws2_32.lib")
using namespace std;

SOCKET Connect;
SOCKET * Connections;
SOCKET Listen;
int ClientCount = 0;

int main()
{
	WSAData data;
	WORD version = MAKEWORD(2,2); //указываем версию

	int res = WSAStartup(version, &data);
	if (res != 0)
	{
		return 0;
	}

	struct addrinfo hints;
	struct addrinfo * result;

	ZeroMemory(&hints, sizeof(hints));

    Connections = new SOCKET[64*sizeof(SOCKET)];

	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(nullptr, "7770", &hints, &result);

	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(Listen, result->ai_addr, result->ai_addrlen);

	listen(Listen, SOMAXCONN);

	freeaddrinfo(result);

	cout << "Server starting...\n";
	char m_connect[] = "Connect - OK";

	for (;;Sleep(75))
	{
		if (Connect = accept(Listen, nullptr, nullptr))
		{
			cout << "Client connect...\n";
			Connections[ClientCount] = Connect;
			send(Connections[ClientCount], m_connect, sizeof(m_connect), NULL);
			ClientCount++;
		}
	}

	system("pause");
}




