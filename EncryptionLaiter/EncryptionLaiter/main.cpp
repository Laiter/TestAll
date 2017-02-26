#include "encryption.h"
#include "laiter_encryption_test.h"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <thread>
#include <filesystem>
#include <fstream>
//S193S736
namespace fs = std::experimental::filesystem;
fs::path a;
void set_file_path(fs::path&& file_path)
{
	a = std::move(file_path);
}

void main()
{
	laiter::Encryption A("input.txt", "", 1);
	std::string a = A.encrypt();
	std::cout << a;
	
	A.set_file_path("input.txt.lcrypt");
	A.decrypt();
	
	system("pause");  
}
