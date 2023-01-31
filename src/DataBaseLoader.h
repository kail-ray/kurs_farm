#ifndef DATABASELOADER_H_
#define DATABASELOADER_H_

#include <iostream>
#include <stdio.h>
#include <map>

//Структура данных о пользователях
struct User
{
	std::string login;
	std::string password;
};

//Класс для доступа к данным о пользоваетлях
class DataBaseLoader {
public:
	DataBaseLoader();
	DataBaseLoader(std::string newfilePath);
	virtual ~DataBaseLoader();
	//Вернуть данные о пользователе по ID
	User getUser(std::string i);
	//Проверить логин пользователя
	bool checkUserLogin(std::string login);
	//Вернуть пароль пользователя
	std::string getUserPassword(std::string login);

private:
	//Путь к файлу
	std::string filePath;
	//закешированная база пользователей
	std::map<std::string, User> users;
	//Разделитель строки в файле
	std::string delimiter = " ";
	//Загрузить базу из файла
	void loadData();
};

#endif /* DATABASELOADER_H_ */
