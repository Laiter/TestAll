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
#include "encryption.h"


namespace laiter {
	
Encryption::Encryption() : loop_(8), file_size_(0), buffer_size_(0)
{
	file_path_ = ".";
	cpu_ = std::thread::hardware_concurrency();
	threads_.resize(cpu_);
}

Encryption::Encryption(const fs::path file_path, const std::string key, size_t loop, size_t buffer_size, size_t threads_num) : loop_(loop), key_(key), buffer_size_(buffer_size)
{
	file_path_ = file_path;
	cpu_ = threads_num == 0 ? std::thread::hardware_concurrency() : threads_num;
	threads_.resize(cpu_);
}

Encryption::~Encryption()
{
}

// Encryption methods
std::string Encryption::encrypt() 
{								  
	if (!get_ready())										// Check needed fields of class if miss something, try to set
	{														// if can't set return
		return std::string("zero buffer");
	}
	size_t counter = 0;										// Count number of iterations of buffer read\write to count tail of file 
	size_t found;											// Position for key_ interpretation
	size_t random_num;										// Container for key_ data
	while (fin_.good())										// break when read all file
	{
		fin_.read(reinterpret_cast<char *>(&buffer_[0]), buffer_size_ * sizeof(size_t));
		found = -1;
		while (1)											// break when reach end of key_
		{
			found = key_.find_first_of("SX", found + 1);	// interpretate key_
			if (found == std::string::npos) break;
			random_num = std::stoi(key_.substr(found + 1, 3));
			if (key_.at(found) == 'S')						
			{
				for (size_t i = 0; i < cpu_; i++)			// devide tasks for each thread
				{											// this brunch use switch_shift method
					threads_[i] = std::thread([=]() {thread_task_ss(i, random_num); });
				}
				for (size_t i = 0; i < cpu_; i++)			// wait for all threads are complete
				{
					threads_[i].join();
				}
			}
			else
			{
				for (size_t i = 0; i < cpu_; i++)			// devide tasks for each thread
				{											// this brunch use xor method
					threads_[i] = std::thread([=]() {thread_task_xor(i, random_num); });
				}
				for (size_t i = 0; i < cpu_; i++)			// wait for all threads are complete
				{
					threads_[i].join();
				}
			}
		}
		if (!fin_.good())									// if reach end of file write tail
		{
			fout_.write(reinterpret_cast<char*>(&buffer_[0]), file_size_ - counter * buffer_size_ * sizeof(size_t));
			break;
		}
		fout_.write(reinterpret_cast<char *>(&buffer_[0]), buffer_size_ * sizeof(size_t));
		counter++;
	}
	fin_.close();
	fout_.close();
	return std::string(key_);
}

void Encryption::decrypt()
{
	if (!get_ready(true))									// Check needed fields of class if miss something, try to set
	{														// if can't set return
		return void();
	}
	size_t counter = 0;										// Count number of iterations of buffer read\write to count tail of file 
	size_t found;											// Position for key_ interpretation
	size_t random_num;										// Container for key_ data
	while (fin_.good())										// break when read all file
	{
		fin_.read(reinterpret_cast<char *>(&buffer_[0]), buffer_size_ * sizeof(size_t));
		found = key_.size();
		while (1)											// break when reach end of key_
		{
			found = key_.substr(0, found).find_last_of("SX");// interpretate key_
			if (found == std::string::npos) break;
			random_num = std::stoull(key_.substr(found + 1, 3));
			if (key_.at(found) == 'S')
			{
				for (size_t i = 0; i < cpu_; i++)			// devide tasks for each thread
				{											// this brunch use switch_shift method with decrypt mode
					threads_[i] = std::thread([=]() {thread_task_ss(i, random_num, true); });
				}
				for (size_t i = 0; i < cpu_; i++)			// wait for all threads are complete
				{
					threads_[i].join();
				}
			}
			else
			{
				for (size_t i = 0; i < cpu_; i++)			// devide tasks for each thread
				{											// this brunch use xor method
					threads_[i] = std::thread([=]() {thread_task_xor(i, random_num); });
				}
				for (size_t i = 0; i < cpu_; i++)			// wait for all threads are complete
				{
					threads_[i].join();
				}
			}
		}
		if (!fin_.good())									// if reach end of file write tail
		{
			fout_.write(reinterpret_cast<char*>(&buffer_[0]), file_size_ - counter * buffer_size_ * sizeof(size_t));
			break;
		}
		fout_.write(reinterpret_cast<char *>(&buffer_[0]), buffer_size_ * sizeof(size_t));
		counter++;
	}
	fin_.close();
	fout_.close();
}

void Encryption::switch_shift(CryptCell32 & source, const size_t random, const bool decrypt)
{
	CryptCell32 tmp = source;
	if (random % 2 > 0)
	{
		if (decrypt)										// if method's called with decrypt mode
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
	CryptCell64 tmp = source;
	if (random % 2 > 0)
	{
		if (decrypt)										// if method's called with decrypt mode
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
	MimicIntCryptCell32 tmp;
	tmp.cell = source;
	tmp.num = tmp.num ^ random;
	source = tmp.cell;
}

void Encryption::xor(CryptCell64 & source, const size_t random)
{
	MimicIntCryptCell64 tmp;
	tmp.cell = source;
	tmp.num = tmp.num ^ random;
	source = tmp.cell;
}

// Thread call methods
void Encryption::thread_task_ss(size_t i, size_t random_num, bool decrypt)
{
	std::for_each(buffer_.begin() + ((buffer_size_ / cpu_) * i), buffer_.end() - ((buffer_size_ / cpu_) * (cpu_ - 1 - i)),
		[=](MimicIntCryptCell64 &n) {switch_shift(n.cell, random_num, decrypt); });
}

void Encryption::thread_task_xor(size_t i, size_t random_num)
{
	std::for_each(buffer_.begin() + ((buffer_size_ / cpu_) * i), buffer_.end() - ((buffer_size_ / cpu_) * (cpu_ - 1 - i)),
		[=](MimicIntCryptCell64 &n) { xor (n.num, random_num); });
}

// Set methods
void Encryption::set_file_path(fs::path & file_path)
{
	file_path_ = file_path;
}

void Encryption::set_file_path(fs::path && file_path)
{
	file_path_ = std::move(file_path);
}

bool Encryption::set_file_size()
{
	if (fin_.is_open())										// if file is open check and set file size
	{
		fin_.seekg(0, fin_.end);
		file_size_ = fin_.tellg();
		fin_.seekg(0, fin_.beg);
		return true;
	}
	else
	{
		return false;
	}
}

void Encryption::set_file_size(size_t file_size)
{
	file_size_ = file_size;
}

bool Encryption::set_buffer_size()
{
	if (file_size_ == 0 && !set_file_size())				// if file size = 0 and can't set file size return false
	{
		return false;
	}
	else
	{
		if (file_size_ <= 128000000) // 128 MB				// if file size < 128 MB set buffer to all file
		{
			buffer_size_ = file_size_ / sizeof(size_t) + 1; 
		}
		else												// else set default buffer size = 128 MB
		{
			buffer_size_ = 128000000 / sizeof(size_t);
		}
		return true;
	}
}

void Encryption::set_buffer_size(size_t buffer_size)
{
	buffer_size_ = buffer_size;
}

bool Encryption::set_buffer()
{
	if (buffer_size_ > 0)									// if buffer size > 0 than set buffer and check file size
	{
		buffer_.resize(buffer_size_);
		set_file_size();
		return true;
	}
	else if (set_buffer_size())								// else set buffer size and file size than set buffer
	{
		buffer_.resize(buffer_size_);
		return true;
	}
	return false;
}

void Encryption::set_loop(size_t loop)
{
	loop_ = loop;
}

void Encryption::set_key()
{
	if (key_.empty())
	{
		srand(time(NULL));
		for (size_t i = 0; i < loop_; i++)
		{
			size_t random_num = 100 + rand() % 900;
			key_ += rand() % 2 == 0 ? 'S' + std::to_string(random_num) : 'X' + std::to_string(random_num);
		}
	}
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

void Encryption::set_cpu(size_t threads_num)
{
	cpu_ = threads_num;
}

void Encryption::set_real_cpu()
{
	cpu_ = std::thread::hardware_concurrency();
}

// Get methods
fs::path Encryption::get_file_path() const
{
	return fs::path(file_path_);
}

size_t Encryption::get_file_size() const
{
	return size_t(file_size_);
}

size_t Encryption::get_buffer_size() const
{
	return size_t(buffer_size_);
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
void Encryption::open_file()								// Call from call_menu()
{
	std::cout << "Current path: " << file_path_ << std::endl;
	std::cout << "Enter full file's path or File name for files in program's directory \n.\\";
	std::cin >> file_path_;
	if (std::cin.eof())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		exit(1);
	}
	if (fin_.is_open())
	{
		fin_.close();
		fout_.close();
	}
	fin_.open(file_path_, std::ios::binary);
	while (!fin_.is_open())
	{
		std::cout << "File's not found" << std::endl << "Current path: " << file_path_ << std::endl;
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
		if (std::cin.eof())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			exit(1);
		}
		fin_.clear();
		fin_.open(file_path_, std::ios::binary);
	}
	if (file_path_.extension() == ".lecrypt")
	{
		fout_.open(file_path_.replace_extension(".ldcrypt"), std::ios::binary);
	}
	else
	{
		fout_.open(file_path_ += ".lecrypt", std::ios::binary);
	}
}

void Encryption::clear_key()
{
	key_.clear();
}

void Encryption::change_key()							   // Call from call_menu()
{
	size_t choice;
	std::cout << "Current key: " << get_key() << std::endl;
	std::cout << "Menu: \t1. Set \t\t2. Clear \t3. Back" << std::endl;
	std::cout << "> ";
	std::cin >> choice;
	switch (choice)
	{
	case 1: // Set
	{
		std::cout << "> ";
		std::cin >> key_;
		std::cout << "Current key: " << get_key() << std::endl;
		break;
	}
	case 2: // Clear
	{
		clear_key();
		std::cout << "Current key: " << get_key() << std::endl;
		break;
	}
	case 3: // Back
	{
		return void();
	}
	default:
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		break;
	}
}

void Encryption::change_loop()								// Call from call_menu()
{
	size_t choice;
	std::cout << "Current number of encryption loops: " << get_loop() << std::endl;
	std::cout << "Menu: \t1. Set \t\t2. Default \t3. Back" << std::endl;
	std::cout << "> ";
	std::cin >> choice;
	switch (choice)
	{
	case 1: // Set
	{
		std::cout << "> ";
		std::cin >> choice;
		set_loop(choice);
		std::cout << "Current number of encryption loops: " << get_loop() << std::endl;
		break;
	}
	case 2: // Default
	{
		set_loop();
		std::cout << "Current number of encryption loops: " << get_loop() << std::endl;
		break;
	}
	case 3: // Back
	{
		return void();
	}
	default:
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		break;
	}
}

void Encryption::change_buffer()							// Call from call_menu()
{
	size_t choice;
	std::cout << "Current buffer size: " << (double)get_buffer_size() * sizeof(size_t) / 1000000 << " MB" << std::endl;
	std::cout << "Menu: \t1. Set \t\t2. Default \t3. Back" << std::endl;
	std::cout << "> ";
	std::cin >> choice;
	switch (choice)
	{
	case 1: // Set
	{
		std::cout << "> ";
		std::cin >> choice;
		set_buffer_size(choice * 1000000 / sizeof(size_t));
		std::cout << "Current buffer size: " << (double)get_buffer_size() * sizeof(size_t) / 1000000 << " MB" << std::endl;
		break;
	}
	case 2: // Default
	{
		set_buffer_size(0);
		set_buffer_size();
		std::cout << "Current buffer size: " << (double)get_buffer_size() * sizeof(size_t) / 1000000 << " MB" << std::endl;
		break;
	}
	case 3: // Back
	{
		return void();
	}
	default:
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		break;
	}
}

void Encryption::change_settings()							// Call from call_menu()
{
	size_t choice;
	while (true)
	{
		std::cout << "Menu: \t1. File path \t2. Key \t\t3. Encryption \t4. Buffer size \t5. Back" << std::endl;
		std::cout << "> ";
		std::cin >> choice;
		switch (choice)
		{
		case 1: // File path
		{
			open_file();
			break;
		}
		case 2: // Key
		{
			change_key();
			break;
		}
		case 3: // Encryption
		{
			change_loop();
			break;
		}
		case 4: // Buffer size
		{
			change_buffer();
			break;
		}
		case 5: // Back
		{
			return void();
		}
		default:
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
	}
}

void Encryption::call_menu()
{
	size_t choice;
	while (true)
	{
		std::cout << "Menu: \t1. Encrypt \t2. Decrypt \t3. Settings \t4. Exit" << std::endl;
		std::cout << "> ";
		std::cin >> choice;
		switch (choice)
		{
		case 1: // Encrypt
		{
			std::cout << "Key: " << encrypt() << std::endl;
			break;
		}
		case 2: // Decrypt
		{
			decrypt();
			break;
		}
		case 3: // Settings
		{
			change_settings();
			break;
		}
		case 4:
		{
			exit(1);
		}
		default:
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
	}
}

bool Encryption::get_ready(bool decrypt)
{
	if (!fin_.is_open())									// Check if file is open, if not than try to open
	{
		fin_.open(file_path_, std::ios::binary);
	}
	if (!set_buffer())										// Check if buffer is set, if not than try to set default
	{
		fin_.clear();
		return false;
	}
	cpu_ = buffer_size_ < cpu_ ? buffer_size_ : cpu_;		// Check possibility of threading
	if (!decrypt)											// Check if method's called in decrypt mode
	{
		if (!fout_.is_open())								// Check if file is open, if not than try to open
		{
			fout_.open(file_path_ += ".lecrypt", std::ios::binary);
		}
		set_key();											// Check if there is key_, if not than create random
	}
	else 
	{
		if (key_.empty())									// Check if there is key_, if not than return not ready status
		{
			return false;
		}
		if (file_path_.extension() == ".lecrypt")			// Check if file extention == encrypt() ext. than replace to decrypt() ext.,
		{													// if not than add decrypt() ext.
			if (!fout_.is_open())
			{
				fout_.open(file_path_.replace_extension(".ldcrypt"), std::ios::binary);
			}
		}
		else
		{
			if (!fout_.is_open())
			{
				fout_.open(file_path_ += ".ldcrypt", std::ios::binary);
			}
		}
	}
	return true;
}
}; // namespace laiter