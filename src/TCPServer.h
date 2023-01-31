
#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include "Logger.h"
#include "DataBaseLoader.h"
#include <netinet/in.h>
#include <stdio.h>
#include <vector>
#include <string>

//Заголовочный файл

//Класс TCP Server
class TCPServer
{
public:
    //Конструктор
    TCPServer(int newPort, std::string logPath, std::string confPath);

    //Деструктора
    ~TCPServer();

    //запуск сервера
    void startServer();
    //отключение сервера
    void closeServer();
    //обработчик входящих  соединений
    void handle();

private:
    //порт
    unsigned short port;
    //сокет
    int this_s;
     //Логгер
    Logger logger;
    //База данных
    DataBaseLoader dataBase;
    //Функция обработки векторов
    long long calcFunction(std::vector<long long> vector);
    //Получение данных с клиента
    char * recvData(int socket);
    //Отрпавка данных на клиент
    void sendData(int socket, void  * data);
    //Сообщение ошибки, отправдяемое на клиент
    std::string errorClientMsg = "ERR";

    std::string okClientMsg = "OK";
};
#endif /* TCPSERVER_H_ */
