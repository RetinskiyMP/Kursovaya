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
	char buff[BUFSIZ];
	int nsize;
public:
	void GetClientSettings();
	void ConnectToServer();
	void StartСommunication();
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
	if (!connect(s, (struct sockaddr*)&sin, sizeof(sin)))
	{
	}
	else
	{
		cout << "Server connection not established" << endl;
	}
}

void Client::StartСommunication()
{
	string q = "";
	string p = "";
	int bytes;

	bytes = recv(s, &buff[0], sizeof(buff), 0); //принимаем сообщение об успешном подключении
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << p;

	p = "";
	q = "223";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0); 
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "2|Moscow2|Piter|12.23.1998|12:30";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "2|Moscow|Piter4|12.23.1998|12:30";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "2|Moscow|Piter|12.a23.1998|12:30";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "2|Moscow|Piter|12.23.1998|12.30";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "2|Moscow|Piter|12.23.1998|12:30a";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "2|Moscow|Piter";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "2|Moscow|Piter|12.23.1998|12:30";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "4|10|Aleksand12|Alekseenko|23";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "4|10|Aleksand|Alek232seenko|23";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "4|10|Aleksand";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "4|100|Aleksand|Alekseenko|23";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "34|100|Aleksand|Alekseenko|23";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;

	p = "";
	q = "4|10|Aleksand|Alekseenko|23";
	cout << "S <= " << q;
	send(s, q.c_str(), q.length(), 0);
	bytes = recv(s, &buff[0], sizeof(buff), 0);
	for (int i = 0; i < bytes; ++i)
		p += buff[i];
	cout << "\nS => " << p;


	q = "0";
	send(s, q.c_str(), q.length(), 0); //отключаемся
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Client *client = new Client[1];

	for (int i = 0; i < 1; ++i)
	{
		client[i].GetClientSettings();
		client[i].ConnectToServer();
		client[i].StartСommunication();
	}

	return 0;
}

