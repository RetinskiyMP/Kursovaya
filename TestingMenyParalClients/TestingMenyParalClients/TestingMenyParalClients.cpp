#include <iostream>
#include <string>
#include <WinSock2.h> 
#include <WS2tcpip.h>
#include <conio.h>
#include <iomanip>
#pragma comment(lib, "ws2_32.lib")


using namespace std;

class Client
{
private:
	SOCKET s;
	WSADATA WsaData;
	SOCKADDR_IN sin;
	int nsize;
public:
	void GetClientSettings();
	void ConnectToServer();
	void StartСommunication();
	static DWORD WINAPI Func(LPVOID);
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
	HANDLE hThreads;
	SOCKET client;
	if (!(client = connect(s, (struct sockaddr*)&sin, sizeof(sin))))
	{
	}
	else
	{
		cout << "Server connection not established" << endl;
	}
}

void Client::StartСommunication()
{
	HANDLE hThreads;
	DWORD thID;
	hThreads = CreateThread(NULL, NULL, Func, &s, NULL, &thID);
	if (NULL == hThreads)
	{
		cout << "Error";
	}
}

DWORD WINAPI Client::Func(LPVOID client)
{
	/*
	SOCKET my_sock;
	my_sock = ((SOCKET *)client)[0];

	char buff[1024];
	string q = "";
	string p = "";
	int bytes;

	bytes = recv(my_sock, &buff[0], sizeof(buff), 0); //принимаем сообщение об успешном подключении
	for (int i = 0; i < bytes; ++i)
		p += buff[i];

	p = "";
	q = "4|2|Ira|Trof|20";
	send(my_sock, q.c_str(), q.length(), 0); //запрашивает бд
	bytes = recv(my_sock, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];

	cout << p;*/
	for (;;);
	/*q = "0";
	send(my_sock, q.c_str(), q.length(), 0); //отключаемся*/
	return 0;
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Client *client = new Client[10];

	for (int i = 0; i < 10; ++i)
	{
		client[i].GetClientSettings();
		client[i].ConnectToServer();
		client[i].StartСommunication();
		cout << "Client " << i + 1 << " connected." << endl;
	}
	
	for (;;);
	cout << "kak tak?";
	return 0;
}

