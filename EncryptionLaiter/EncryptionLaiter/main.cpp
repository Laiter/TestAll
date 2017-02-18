#include "laiter_encryption.h"
#include <iostream>
#include <string>

namespace lcrypt = ::laiter::crypt;

void main()
{
	std::ifstream fin("input.txt", std::fstream::binary);
	std::ofstream fout("output.txt", std::fstream::binary);
	std::string key = lcrypt::Encrypt(fin, fout, 8);
	std::cout << key << std::endl;
	fin.close();
	fout.close();
	std::ifstream find("output.txt", std::fstream::binary);
	std::ofstream foutd("decrypt.txt", std::fstream::binary);
	lcrypt::Decrypt(find, foutd, key);
	find.close();
	foutd.close();
	
	system("pause");  
}