#ifndef LOGGER_H_
#define LOGGER_H_
#include <iostream>

//Класс для рабоыт с логом
class Logger {
public:
	Logger();
	Logger(std::string filePath);
	virtual ~Logger();
	//Создать запись в логах
	void WriteError(std::string error, int criticalCode);

private:
	//Путь к файлу
	std::string filePath;
};

#endif /* LOGGER_H_ */
