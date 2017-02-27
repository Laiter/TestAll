#include "encryption.h"
#include "laiter_encryption_test.h"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <thread>
#include <filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

void main()
{

	laiter::Encryption A("input.txt", "", 8); // учеба.rar input.txt
	std::string a = A.encrypt();
	std::cout << a;
	
	A.set_file_path("input.txt.lcrypt");
	A.decrypt();
	
	system("pause");  
}
