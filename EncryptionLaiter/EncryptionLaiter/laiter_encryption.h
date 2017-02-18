#ifndef LAITER_ENCRYPTION_H
#define LAITER_ENCRYPTION_H
#include <string>
#include <ctime>
#include <fstream>
#include <vector>
#include <cstdlib>
namespace crypt {
	typedef struct 
	{
		uint32_t b10 : 4;
		uint32_t b11 : 4;
		uint32_t b12 : 4; 
		uint32_t b13 : 4; //16
		uint32_t b20 : 8;
		uint32_t b21 : 8; //32
	}CryptCell32;
	//Тип для преобразований битовых полей int
	typedef union 
	{
		uint32_t num;
		CryptCell32 cell;
	}MimicIntCryptCell32;
	//Шифрование потока чисел int из файла в файл. Loop отвечает за количество операций 
	//преобразования числа int. Функция возвращает ключ дешифровки в string.
	std::string EncryptLaiter(std::ifstream& fin, std::ofstream& fout, const int loop);
	//Дешифровка потока чисел int из файла в файл. key - ключ, полученный от функции шифрования EncryptLaiter.
	void DecryptLaiter(std::ifstream& fin, std::ofstream& fout, std::string key);
	//Операции по сдвигу и перемешиванию битовых полей Source в зависимости от Rand. 
	//decrypt отвечает за направление таких операций. 0 - по умолчанию для шифрования, 1 - для дешифровки. 
	void SwSh(CryptCell32& source, const int Rand, const bool decrypt = 0);
	void XOR(CryptCell32& source, const int Rand);
};
#endif // !LAITER_ENCRYPTION_H
