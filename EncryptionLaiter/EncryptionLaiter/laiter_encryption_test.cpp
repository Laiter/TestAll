///////////////////////////////////////////////////////////////////////////////////
//Copyright(c) 2017 Laiter. @Vladislav Sarychev
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//http ://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.
///////////////////////////////////////////////////////////////////////////////////
//author: @Vladislav Sarychev
//version: 1.0 28.02.17

//old
//DEPRECATED
//******************************************************************************************
// Task2: Encrypter/Decrypter
// 1. write Encrypter that should read and encrypt any file. 
// 2. write Decrypter =/=
#include "laiter_encryption_test.h"
#include <iostream>
#include <chrono>
#include <filesystem>
namespace laiter {
namespace crypt {
	std::string Encrypt(std::ifstream& fin, std::ofstream& fout, const int loop)
	{
		std::cout << "encrypt started!" << std::endl;																	// chrono
		auto start_time = std::chrono::steady_clock::now();																// chrono
		srand(time(NULL));
		std::string key;
		fin.seekg(0, fin.end);
		size_t length = fin.tellg();
		fin.seekg(0, fin.beg);
		std::vector<MimicIntCryptCell64> data(length / 8 + 1);
		fin.read(reinterpret_cast<char *>(&data[0]), length);
		for (int i = 0; i < loop; i++)
		{
			int random_num = rand() % 1000;
			if (rand() % 2 == 0)
			{
				std::for_each(data.begin(), data.end(), [random_num](MimicIntCryptCell64 &n) {SwSh(n.cell, random_num); });
				key += 'S' + std::to_string(random_num);
			}
			else
			{
				std::for_each(data.begin(), data.end(), [random_num](MimicIntCryptCell64 &n) {XOR(n.num, random_num); });
				key += 'X' + std::to_string(random_num);
			}
			auto end_time_loop = std::chrono::steady_clock::now();														// chrono
			auto elapsed_loop = std::chrono::duration_cast<std::chrono::seconds>(end_time_loop - start_time);		    // chrono
			std::cout << "loop " << i + 1 << " time:" << elapsed_loop.count() << " ns\n";								// chrono
		}
		fout.write(reinterpret_cast<char *>(&data[0]), length);
		auto end_time = std::chrono::steady_clock::now();																// chrono
		auto elapsed_ns = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);						// chrono
		std::cout << "encrypt ended! time:" << elapsed_ns.count() << " ns\n";											// chrono
		return key;
	}

