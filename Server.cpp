#include "pch.h"
#include <iostream>
#include <WinSock2.h> //сокеты 
#include <WS2tcpip.h> //функции для сервера 
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Server
{
protected:
	SOCKET s;//Для сервера
	WSADATA WsaData;
	SOCKADDR_IN sin;
	SOCKET client_socket; // сокет для клиента
	SOCKADDR_IN client_addr; // адрес клиента (заполняется системой)
public:
	Server();
	~Server();
	void GetServerSettings();
	void StartServer();
	void StartListenning();
	static DWORD WINAPI Func(LPVOID);
};

Server::Server()
{}
Server::~Server()
{}

void Server::GetServerSettings()
{
	WSAStartup(0x0101, &WsaData); //Инициализируем процесс библиотеки ws2_32, вызвав функцию WSAStartup
	
	s = socket(AF_INET, SOCK_STREAM, 0); //Теперь объявление переменную типа SOCKET

	//Задаем параметры для сокета(сервера)
	sin.sin_family = AF_INET;
	sin.sin_port = htons(7770);
	sin.sin_addr.s_addr = INADDR_ANY;

	cout << "Сервер успешно настроен." << endl;
}

void Server::StartServer()
{
	bind(s, (LPSOCKADDR)&sin, sizeof(sin));
	cout << "Сервер успешно запущен." << endl;
}

void Server::StartListenning()
{
	listen(s, SOMAXCONN); //Ждем подключений

	int client_addr_size = sizeof(client_addr);

	cout << "Ожидание подключения..." << endl;

	while ((client_socket = accept(s, (sockaddr *)&client_addr, \
		&client_addr_size)))
	{
		cout << "Сокет клиента [" << client_socket << "] подключен, обработка..." << endl;

		DWORD thID;
		CreateThread(NULL, NULL, Func, &client_socket, NULL, &thID);
	}

}

DWORD WINAPI Server::Func(LPVOID client_socket)
{
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	char buff[BUFSIZ]; //char buff[20 * 1024];
#define sHELLO "Сокет подключен\r\n"

// отправляем клиенту приветствие
	send(my_sock, sHELLO, sizeof(sHELLO), 0);

	// цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
	int bytes_recv;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) &&
		bytes_recv != SOCKET_ERROR)
	{
		send(my_sock, &buff[0], bytes_recv, 0);
	}

	cout << "Соединение с сокетом клиента [" << my_sock << "] разорвано." << endl;
	// если мы здесь, то произошел выход из цикла по причине
	// возращения функцией recv ошибки - соединение с клиентом разорвано

	// закрываем сокет
	closesocket(my_sock);
	return 0;
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Server Server1;

	Server1.GetServerSettings();
	Server1.StartServer();
	Server1.StartListenning();
}
