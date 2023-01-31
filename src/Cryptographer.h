#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifndef CRYPTOGRAPHER_H_
#define CRYPTOGRAPHER_H_

class Cryptographer {
public:
	Cryptographer();
	virtual ~Cryptographer();
	std::string encrypt(std::string pass);
};

#endif /* CRYPTOGRAPHER_H_ */
