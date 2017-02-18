#include "laiter_encryption.h"
#include <iostream>
#include <string>

void main()
{
	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");
	std::string key = crypt::EncryptLaiter(fin, fout, 8);
	std::cout << key << std::endl;
	fin.close();
	fout.close();
	std::ifstream find("output.txt");
	std::ofstream foutd("decrypt.txt");
	crypt::DecryptLaiter(find, foutd, key);
	find.close();
	foutd.close();
	
	system("pause");  
}