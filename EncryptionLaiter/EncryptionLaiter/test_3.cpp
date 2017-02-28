#include "encryption.h"
#include <iostream>
#include <string>
#include <chrono>

void test(size_t i)
{
	std::cout << "test #" << i << std::endl;
	std::cout << "test decrypt\\encrypt. \nfile.rar size: 2,27 Gb (2 444 715 303 byte) \nbuffer size: " << 2048 / pow(2, i) << " MB \nuse MimicIntCryptCell64 \nhard drive type: HDD \nx64 build\n";																	// chrono
	std::cout << "encrypt started!" << std::endl;
	auto start_time = std::chrono::steady_clock::now();																// chrono
	laiter::Encryption A("учеба.rar", "", 8, 256000000 / pow(2, i));
	A.encrypt();
	auto end_time = std::chrono::steady_clock::now();																// chrono
	auto elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);						// chrono
	std::cout << "encrypt ended! time:" << elapsed_s.count() << " s\n";												// chrono

	std::cout << "decrypt started!" << std::endl;																	// chrono
	auto start_time1 = std::chrono::steady_clock::now();															// chrono
	A.decrypt();
	auto end_time1 = std::chrono::steady_clock::now();																// chrono
	auto elapsed_s1 = std::chrono::duration_cast<std::chrono::seconds>(end_time1 - start_time1);					// chrono
	std::cout << "decrypt ended! time:" << elapsed_s1.count() << " s\n";											// chrono
	std::cout << "overall time: " << elapsed_s.count() + elapsed_s1.count() << " s 8 loops\n";						// chrono
}

void main()
{
	for (size_t i = 1; i < 10; i++)
	{
		test(i);
		system("pause");
	}
	system("pause");
}