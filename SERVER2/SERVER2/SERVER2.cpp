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
	int err; //для проверок
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
	static string AddFlight(sqlite3 *, char *, int len);
	static string DeleteFlight(sqlite3 *, char *, int len);
	static string AddPassenger(sqlite3 *, char *, int len);
	static string DeletePassenger(sqlite3 *, char *, int len);
	static string ShowDB(sqlite3 *);
	static bool CheсkCommand(char *, int len);
	static DWORD WINAPI Func(LPVOID);
};

Server::Server()
{}
Server::~Server()
{}

void Server::GetServerSettings()
{
	try
	{

		WSAStartup(0x0101, &WsaData); //Инициализируем процесс библиотеки ws2_32, вызвав функцию WSAStartup

		s = socket(AF_INET, SOCK_STREAM, 0); //Теперь объявление переменную типа SOCKET
		if (s == -1) throw - 1;
		//Задаем параметры для сокета(сервера)
		sin.sin_family = AF_INET;
		sin.sin_port = htons(7770);
		sin.sin_addr.s_addr = INADDR_ANY;
	}
	catch (int i)
	{
		cout << "Обнаружена ошибка в ходе работы функции socket, s  = " << i << endl;
		exit(0);
	}

	cout << "Сервер успешно настроен." << endl;
}

void Server::StartServer()
{
	try
	{
		err = bind(s, (LPSOCKADDR)&sin, sizeof(sin));
		if (err == -1) throw - 1;
	}
	catch (int i)
	{
		cout << "Обнаружена ошибка при системном вызове bind, err  = " << i << endl;
		exit(0);
	}
	cout << "Сервер успешно запущен." << endl;
}

void Server::StartListenning()
{
	try
	{
		err = listen(s, SOMAXCONN); //Ждем подключений
		if (err == -1) throw - 1;
	}
	catch (int i)
	{
		cout << "Обнаружена ошибка при системном вызове listen, err  = " << i << endl;
		exit(0);
	}

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
	//формируем ответ
	DATA += "\r\n==========Flights==========\r\n";
	if (sqlite3_prepare_v2(db, query1, -1, &stmt, 0) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW) //пока в бд есть данные происходит считывание
		{
			for (int i = 0; i < 5; ++i)
			{
				char *tmp = (char *)sqlite3_column_text(stmt, i);
				DATA += tmp;
				DATA += " | ";
			}
			DATA += "\r\n";
		}
	}
	DATA += "========Passengers========\r\n";
	if (sqlite3_prepare_v2(db, query2, -1, &stmt, 0) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW) //пока в бд есть данные происходит считывание
		{
			for (int i = 0; i < 5; ++i)
			{
				char *tmp = (char *)sqlite3_column_text(stmt, i);
				DATA += tmp;
				DATA += " | ";
			}
			DATA += "\r\n";
		}
	}
	return DATA;
}

string Server::AddFlight(sqlite3 *db, char *buf, int len)
{
	string result;

	cout << "Получен запрос на добавление рейса: ";
	for (int i = 2; i < len; ++i)
	{
		cout << buf[i];
	}
	cout << endl;

	string str[4];
	for (int i = 2, j = 0; i < len; ++i)
	{
		if (buf[i] == '|')
		{
			j++;
			continue;
		}
		str[j] += buf[i];
	}

	char *err = 0;
	string SQL = "insert into flights(from_point,to_point,date,time) values ('" + str[0] + "','" + str[1] + "','" + str[2] + "','" + str[3] + "')";

	if (sqlite3_exec(db, SQL.c_str(), 0, 0, &err))
	{
		fprintf(stderr, "Ошибка SQL: %s\n", err);
		result = "Error SQL: "; result += err; result += "\r\n";
		sqlite3_free(err);
		return result;
	}
	else
	{
		result = "Success\r\n";
		return result;
	}

}

string Server::DeleteFlight(sqlite3 *db, char *buf, int len)
{
	string result = "";

	cout << "Получен запрос на удаление рейса: ";
	for (int i = 2; i < len; ++i)
	{
		cout << buf[i];
	}
	cout << endl;

	string str;
	for (int i = 2, j = 0; i < len; ++i)
	{
		str += buf[i];
	}

	char *err = 0;
	string SQL = "DELETE FROM passengers WHERE id_flight=" + str;

	if (sqlite3_exec(db, SQL.c_str(), 0, 0, &err))
	{
		fprintf(stderr, "Ошибка SQL: %s\n", err);
		result = "Error SQL: "; result += err; result += "\r\n";
		sqlite3_free(err);
		return result;
	}

	char *err2 = 0;
	string SQL2 = "DELETE FROM flights WHERE id_flight=" + str;

	if (sqlite3_exec(db, SQL2.c_str(), 0, 0, &err2))
	{
		fprintf(stderr, "Ошибка SQL: %s\n", err2);
		result = "Error SQL: "; result += err2; result += "\r\n";
		sqlite3_free(err2);
		return result;
	}
	else
	{
		result = "Success\r\n";
		return result;
	}

}

string Server::AddPassenger(sqlite3 *db, char *buf, int len)
{
	string result;

	cout << "Получен запрос на добавление пассажира: ";
	for (int i = 2; i < len; ++i)
	{
		cout << buf[i];
	}
	cout << endl;

	string str[4];
	for (int i = 2, j = 0; i < len; ++i)
	{
		if (buf[i] == '|')
		{
			j++;
			continue;
		}
		str[j] += buf[i];
	}

	char *err = 0;
	string SQL = "PRAGMA foreign_keys=on;insert into passengers(id_flight,first_name,second_name,age) values ('" + str[0] + "','" + str[1] + "','" + str[2] + "','" + str[3] + "')";

	if (sqlite3_exec(db, SQL.c_str(), 0, 0, &err))
	{
		fprintf(stderr, "Ошибка SQL: %s\n", err);
		result = "Error SQL: "; result += err; result += "\r\n";
		sqlite3_free(err);
		return result;
	}
	else
	{
		result = "Success\r\n";
		return result;
	}
}

