#include "pch.h"
#include <iostream>
#include <WinSock2.h> //сокеты 
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	SOCKET s;

	//Инициализируем процесс библиотеки ws2_32, вызвав функцию WSAStartup
	WSADATA WsaData;
	int err = WSAStartup(0x0101, &WsaData); //0х0101 - версия
	if (err == SOCKET_ERROR)
	{
		cout << "Error";
		return 1;
	}
	//Теперь объявление переменную типа SOCKET
	s = socket(AF_INET, SOCK_STREAM, 0);

	//Задаем параметры для сокета(сервера)
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(7770);
	//sin.sin_addr.s_addr = inet_pton(AF_INET, "127.0.0.1", &(sin.sin_addr));
	inet_pton(AF_INET, "127.0.0.1", &(sin.sin_addr));
	

	cout << "+";
	connect(s, (struct sockaddr*)&sin, sizeof(sin));
	cout << "+";

	return 0;
}
