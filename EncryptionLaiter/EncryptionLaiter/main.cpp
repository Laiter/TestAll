//******************************************************************************************
// v1.0: Encrypter/Decrypter
// 1. write a class Encryption that should read\write and encrypt\decrypt any file. 
// 2. have to be optimized for x64
// 3. have to support multithreading and single thread
// 4. main methods should support auto and manual mode
// 5. have to work with huge files
// 6. have to support differend buffer size of file 
// 7. write console program that use class Encryption
// 8. console UI should be user frieldly
// for the future releases:
// TODO: develop better key algorithms
// TODO: develop GUI
#include "encryption.h"

void main()
{
	//Enter test.txt in console to use test file
	laiter::Encryption L;
	L.open_file();
	L.call_menu();
}
