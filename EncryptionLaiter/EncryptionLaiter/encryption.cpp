#include "encryption.h"


namespace laiter {
	
Encryption::Encryption() : loop_(8) //TODO
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

std::string Encryption::encrypt()
{
	srand(time(NULL));

	fin_.open(file_path_, std::ios::binary);
	if (set_buffer() == -1)
	{
		return std::string("-1");
	}
	fout_.open(file_path_ += ".lcrypt", std::ios::binary);
	cpu_ = buffer_size_ < cpu_ ? buffer_size_ : cpu_; // Check possibility of threading

	size_t counter = 0;
	while (fin_.good())
	{
		fin_.read(reinterpret_cast<char *>(&buffer_[0]), buffer_size_ * sizeof(size_t));

		size_t random_num = 100 + rand() % 900; // TODO: generate key

		for (int i = 0; i < loop_; i++) // TODO: function should support buffer size < file size
		{
			
			if (rand() % 2 == 0)
			{
				key_ += 'S' + std::to_string(random_num); 
				for (size_t i = 0; i < cpu_; i++)
				{
					threads_[i] = std::thread(std::for_each(buffer_.begin() +((buffer_size_ / cpu_) * i), buffer_.end() - ((buffer_size_ / cpu_) * (cpu_ - 1 - i)), 
						[=](MimicIntCryptCell64 &n) {switch_shift(n.cell, random_num); }));
				}
				for (size_t i = 0; i < cpu_; i++)
				{
					threads_[i].join();
				}
			}
			else
			{
				key_ += 'X' + std::to_string(random_num);
				for (size_t i = 0; i < cpu_; i++)
				{
					threads_[i] = std::thread(std::for_each(buffer_.begin() + ((buffer_size_ / cpu_) * i), buffer_.end() - ((buffer_size_ / cpu_) * (cpu_ - 1 - i)),
						[=](MimicIntCryptCell64 &n) { xor (n.num, random_num); }));
				}
				for (size_t i = 0; i < cpu_; i++)
				{
					threads_[i].join();
				}
			}
		}
		if (!fin_.good())
		{
			fout_.write(reinterpret_cast<char*>(&buffer_[0]), file_size_ - counter * buffer_size_ * sizeof(size_t));
			break;
		}
		fout_.write(reinterpret_cast<char *>(&buffer_[0]), buffer_size_ * sizeof(size_t));
		counter++;
	}

	
	

	return std::string(key_);
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

void Encryption::xor(uint32_t & source, const size_t random)
{
	source = source ^ random;
}

void Encryption::xor(uint64_t & source, const size_t random)
{
	source = source ^ random;
}

void Encryption::xor(CryptCell32 & source, const size_t random)
{
	static MimicIntCryptCell32 tmp;
	tmp.cell = source;
	tmp.num = tmp.num ^ random;
	source = tmp.cell;
}

void Encryption::xor(CryptCell64 & source, const size_t random)
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

int Encryption::set_file_size()
{
	if (fin_.is_open())
	{
		fin_.seekg(0, fin_.end);
		file_size_ = fin_.tellg();
		fin_.seekg(0, fin_.beg);
		return 0;
	}
	else
	{
		return -1;
	}
}

void Encryption::set_file_size(size_t file_size)
{
	file_size_ = file_size;
}

int Encryption::set_buffer_size()
{
	if (file_size_ == 0 && set_file_size() == -1) 
	{
		return -1;
	}
	else
	{
		if (file_size_ <= 1024000000) // 1 GB
		{
			buffer_size_ = file_size_ / sizeof(size_t) + 1; //TODO: x86 supp
		}
		else
		{
			buffer_size_ = 1024000000 / sizeof(size_t);
		}
		return 0;
	}
}

void Encryption::set_buffer_size(size_t buffer_size)
{
	buffer_size_ = buffer_size;
}

int Encryption::set_buffer()
{
	if (buffer_size_ > 0)
	{
		buffer_.resize(buffer_size_);
		return 0;
	}
	else if (set_buffer_size() == 0)
	{
		buffer_.resize(buffer_size_);
		return 0;
	}
	return -1;
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

fs::path Encryption::get_file_path() const
{
	return fs::path(file_path_);
}

size_t Encryption::get_file_size() const
{
	return size_t(file_size_);
}

size_t Encryption::get_loop() const
{
	return size_t(loop_);
}

std::string Encryption::get_key() const
{
	return std::string(key_);
}

size_t Encryption::get_threads() const
{
	return size_t(threads_.size());
}

size_t Encryption::get_cpu() const
{
	return size_t(cpu_);
}

size_t Encryption::get_real_cpu() const
{
	return size_t(std::thread::hardware_concurrency());
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
	set_file_size();
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