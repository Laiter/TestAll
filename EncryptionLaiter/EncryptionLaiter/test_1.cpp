#include "laiter_encryption_test.h"
#include <iostream>
#include <string>
#include <chrono>

namespace lcrypt = ::laiter::crypt;

void test1()
{
	std::cout << "test decrypt\\encrypt. \nfile.rar size: 2,27 Gb (2 444 715 303 byte) \nbuffer size: all file \nuse MimicIntCryptCell32 \nhard drive type: HDD \nx64 build\n";
	auto start_time = std::chrono::steady_clock::now();													// chrono
	std::string filepath("D:\\GitProjects\\TestAll\\EncryptionLaiter\\EncryptionLaiter\\input.txt");
	std::ifstream fin(filepath.c_str(), std::fstream::binary);											
	std::ofstream fout("output.txt", std::fstream::binary);
	std::string key = lcrypt::Encrypt(fin, fout, 1);
	std::cout << key << std::endl;
	fin.close();
	fout.close();
	std::ifstream find("output.txt", std::fstream::binary);
	std::ofstream foutd("decrypt.txt", std::fstream::binary);
	lcrypt::Decrypt(find, foutd, key);
	find.close();
	foutd.close();
	auto end_time = std::chrono::steady_clock::now();													// chrono
	auto elapsed_ns = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);		    // chrono
	std::cout << "overall time: " << elapsed_ns.count() << " ns 8 loops\n";								// chrono
	system("pause");
}