string Server::DeletePassenger(sqlite3 *db, char *buf, int len)
{
	string result;

	cout << "Получен запрос на удаление пассажира: ";
	for (int i = 2; i < len; ++i)
	{
		cout << buf[i];
	}
	cout << endl;

	string str;
	for (int i = 2, j = 0; i < len; ++i)
	{
		str += buf[i];
	}

	char *err = 0;
	string SQL = "DELETE FROM passengers WHERE id_passenger=" + str;

	if (sqlite3_exec(db, SQL.c_str(), 0, 0, &err))
	{
		fprintf(stderr, "Ошибка SQL: %s\n", err);
		result = "Error SQL: "; result += err; result += "\r\n";
		sqlite3_free(err);
		return result;
	}
	else
	{
		result = "Success\r\n";
		return result;
	}


}

bool Server::CheсkCommand(char *buff, int len)
{
	if (len == 0) return 0;
	if ((buff[0] > '5') || (buff[0] < '0'))  return 0;
	
	char i = buff[0];
	switch (i)
	{
	case '2':
	{
		if (buff[1] != '|') return 0;

		int kol = 0;
		for (int i = 2; i < len; ++i)
		{
			if (buff[i] == '|') kol++;
		}
		if (kol != 3) return 0;

		int razd = 0;
		kol = 0;

		for (int i = 2; i < len; ++i)
		{
			if (buff[i] == '|')
			{
				if (kol == 0) return 0;
				else { kol = 0; razd++; continue; }
			}
			
			if (razd == 0 || razd == 1) {
				if ((buff[i] < 'A') || ((buff[i] > 'Z') && (buff[i] < 'a')) || (buff[i] > 'z'))  return 0;
				kol++;
			}
			if (razd == 2) {
				if (kol == 0)
				{
					if (buff[i + 10] != '|') return 0;
					if ((buff[i + 2] != '.') || (buff[i + 5] != '.')) return 0;
				}
				if ((buff[i] < '0' || buff[i] > '9') && (buff[i] != '.')) return 0;
				kol++;
			}
			if (razd == 3) {
				if (kol == 0)
				{
					if (i + 5 != len) return 0;
					if (buff[i + 2] != ':') return 0;
				}
				if ((buff[i] < '0' || buff[i] > '9') && (buff[i] != ':')) return 0;
				kol++;
			}
		}
		return 1;
		break;
	}
	case '3':
	{
		if (buff[1] != '|') return 0;

		for (int i = 2; i < len; ++i)
		{
			if (buff[i] < '0' || buff[i] > '9') return 0;
		}
		return 1;
		break;
	}
	case '5':
	{
		if (buff[1] != '|') return 0;

		for (int i = 2; i < len; ++i)
		{
			if (buff[i] < '0' || buff[i] > '9') return 0;
		}
		return 1;
		break;
	}
	case '4':
	{
		if (buff[1] != '|') return 0;

		int kol = 0;
		for (int i = 2; i < len; ++i)
		{
			if (buff[i] == '|') kol++;
		}
		if (kol != 3) return 0;

		int razd = 0;
		kol = 0;

		for (int i = 2; i < len; ++i)
		{
			if (buff[i] == '|')
			{
				if (kol == 0) return 0;
				else { kol = 0; razd++; continue; }
			}

			if (razd == 0) {
				if (buff[i] < '0' || buff[i] > '9') return 0;
				kol++;
			}
			if (razd == 1 || razd == 2) {
				if ((buff[i] < 'A') || ((buff[i] > 'Z') && (buff[i] < 'a')) || (buff[i] > 'z'))  return 0;
				kol++;
			}
			if (razd == 3) {
				if ((buff[i] < '0' || buff[i] > '9') && (buff[i] != ':')) return 0;
				kol++;
			}
		}
		return 1;
		break;
	}
	}
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
	string p = "";
#define hi "You are connected to server...\n"
	send(my_sock, hi, sizeof(hi), 0);

	int bytes_recv;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) &&
		bytes_recv != SOCKET_ERROR)
	{
		if (CheсkCommand(&buff[0], bytes_recv))
		{
			if (buff[0] == '0')
			{
				cout << "Соединение с сокетом клиента [" << my_sock << "] разорвано." << endl;
				closesocket(my_sock);
				return 0;
			}
			if (buff[0] == '1')
			{
				p = ShowDB(db);
				send(my_sock, p.c_str(), p.length(), 0);
				p = "";
			}
			if (buff[0] == '2')
			{
				p = AddFlight(db, &buff[0], bytes_recv);
				send(my_sock, p.c_str(), p.length(), 0);
				p = "";
			}
			if (buff[0] == '3')
			{
				p = DeleteFlight(db, &buff[0], bytes_recv);
				send(my_sock, p.c_str(), p.length(), 0);
				p = "";
			}
			if (buff[0] == '4')
			{
				p = AddPassenger(db, &buff[0], bytes_recv);
				send(my_sock, p.c_str(), p.length(), 0);
				p = "";
			}
			if (buff[0] == '5')
			{
				p = DeletePassenger(db, &buff[0], bytes_recv);
				send(my_sock, p.c_str(), p.length(), 0);
				p = "";
			}
		}
		else
		{
			p = "Sent command error\r\n";
			send(my_sock, p.c_str(), p.length(), 0);
			p = "";
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
