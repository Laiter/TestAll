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
#ifndef LAITER_ENCRYPTION_H
#define LAITER_ENCRYPTION_H
#include <cstdint>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <limits>
namespace fs = std::experimental::filesystem;
namespace laiter {
// Encrypt and Decrypt file
// Example:
//	//Ex1: use console UI
//	laiter::Encryption L;
//	L.open_file();
//	L.call_menu();
//	//Ex2: Parametric constructor, custom settings
//	laiter::Encryption L1("C:\\Project10\\file_path.txt", "", 2, 128000000, 8);
//	L2.set_key("X932S324");
//	std::string key = L1.encrypt();
//	L1.decrypt();
//	//Ex3: Default constructor, default settings
//	laiter::Encryption L2;
//	L2.set_file_path("file_path.lecrypt");
//	std::string key = L2.encrypt();
//	L2.decrypt();
class Encryption
{
public:
	// De\Constructors
	Encryption();
	Encryption(const fs::path file_path, const std::string key = "", 
		size_t loop = 8, size_t buffer_size = 0, size_t threads_num = 0);
	Encryption(const Encryption& copy) = delete;
	~Encryption();
	//Operators
	Encryption& operator=(const Encryption& copy) = delete;
	// Bitfields for int changing in union
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
	// Methods
	// Encryption methods
	std::string encrypt();
	void decrypt();
	void switch_shift(CryptCell32& source, const size_t random, const bool decrypt = 0);
	void switch_shift(CryptCell64& source, const size_t random, const bool decrypt = 0);
	void xor(uint32_t& source, const size_t random);
	void xor(uint64_t& source, const size_t random);
	void xor(CryptCell32& source, const size_t random);
	void xor(CryptCell64& source, const size_t random);
	// Thread call methods (divide buffer for each thread)
	void thread_task_ss(size_t i, size_t random_num, bool decrypt = false);
	void thread_task_xor(size_t i, size_t random_num);
	// Set methods
	void set_file_path(fs::path& file_path);
	void set_file_path(fs::path&& file_path);
	bool set_file_size();							// Set file size of ifstream fin_, if failure return false
	void set_file_size(size_t file_size);
	bool set_buffer_size();							// call set_file_size() if not set
	void set_buffer_size(size_t buffer_size);
	bool set_buffer();								// call set_buffer_size() and set_file_size() if not set
	void set_loop(size_t loop = 8);
	void set_key();									// create random key_ if key_ is empty
	void set_key(std::string& key);
	void set_key(std::string&& key);
	void set_threads();								// call set_real_cpu() if cpu_ = 0
	void set_threads(size_t threads_num);			// call set_real_cpu() if cpu_ = 0
	void set_real_threads();						// call set_real_cpu() before set threads
	void set_cpu();
	void set_cpu(size_t threads_num);
	void set_real_cpu();							// the same as set_cpu()
	// Get methods
	fs::path get_file_path() const;
	size_t get_file_size() const;
	size_t get_buffer_size() const;
	size_t get_loop() const;
	std::string get_key() const;
	size_t get_threads() const;
	//void get_real_threads();
	size_t get_cpu() const;
	size_t get_real_cpu() const;					// call std::thread::hardware_concurrency()
	// Utility
	void open_file();
	void clear_key();
	void change_key();
	void change_loop();
	void change_buffer();
	void change_settings();
	void call_menu();								// console UI
	bool get_ready(bool decrypt = false);
private:
	// Streams
	std::ifstream fin_;
	std::ofstream fout_;
	fs::path file_path_;
	size_t file_size_;
	// File buffer
	std::vector<MimicIntCryptCell64> buffer_;
	size_t buffer_size_;							// 1 buffer_size = sizeof(size_t) byte. x64 - 8 byte, x86 - 4 byte
	// Number of encrypt operations for each cell
	// use to generate key
	size_t loop_;
	// Decryption key
	std::string key_;
	// Number of threads
	size_t cpu_;
	// Threads container
	std::vector<std::thread> threads_;
};
}; // namespace laiter

#endif // !LAITER_ENCRYPTION_H
