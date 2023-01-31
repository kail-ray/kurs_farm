#include "Logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>


Logger::Logger()
{
}

Logger::Logger(std::string newFilePath)
{
	filePath = newFilePath;
}

Logger::~Logger()
{
}

//Создание записи в логах ошибок
void Logger::WriteError(std::string error, int criticalCode){

	std::string criticalString;

	//Критичность ошибки
	switch(criticalCode)
	{
		case 0:
			criticalString = "Not critical";
			break;
		case 1:
			criticalString = "Critical";
			break;
	}

	//Получение текущего времени
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	//Открытие файла для записи и запись в файл
	std::ofstream logfile;
	logfile.open (filePath,std::fstream::out | std::fstream::app);
	logfile << "-time: " << std::ctime(&end_time)
			<< "\t error: " << error
			<< " | critical: " << criticalString << ";\n";
	logfile.close();
}
