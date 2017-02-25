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

namespace fs = std::experimental::filesystem;

namespace laiter {
class Encryption
{
public:
	Encryption();
	Encryption(const fs::path file_path, const std::string key = "", size_t loop = 2, size_t threads_num = 0);
	~Encryption();

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
	std::string Encrypt();
	void Decrypt();
	void switch_shift(CryptCell32& source, const size_t random, const bool decrypt = 0);
	void switch_shift(CryptCell64& source, const size_t random, const bool decrypt = 0);
	void xor(uint32_t& source, const size_t random);
	void xor(uint64_t& source, const size_t random);
	void xor(CryptCell32& source, const size_t random);
	void xor(CryptCell64& source, const size_t random);
	void set_file_path(fs::path& file_path);
	void set_file_path(fs::path&& file_path);
	void set_file_size(size_t file_size);
	void set_buffer();
	void set_loop(size_t loop);
	void set_key(std::string& key);
	void set_key(std::string&& key);
	void set_threads();
	void set_threads(size_t threads_num);
	void set_real_threads();
	void set_cpu();
	void set_cpu(size_t cpu_num);
	void set_real_cpu(); // the same as set_cpu()
	// Utility
	void open_file();
private:
	// Streams
	std::ifstream fin_;
	std::ofstream fout_;
	fs::path file_path_;
	size_t file_size_;
	// File buffer
	std::vector<MimicIntCryptCell64> buffer_;
	// Number of encrypt operations for each cell
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
