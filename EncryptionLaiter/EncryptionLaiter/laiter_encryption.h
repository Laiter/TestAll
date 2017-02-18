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
	//��� ��� �������������� ������� ����� int
	typedef union 
	{
		uint32_t num;
		CryptCell32 cell;
	}MimicIntCryptCell32;
	//���������� ������ ����� int �� ����� � ����. Loop �������� �� ���������� �������� 
	//�������������� ����� int. ������� ���������� ���� ���������� � string.
	std::string EncryptLaiter(std::ifstream& fin, std::ofstream& fout, const int loop);
	//���������� ������ ����� int �� ����� � ����. key - ����, ���������� �� ������� ���������� EncryptLaiter.
	void DecryptLaiter(std::ifstream& fin, std::ofstream& fout, std::string key);
	//�������� �� ������ � ������������� ������� ����� Source � ����������� �� Rand. 
	//decrypt �������� �� ����������� ����� ��������. 0 - �� ��������� ��� ����������, 1 - ��� ����������. 
	void SwSh(CryptCell32& source, const int Rand, const bool decrypt = 0);
	void XOR(CryptCell32& source, const int Rand);
};
#endif // !LAITER_ENCRYPTION_H
