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
// v2: Encrypter/Decrypter
// 1. write Encrypter that should read and encrypt any file. 
// 2. write Decrypter =/=
//******************************************************************************************
// v1: Encrypter/Decrypter
// 1. write Encrypter that should read and encrypt integers from file. 
// 2. write Decrypter =/=
#ifndef LAITER_ENCRYPTION_TEST_H
#define LAITER_ENCRYPTION_TEST_H
#include <string>
#include <ctime>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>

namespace laiter {
namespace crypt {
	typedef struct
	{
		uint32_t b10 : 5;
		uint32_t b11 : 5;
		uint32_t b12 : 5;
		uint32_t b13 : 5; //20
		uint32_t b20 : 6;
		uint32_t b21 : 6; //32
	}CryptCell32;
	typedef struct
	{
		uint64_t b10 : 10;
		uint64_t b11 : 10;
		uint64_t b12 : 10;
		uint64_t b13 : 10; //40
		uint64_t b20 : 12;
		uint64_t b21 : 12; //64
	}CryptCell64;
	//��� ��� �������������� ������� ����� int
	typedef union
	{
		uint32_t num;
		CryptCell32 cell;
	}MimicIntCryptCell32;
	typedef union
	{
		uint64_t num;
		CryptCell64 cell;
	}MimicIntCryptCell64;
	//���������� ������ ����� int �� ����� � ����. Loop �������� �� ���������� �������� 
	//�������������� ����� int. ������� ���������� ���� ���������� � string.
	std::string EncryptLaiter(std::ifstream& fin, std::ofstream& fout, const int loop);
	//���������� ������ ����� int �� ����� � ����. key - ����, ���������� �� ������� ���������� EncryptLaiter.
	void DecryptLaiter(std::ifstream& fin, std::ofstream& fout, std::string key);
	//�������� �� ������ � ������������� ������� ����� Source � ����������� �� Rand. 
	//decrypt �������� �� ����������� ����� ��������. 0 - �� ��������� ��� ����������, 1 - ��� ����������. 
	void SwSh(CryptCell32& source, const int Rand, const bool decrypt = 0);
	void SwSh(CryptCell64& source, const int Rand, const bool decrypt = 0);
	void XOR(uint32_t& source, const int Rand);
	void XOR(uint64_t& source, const int Rand);
	void XOR(CryptCell32& source, const int Rand);

	std::string Encrypt(std::ifstream& fin, std::ofstream& fout, const int loop);
	void Decrypt(std::ifstream& fin, std::ofstream& fout, std::string key);
}; // namespace crypt
}; // namespace laiter
#endif // !LAITER_ENCRYPTION_H
