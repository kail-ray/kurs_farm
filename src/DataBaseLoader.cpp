#include "DataBaseLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

DataBaseLoader::DataBaseLoader()
{
}

DataBaseLoader::DataBaseLoader(std::string newFilePath)
{
	filePath = newFilePath;
	loadData();
}

DataBaseLoader::~DataBaseLoader()
{
}

//Считать файл с пользователями в переменную кеша
void DataBaseLoader::loadData()
{
	std::string line;
	std::ifstream databasefile(filePath);

	//Открыть файл для чтения
	if (databasefile.is_open())
	{
		//Считать все записи в файле
		while (getline(databasefile, line))
		{
			size_t pos = 0;
			std::vector<std::string> token(3);
			int i = 0;

			//Разбить запись по пробелу
			while ((pos = line.find(delimiter)) != std::string::npos) {
			    token[i] = line.substr(0, pos);
			    line.erase(0, pos + delimiter.length());
			    i++;
			}
		    token[2] = line.substr(0, pos);

			std::string index = token[1];

			//Создать пользователя с полученным данными
			users[index].login = token[1];
			users[index].password = token[2];
	    }
	}

	databasefile.close();
}

//Вернуть пользователя по ID
User DataBaseLoader::getUser(std::string i)
{
	return users[i];
}

//Проверить наличие пользователя
bool DataBaseLoader::checkUserLogin(std::string checkLogin)
{
	bool result = false;

	auto it = users.begin();
	for (int i = 0; it != users.end(); it++, i++)
	{
		if(it->first == checkLogin)
		{
			result = true;
			break;
		}
	}

	return result;
}

//Вернуть пароль пользователя
std::string DataBaseLoader::getUserPassword(std::string checkLogin)
{
	return users[checkLogin].password;
}
