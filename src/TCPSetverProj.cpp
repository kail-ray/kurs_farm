//============================================================================
// Name        : TCPSetverProj.cpp
// Author      : Karamishev I.A.
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Cryptographer.h"
#include "TCPServer.h"
#include <iostream>
#include <unistd.h>
#include "UnitTest++/UnitTest++.h"

//Unit тесты
SUITE(EncryptTest)
{
	TEST(ValidKey)
			{
				//прописные
				CHECK_EQUAL("5F4DCC3B5AA765D61D8327DEB882CF99", Cryptographer().encrypt("password"));
				//Строчные
				CHECK_EQUAL("319F4D26E3C536B5DD871BB2C52E3178", Cryptographer().encrypt("PASSWORD"));
				//С пробелами и знаками препинания
				CHECK_EQUAL("D252F25E94BE3FBDF86A634561C10A84", Cryptographer().encrypt("PASS WORD!"));
				//С цифрами
				CHECK_EQUAL("6F0F4D469EAEAD0AC18DA3A460F263B6", Cryptographer().encrypt("PASSWORD123"));
				//Пустая строка
				CHECK_EQUAL("D41D8CD98F00B204E9800998ECF8427E", Cryptographer().encrypt(""));
				//Нет букв
				CHECK_EQUAL("202CB962AC59075B964B07152D234B70", Cryptographer().encrypt("123"));
			}
}

//Аргументы коандной строки
class Opts{
private:
	std::string logPath = "/var/log/vcalc.log"; //Путь к файлу с логами
	std::string confPath = "/etc/vcalc.conf"; //Путь к фа
	int port = 33333; //Номер порта
	void usage(const char* progName); //Вывод подсказки

public:
	Opts(int argc, char **argv);
	std::string getLogPath() { return logPath; }
	std::string getConPath() { return confPath; }
	int getPort() { return port; }
};
//Основная программа сервера. Точка входа в программу
int main(int argc, char **argv)
{
	//Выполнение только Unit тестов (расскоментировать, чтобы выполнить)
	//return UnitTest::RunAllTests();
    Opts op(argc, argv);

    //Заменить на передачу параментов из командной строки
    TCPServer server = TCPServer(op.getPort(),op.getLogPath(), op.getConPath());

    server.startServer();

   do
    {
      server.handle();
    }
   while(true);

   server.closeServer();

   return 0;
}


Opts::Opts(int argc, char **argv)
{
	int opt;

	//p - порт
	//l - файл логов
	//c - файл бд
	//h - справка
	while((opt = getopt(argc, argv, "p:l:c:h")) != -1)
	{
		switch(opt)
		{
			case 'p':
				port = strtol(optarg, nullptr, 10);
				break;

			case 'l':
				logPath = optarg;
				break;

			case 'c':
				confPath = optarg;
				break;

			case 'h':
			case '?':
			case ':':
				usage(argv[0]);
		}
	}
}

void Opts::usage(const char* progName)
{
	std::cout<<"Usage: "<<progName<<" [-p port] [-l log file path] [-c data base file path] [-h help] \n";
	exit(0);
}

