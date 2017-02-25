#include "encryption.h"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <thread>
#include <filesystem>
#include <fstream>
namespace fs = std::experimental::filesystem;
fs::path a;
void set_file_path(fs::path&& file_path)
{
	a = std::move(file_path);
}

void main()
{
	set_file_path("sadfasdf");
	fs::path file_path = ".";
	std::ifstream fin;
	std::ofstream fout;
	fin.open(file_path, std::ios::binary);

	while (!fin.is_open())
	{
		
		std::cout << "File's not found" << std::endl << "Your path is " << file_path << std::endl;
		if (is_directory(file_path))
		{
			std::cout << file_path << " is a directory containing:\n";
			
			for (auto& item : fs::directory_iterator(file_path))
			{
				std::cout << item << std::endl;
			}	
		}
		std::cout << "Enter full file's path or File name for files in program's directory \n.\\";
		std::cin >> file_path;
		fin.clear();
		fin.open(file_path, std::ios::binary);
	}
	if (file_path.extension() == ".lcrypt")
	{
		fout.open(file_path.replace_extension(""), std::ios::binary);
		file_path.replace_extension(".lcrypt");
	}
	else
	{
		fout.open(file_path += ".lcrypt", std::ios::binary);
	}

	fin.close();
	fout.close();
	
	system("pause");  
}
