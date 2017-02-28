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
// TODO: port code to Android
#include "encryption.h"

void main()
{
	//Enter test.txt in console to use test file
	laiter::Encryption L;
	L.open_file();
	L.call_menu();
}
