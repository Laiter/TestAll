#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>

void XOR16(uint16_t& source, uint16_t random)
{
	source = source ^ random;
}
void XOR32(uint32_t& source, uint32_t random)
{
	source = source ^ random;
}
void XOR64(uint64_t& source, uint64_t random)
{
	source = source ^ random;
}

uint16_t* A16 = new uint16_t[4000000000];
//std::vector<uint16_t> A16(4000000000);
std::vector<uint32_t> B32(2000000000);
std::vector<uint64_t> C64(1000000000);
void test2()
{
	uint16_t rand16 = 43690;
	uint32_t rand32 = 2863311530UL;
	uint64_t rand64 = 12297829382473034412ULL;

	uint64_t i = 123;
	std::generate_n(A16, 100000000, [&i]() {return i * i++; });
	i = 123;
	std::generate_n(B32.begin(), 100000000, [&i]() {return i * i++; });
	i = 123;
	std::generate_n(C64.begin(), 100000000, [&i]() {return i * i++; });
	system("pause");
	std::cout << "Start!16\n";
	auto start_time = std::chrono::steady_clock::now();													// chrono
	std::for_each(A16, A16 + 4000000000, [rand16](uint16_t& a) {XOR16(a, rand16); });
	auto end_time = std::chrono::steady_clock::now();													// chrono
	auto elapsed_ns = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);		    // chrono
	std::cout << "overall time: " << elapsed_ns.count() << " ns 16\n";								// chrono
	system("pause");

	std::cout << "Start!32\n";
	start_time = std::chrono::steady_clock::now();													// chrono
	std::for_each(B32.begin(), B32.end(), [rand32](uint32_t& a) {XOR32(a, rand32); });
	end_time = std::chrono::steady_clock::now();													// chrono
	elapsed_ns = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);		    // chrono
	std::cout << "overall time: " << elapsed_ns.count() << " ns 32\n";								// chrono
	system("pause");

	std::cout << "Start!64\n";
	start_time = std::chrono::steady_clock::now();													// chrono
	std::for_each(C64.begin(), C64.end(), [rand64](uint64_t& a) {XOR64(a, rand64); });
	end_time = std::chrono::steady_clock::now();													// chrono
	elapsed_ns = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);		    // chrono
	std::cout << "overall time: " << elapsed_ns.count() << " ns 64\n";								// chrono
	system("pause");
}