#include "pch.h"
#include <iostream>
#include <WinSock2.h> //сокеты 
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Client
{
protected:
	SOCKET s;
	WSADATA WsaData;
	SOCKADDR_IN sin;
	char buff[BUFSIZ];
	int nsize;
public:
	void GetClientSettings();
	void ConnectToServer();
	void Сommunication();

};

void Client::GetClientSettings()
{
	WSAStartup(0x0101, &WsaData); //Инициализируем процесс библиотеки ws2_32, вызвав функцию WSAStartup

	s = socket(AF_INET, SOCK_STREAM, 0); //Теперь объявление переменную типа SOCKET

	//Задаем параметры для сокета(сервера)
	sin.sin_family = AF_INET;
	sin.sin_port = htons(7770);
	inet_pton(AF_INET, "127.0.0.1", &(sin.sin_addr));
}

void Client::ConnectToServer()
{
	connect(s, (struct sockaddr*)&sin, sizeof(sin));
}

void Client::Сommunication()
{
	while ((nsize = recv(s, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
	{
		// ставим завершающий ноль в конце строки
		buff[nsize] = 0;

		// выводим на экран
		printf("S=>C:%s", buff);

		// читаем пользовательский ввод с клавиатуры
		printf("S<=C:"); fgets(&buff[0], sizeof(buff) - 1, stdin);

		// передаем строку клиента серверу
		send(s, &buff[0], strlen(&buff[0]), 0);
	}
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Client Client1;

	Client1.GetClientSettings();
	Client1.ConnectToServer();
	Client1.Сommunication();

	return 0;
}
