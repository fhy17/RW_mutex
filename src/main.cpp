#include <iostream>
#include <string>

#include "read_write_mutex_test.h"

int main(){
	funcReadWriteMutex();

	std::cout<< "main thread" << std::endl;
	std::string s;
	s.append("123");


		
	return 0;
}
