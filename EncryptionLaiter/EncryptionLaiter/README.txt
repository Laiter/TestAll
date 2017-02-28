//branch: Encryption
//author: @Vladislav Sarychev
//version: 1.0 28.02.17
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
// for the future possible releases:
// TODO: develop better key algorithms
// TODO: develop GUI
// TODO: port code to Android
/////////////////////////////////////////////////////////////////////////////////////////////

This branch contain class Encryption that can enrypt\decrypt any file with encryption key.

To use it you can simply use code in main.cpp that call console UI. 
Or it can be used manually with methods described in encryption.h
Repository contain test.txt for test class Encryption. Use Notepad++ plugin Compare to check this is work.
encrypt() method create file extension ".lecrypt", decrypt() method create ".ldcrypt" 

Also tests and old function can be found in this repository (test_1, 2, 3.cpp, laiter_encryption_test.h)
Old functions and tests may need changes to work.
Results of tests can be found in results.txt

You can use this code. For details see LICENCE.txt