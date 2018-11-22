#include <iostream>
#include <string>
#include <WinSock2.h> //сокеты 
#include <WS2tcpip.h> //функции для сервера 
#include "sqlite3.h"
#pragma comment(lib, "sqlite3.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Server
{
private:
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
	static string ShowDB(sqlite3 *);
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

string Server::ShowDB(sqlite3 *db)
{
	string DATA = "";
	sqlite3_stmt *stmt;
	char query1[] = "SELECT * FROM flights;";
	char query2[] = "SELECT * FROM passengers;";
	DATA += "Flights:\n\r";
	if (sqlite3_prepare_v2(db, query1, -1, &stmt, 0) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW) //пока в бд есть данные происходит считывание
		{
			char *id = (char *)sqlite3_column_text(stmt, 0);
			DATA += id;
			DATA += "|";
			char *from_point = (char *)sqlite3_column_text(stmt, 1);
			DATA += from_point;
			DATA += "|";
			char *to_point = (char *)sqlite3_column_text(stmt, 2);
			DATA += to_point;
			DATA += "|";
			char *date = (char *)sqlite3_column_text(stmt, 3);
			DATA += date;
			DATA += "|";
			char *time = (char *)sqlite3_column_text(stmt, 4);
			DATA += time;
			DATA += "|\n";
		}
	}
	DATA += "Passengers:\n";
	if (sqlite3_prepare_v2(db, query2, -1, &stmt, 0) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW) //пока в бд есть данные происходит считывание
		{
			char *id = (char *)sqlite3_column_text(stmt, 0);
			DATA += id;
			DATA += "|";
			char *id1 = (char *)sqlite3_column_text(stmt, 1);
			DATA += id1;
			DATA += "|";
			char *name1 = (char *)sqlite3_column_text(stmt, 2);
			DATA += name1;
			DATA += "|";
			char *name2 = (char *)sqlite3_column_text(stmt, 3);
			DATA += name2;
			DATA += "|";
			char *age = (char *)sqlite3_column_text(stmt, 4);
			DATA += age;
			DATA += "|\n";
		}
	}
	return DATA;
}

DWORD WINAPI Server::Func(LPVOID client_socket)
{
	//подключение клиента к бд
	sqlite3 *db; // хэндл объекта соединение к БД
	if (sqlite3_open("database.dblite", &db))
		fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
	
	//получение сокета клиента
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];

	//Общение с клиентом
	char buff[BUFSIZ]; //char buff[20 * 1024];
    #define hi "You are connected to server...\n"
	send(my_sock, hi, sizeof(hi), 0);

	int bytes_recv;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) &&
		bytes_recv != SOCKET_ERROR)
	{
		if (buff[0] == '0')
		{
			cout << "Соединение с сокетом клиента [" << my_sock << "] разорвано." << endl;
			closesocket(my_sock);
			return 0;
		}
		if (buff[0] == '1')
		{
			string a = ShowDB(db);
			send(my_sock, a.c_str(), a.length(), 0);
		}

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
	//Получаем настройки сокета для сервера
	Server1.GetServerSettings();
	//Запускаем сервер
	Server1.StartServer();
	//Начинаем обработку подключений
	Server1.StartListenning();
	
	
}