	void Decrypt(std::ifstream& fin, std::ofstream& fout, std::string key)
	{
		int i = 1;
		std::cout << "decrypt started!" << std::endl;																	// chrono
		auto start_time = std::chrono::steady_clock::now();																// chrono
		fin.seekg(0, fin.end);
		size_t length = fin.tellg();
		fin.seekg(0, fin.beg);
		std::vector<MimicIntCryptCell64> data(length / 8 + 1);
		fin.read(reinterpret_cast<char *>(&data[0]), length);
		while (1)
		{
			size_t found = key.find_last_of("SX");
			if (found == std::string::npos) break;
			int random_num = std::stoi(key.substr(found + 1));
			if (key.at(found) == 'S')
			{
				std::for_each(data.begin(), data.end(), [random_num](MimicIntCryptCell64 &n) {SwSh(n.cell, random_num, true); });
				key.resize(found);
			}
			else
			{
				std::for_each(data.begin(), data.end(), [random_num](MimicIntCryptCell64 &n) {XOR(n.num, random_num); });
				key.resize(found);
			}
			auto end_time_loop = std::chrono::steady_clock::now();														// chrono
			auto elapsed_loop = std::chrono::duration_cast<std::chrono::seconds>(end_time_loop - start_time);		    // chrono
			std::cout << "loop " << i++ << " time:" << elapsed_loop.count() << " ns\n";									// chrono
		}
		fout.write(reinterpret_cast<char *>(&data[0]), length);
		auto end_time = std::chrono::steady_clock::now();																// chrono
		auto elapsed_ns = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);						// chrono
		std::cout << "decrypt ended! time:" << elapsed_ns.count() << " ns\n";											// chrono
	}

	void XOR(uint32_t& source, const int Rand)
	{
		source = source ^ Rand;
	}

	void XOR(uint64_t& source, const int Rand)
	{
		source = source ^ Rand;
	}

	void SwSh(CryptCell32& source, const int Rand, const bool decrypt)
	{
		static CryptCell32 tmp = source;
		if (Rand % 10 > 4)
		{
			if (decrypt == 1)
			{
				tmp.b13 = source.b13;
				source.b13 = source.b12;
				source.b12 = source.b11;
				source.b11 = source.b10;
				source.b10 = tmp.b13;
			}
			else
			{
				tmp.b10 = source.b10;
				source.b10 = source.b11;
				source.b11 = source.b12;
				source.b12 = source.b13;
				source.b13 = tmp.b10;
			}
			tmp.b20 = source.b20;
			source.b20 = source.b21;
			source.b21 = tmp.b20;
		}
		else
		{
			tmp.b10 = source.b10;
			source.b10 = source.b11;
			source.b11 = tmp.b10;
			tmp.b12 = source.b12;
			source.b12 = source.b13;
			source.b13 = tmp.b12;
		}
	}

	void SwSh(CryptCell64& source, const int Rand, const bool decrypt)
	{
		static CryptCell64 tmp = source;
		if (Rand % 10 > 4)
		{
			if (decrypt == 1)
			{
				tmp.b13 = source.b13;
				source.b13 = source.b12;
				source.b12 = source.b11;
				source.b11 = source.b10;
				source.b10 = tmp.b13;
			}
			else
			{
				tmp.b10 = source.b10;
				source.b10 = source.b11;
				source.b11 = source.b12;
				source.b12 = source.b13;
				source.b13 = tmp.b10;
			}
			tmp.b20 = source.b20;
			source.b20 = source.b21;
			source.b21 = tmp.b20;
		}
		else
		{
			tmp.b10 = source.b10;
			source.b10 = source.b11;
			source.b11 = tmp.b10;
			tmp.b12 = source.b12;
			source.b12 = source.b13;
			source.b13 = tmp.b12;
		}
	}

	//******************************************************************************************
	// Task1: Encrypter/Decrypter
	// 1. write Encrypter that should read and encrypt integers from file. 
	// 2. write Decrypter =/=
	std::string EncryptLaiter(std::ifstream& fin, std::ofstream& fout, const int loop) 
	{
		srand(time(NULL));
		std::string key;
		MimicIntCryptCell32 temp;
		std::vector<int> arrFin;
		while (fin >> temp.num) 
		{
			arrFin.push_back(temp.num);
		}
		for (int i = 0; i < loop; i++) 
		{
			int Rand = rand() % 1000;
			if (rand() % 2 == 0) 
			{
				for (std::vector<int>::iterator it = arrFin.begin(); it != arrFin.end(); it++) 
				{
					temp.num = *it;
					SwSh(temp.cell, Rand);
					*it = temp.num;
				}
				key.push_back('S');
				key += std::to_string(Rand);
			}
			else 
			{
				for (std::vector<int>::iterator it = arrFin.begin(); it != arrFin.end(); it++) 
				{
					temp.num = *it;
					XOR(temp.cell, Rand);
					*it = temp.num;
				}
				key.push_back('X');
				key += std::to_string(Rand);
			}
		}
		for (std::vector<int>::iterator it = arrFin.begin(); it != arrFin.end(); it++) 
		{
			fout << *it << " ";
		}
		return key;
	}
	void DecryptLaiter(std::ifstream& fin, std::ofstream& fout, std::string key) 
	{
		MimicIntCryptCell32 temp;
		std::vector<int> arrFin;
		while (fin >> temp.num) 
		{
			arrFin.push_back(temp.num);
		}
		while(1) 
		{
			std::size_t found = key.find_last_of("SX");
			if (found == std::string::npos) break;
			int Rand = std::stoi(key.substr(found+1));
			if (key.at(found) == 'S') 
			{
				for (std::vector<int>::iterator it = arrFin.begin(); it != arrFin.end(); it++) 
				{
					temp.num = *it;
					SwSh(temp.cell, Rand, 1);
					*it = temp.num;
				}
				key.resize(found);
			}
			else 
			{
				for (std::vector<int>::iterator it = arrFin.begin(); it != arrFin.end(); it++) 
				{
					temp.num = *it;
					XOR(temp.cell, Rand);
					*it = temp.num;
				}
				key.resize(found);
			}
		}
		for (std::vector<int>::iterator it = arrFin.begin(); it != arrFin.end(); it++) 
		{
			fout << *it << " ";
		}
	}
	
	void XOR(CryptCell32& source, const int Rand)
	{
		static MimicIntCryptCell32 tmp;
		tmp.cell = source;
		tmp.num = tmp.num ^ Rand;
		source = tmp.cell;
	}
}; // namespace crypt
}; // namespace laiter