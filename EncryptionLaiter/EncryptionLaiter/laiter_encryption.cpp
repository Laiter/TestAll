#include "laiter_encryption.h"
namespace crypt {
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
	void XOR(CryptCell32& source, const int Rand) 
	{
		static MimicIntCryptCell32 tmp;
		tmp.cell = source;
		tmp.num = tmp.num ^ Rand;
		source = tmp.cell;
	}
};