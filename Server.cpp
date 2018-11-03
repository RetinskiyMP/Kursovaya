#include "pch.h"
#include <iostream>
#include <WinSock2.h> //сокеты 
#include <WS2tcpip.h> //функции для сервера 
#pragma comment(lib, "ws2_32.lib")

using namespace std;

DWORD WINAPI Func(LPVOID client_socket)
{
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	char buff[BUFSIZ];//char buff[20 * 1024];
#define sHELLO "SOCKET PODKLUCHEN\r\n"

// отправляем клиенту приветствие
	send(my_sock, sHELLO, sizeof(sHELLO), 0);

	// цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
	int bytes_recv;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) &&
		bytes_recv != SOCKET_ERROR)
		//*tmp_buff = buff[0];
		//changeWords();

		send(my_sock, &buff[0], bytes_recv, 0);

	// если мы здесь, то произошел выход из цикла по причине
	// возращения функцией recv ошибки - соединение с клиентом разорвано
	
		// закрываем сокет
		closesocket(my_sock);
	return 0;
}

int main()
{
	SOCKET s;//Для сервера

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
	SOCKET client_socket; // сокет для клиента
	sockaddr_in client_addr; // адрес клиента (заполняется системой)

	int client_addr_size = sizeof(client_addr);
	char buffer[] = "connect - ok";

	while ((client_socket = accept(s, (sockaddr *)&client_addr, \
		&client_addr_size)))
	{
		cout << "Client is connected.";

		DWORD thID;
		CreateThread(NULL, NULL, Func, &client_socket, NULL, &thID);

		send(s, buffer, sizeof(buffer), NULL);
	}
}
