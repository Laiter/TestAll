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
	//Тип для преобразований битовых полей int
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
	//Шифрование потока чисел int из файла в файл. Loop отвечает за количество операций 
	//преобразования числа int. Функция возвращает ключ дешифровки в string.
	std::string EncryptLaiter(std::ifstream& fin, std::ofstream& fout, const int loop);
	//Дешифровка потока чисел int из файла в файл. key - ключ, полученный от функции шифрования EncryptLaiter.
	void DecryptLaiter(std::ifstream& fin, std::ofstream& fout, std::string key);
	//Операции по сдвигу и перемешиванию битовых полей Source в зависимости от Rand. 
	//decrypt отвечает за направление таких операций. 0 - по умолчанию для шифрования, 1 - для дешифровки. 
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
