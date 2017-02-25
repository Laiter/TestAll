#include "encryption.h"


namespace laiter {
	
Encryption::Encryption() : loop_(2)
{
	cpu_ = std::thread::hardware_concurrency();
	threads_.resize(cpu_);
}

Encryption::Encryption(const fs::path file_path, const std::string key, size_t loop, size_t threads_num) : loop_(loop), file_path_(file_path), key_(key)
{
	cpu_ = threads_num == 0 ? std::thread::hardware_concurrency() : threads_num;
	threads_.resize(cpu_);
}


Encryption::~Encryption()
{
}

void Encryption::switch_shift(CryptCell32 & source, const size_t random, const bool decrypt)
{
	static CryptCell32 tmp = source;
	if (random % 2 > 0)
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

void Encryption::switch_shift(CryptCell64 & source, const size_t random, const bool decrypt)
{
	static CryptCell64 tmp = source;
	if (random % 2 > 0)
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

{
	source = source ^ random;
}

{
	source = source ^ random;
}

{
	static MimicIntCryptCell32 tmp;
	tmp.cell = source;
	tmp.num = tmp.num ^ random;
	source = tmp.cell;
}

{
	static MimicIntCryptCell64 tmp;
	tmp.cell = source;
	tmp.num = tmp.num ^ random;
	source = tmp.cell;
}

void Encryption::set_file_path(fs::path & file_path)
{
	file_path_ = file_path;
}

void Encryption::set_file_path(fs::path && file_path)
{
	file_path_ = std::move(file_path);
}

void Encryption::set_file_size(size_t file_size)
{
	file_size_ = file_size;
}

void Encryption::set_buffer()
{
}

void Encryption::set_loop(size_t loop)
{
	loop_ = loop;
}

void Encryption::set_key(std::string & key)
{
	key_ = key;
}

void Encryption::set_key(std::string && key)
{
	key_ = std::move(key);
}

void Encryption::set_threads()
{
	cpu_ == 0 ? set_cpu() : 0;
	threads_.resize(cpu_);
}

void Encryption::set_threads(size_t threads_num)
{
	threads_num == 0 ? set_cpu() : 0;
	threads_.resize(cpu_);
}

void Encryption::set_real_threads()
{
	set_real_cpu();
	threads_.resize(cpu_);
}

void Encryption::set_cpu()
{
	cpu_ = std::thread::hardware_concurrency();
}

void Encryption::set_cpu(size_t cpu_num)
{
	cpu_ = cpu_num;
}

void Encryption::set_real_cpu()
{
	cpu_ = std::thread::hardware_concurrency();
}



// Utility
void Encryption::open_file()
{
	fin_.open(file_path_, std::ios::binary);
	while (!fin_.is_open())
	{
		std::cout << "File's not found" << std::endl << "Your path is " << file_path_ << std::endl;
		if (is_directory(file_path_))
		{
			std::cout << file_path_ << " is a directory containing:\n";
			for (auto& item : fs::directory_iterator(file_path_))
			{
				std::cout << item << std::endl;
			}
		}
		std::cout << "Enter full file's path or File name for files in program's directory \n.\\";
		std::cin >> file_path_;
		fin_.clear();
		fin_.open(file_path_, std::ios::binary);
	}
	if (file_path_.extension() == ".lcrypt")
	{
		fout_.open(file_path_.replace_extension(""), std::ios::binary);
		file_path_.replace_extension(".lcrypt");
	}
	else
	{
		fout_.open(file_path_ += ".lcrypt", std::ios::binary);
	}
}

}; // namespace laiter
