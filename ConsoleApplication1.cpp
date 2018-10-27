#include "pch.h"
#include <iostream>
#include <WinSock2.h> //сокеты 
#include <WS2tcpip.h> //функции для сервера 
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	SOCKET s;//Для сервера
	SOCKET connect;//Для клиента

	//Инициализируем процесс библиотеки ws2_32, вызвав функцию WSAStartup
	WSADATA WsaData;
	int err = WSAStartup(0x0101, &WsaData); //0х0101 - версия
	if (err == SOCKET_ERROR)
	{
		cout << "Error";
		return 1;
	}
	//Теперь объявление переменную типа SOCKET
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Задаем параметры для сокета(сервера)
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(7770);
	sin.sin_addr.s_addr = INADDR_ANY;

	//Запускаем сервер
	err = bind(s, (LPSOCKADDR)&sin, sizeof(sin));
	if (err == 0)
	{
		cout << "Server starting..." << endl;
	}
	else
	{
		cout << "Error";
		return 1;
	}

	//Ждем подключений 
	err = listen(s, SOMAXCONN);
	if (err == 0)
	{
		cout << "Listening..." << endl;
	}
	else
	{
		cout << "Error";
		return 1;
	}

	//Обработка подключений
	char m_connect[] = "Connect - OK"; //Сообщение для клиента
	/*for (;;Sleep(75))
	{
		if (connect = accept(s, nullptr, nullptr))
		{
			cout << "Client is connected..." << endl;
			//send(connect, m_connect, sizeof(m_connect), NULL);
		}
	}*/
	connect = accept(s, nullptr, nullptr);
	cout << "+";
}




