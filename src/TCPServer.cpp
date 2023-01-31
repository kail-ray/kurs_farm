
#define cplusplus

#include "TCPServer.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <iostream>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>


//Реализацтия конструктора
TCPServer::TCPServer(int newPort = 33333, std::string logPath = "/var/log/vcalc.log", std::string confPath = "/etc/vcalc.conf")
    {
        port = newPort;
        this_s = 0;

        logger = Logger(logPath);
        dataBase = DataBaseLoader(confPath);
    }

//Реализация деструктора
TCPServer::~TCPServer()
    {
        closeServer();
    }


//Функция обработки векторов
long long TCPServer::calcFunction(std::vector<long long> vector)
{
	long long sum = 0;

	for(int i = 0; i < vector.size(); i++)
	{
		sum += vector[i];
	}

	return sum;
}

//реализация запуска сервера
void TCPServer::startServer()
{
    sockaddr_in addr;
    int addr1 = sizeof(addr);
    //IP адрес, к которому будет привязан сокет.
    //INADDR_ANY - сервер доступен с любой пользовательской машины
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //Номер порта, занимаемого процессом
    addr.sin_port = htons(port);
    //Идентификатор семейства адресов
    //AF_INET - константа, отвечающая за то, что устройство использует глобальную сеть по протоколу IPv4
    addr.sin_family = AF_INET;

    //Создание сокета(домен, тип, протокол) возвращает дескриптор сокета
    //Sock_STREAM - передача данных с предварителбной установкой соединения.
    //Использует TCP протокол
    this_s = socket(AF_INET, SOCK_STREAM, 0);

    //Если -1 - сокет не был создан
    //Здесь и далее - в случае ошибки полностью выходим из программы, т.к. дальнейшая работа программы будет не возможна
    if(this_s < 0)
    {
        logger.WriteError("Socket not created", 1);
        exit(-1);
    }

    //Привязка сокета и номера порта и проверка ее успешности
    if(bind(this_s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        logger.WriteError("Socket bind failed", 1);
        exit(-1);
    }

    //Перевод сокета сервера в пассивный режим, где он ожидает, пока клиент
    //не подключится к серверу. Второй параметр - максимальная длина, которую может иметь очередь
    //ожидающих соединений для сокета.
    if(listen(this_s, SOMAXCONN) < 0)
    {
        logger.WriteError("Port listen failed", 1);
        exit(-1);
    }

    std::cout << "Start Server!\n";
}


//реализация закрытие сервера
void TCPServer::closeServer()
{
    close(this_s);
}

char * TCPServer::recvData(int socket)
{
    size_t len = 1024;
    char *  data = new char[len]{0};

	int res;

	try
	{
		while (res = recv(socket, (void*) data, len, MSG_PEEK) == len)
		{
			delete[] data;
			len *= 2;
			data = new char[len];
		}

		if(res = recv(socket, (void*) data, len, 0) == -1)
		{
			logger.WriteError("Can't receive message from client", 0);
			throw;
		}

		std::cout << "Data ";

		for(int i = 0; i < len; i++)
		{
			std::cout <<  /*(int)*/data[i];
		}

		std::cout << "\n";
	}
	catch(std::string & e)
	{
		logger.WriteError(e, 1);
		throw (e);
	}

	return data;
}

void TCPServer::sendData(int socket, void * data)
{
	if(!send(socket, data, sizeof data, 0) < 0)
	{
		logger.WriteError("Can't send data", 0);
		throw;
	}
}

//реализация основной логики
void TCPServer::handle()
{
    int acceptS;
    sockaddr_in * addr_c = new sockaddr_in;

    socklen_t addrlen = sizeof(addr_c);

    std::cout << "Wait client!\n";

    //ожидание клиента
    //установка соединения с клиентом
    if((acceptS = accept(this_s, (struct sockaddr*)&addr_c, &addrlen)) <= 0)
    {
    	logger.WriteError("Connection not create", 0);
    	return;
    }
    else
    {
        char * data;

        //Получение идентификатора ID
        //Здесь и далее - в случае ошибки не завершаем работу программы, а выходим в main и запкскаем следующую
        //итерацию цикла и снова ожидаем подключения, т.к. это ошибка сеанса
        try
        {
        	data = recvData(acceptS);
        }
        catch(std::string & e)
        {
        	return;
        }

    	if(!dataBase.checkUserLogin(data))
    	{
    		try
    		{
    			sendData(acceptS, &errorClientMsg);
    			logger.WriteError("Wrong User login", 0);
        		return;
    		}
    		catch(std::string & e)
    		{
    	        close(acceptS);
    			return;
    		}
    	}

    	//Вычисление числа salt16
        //При успешной проверке идентификатора в БД  передать случайное число SALT16
        unsigned long salt[64];

        for(int i = 0; i < 64; i++)
        {
        	salt[i] = (unsigned long)std::rand();
        }

        //Отправка числа salt16
        try
        {
        	sendData(acceptS, salt);
        }
        catch(std::string & e)
        {
        	//При ошибке передать строку ERR и разорвать соединение
            close(acceptS);
        	return;
        }

        //Получение от клиента HASH MD5(SALT16||PASSWORD)
        try
        {
        	data = recvData(acceptS);
        }
        catch(std::string & e)
        {
            close(acceptS);
        	return;
        }

        std::string gsm, result;
        std::string pass = "P@ssW0rd";

        CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result));

        CryptoPP::Weak1::MD5 hash;
        hash.Update((unsigned char*)&pass[0], pass.size());
        gsm.resize(hash.DigestSize());
        hash.Final((unsigned char*)&gsm[0]);

        CryptoPP::StringSource(gsm, true, new CryptoPP::Redirector(encoder));

        //Передать ОК при успешной аутентификации
        try
        {
        	sendData(acceptS, &okClientMsg);
        }
        catch(std::string & e)
        {
            //Передать ERR и разорвать соединение при ошибке
            close(acceptS);
        	return;
        }

        //Получить количество векторов
        try
        {
        	data = recvData(acceptS);
        }
        catch(std::string & e)
        {
        	close(acceptS);
        	return;
        }

        int vectorsCount = *(int *) data;


		printf("Data %d\n",  vectorsCount);

		//std::cout << "Start create vector\n";
        std::vector<long long> vector;
        //std::cout << "Start loop\n";
        //Цикл по количетсву
        try
        {
			for(int i = 0; i < vectorsCount; i++)
			{

				//std::cout << "Start sleep\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
				//std::cout << "End sleep\n";

				//Получить размер вектора
				try
				{
					//std::cout << "Try get lenght\n";
					data = recvData(acceptS);
				}
				catch(std::string & e)
				{
					close(acceptS);
					return;
				}

				int lenght = *(int *) data;


				printf("Data %d\n", lenght);

				vector.resize(lenght);
				long long* m;

				long long* testm = (long long *)&(((int *) data)[1]);

				//Получить значения вектора
				//if (testm != 0)
				{
					m = (long long *)&(((int *) data)[1]);
				}
				//else
				//{
					//try
					//{
						//data = recvData(acceptS);
					//}
					//catch(std::string & e)
					//{
						//close(acceptS);
						//return;
					//}

					//m = (long long *)&(((int *) data)[0]);
				//}

				for(int j = 0; j < lenght; j++)
				{
					vector[j] = m[j];
				}

				//Вычислить значение функции сервера
				long long res = calcFunction(vector);

			//Отправить результат
				try
				{
					sendData(acceptS, &res);
				}
				catch(std::string & e)
				{
					//Передать ERR и разорвать соединение при ошибке
					close(acceptS);
					return;
				}
			}
        }
        catch(std::string & e)
        {
        	logger.WriteError(e, 1);
        }

        //Разорвать соединение с клиентом
        close(acceptS);
    }
}

