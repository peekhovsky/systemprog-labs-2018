// program.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <thread>

typedef void(*DLL_LIB_FUNC)();
HINSTANCE dll_library;

void read();
void write();


int main(int args, char* argv[]) {
	
	//loading library
	dll_library = LoadLibrary("library1.dll");

	if (dll_library == NULL) {
		std::cout << "Cannot load dll library!" << std::endl;
		_getch();
		exit(0);
	}
	else {
		std::cout << "Dll library has been loaded." << std::endl;
	}

	std::cout << "Performing reading and writing..." << std::endl;
	

	std::thread reader(read);
	std::thread writer(write);

	reader.join();
	writer.join();
	
	std::cout << "Reader and writer finished their tasks." << std::endl;

	FreeLibrary(dll_library);
	_getch();
	return 0;
}

void read() {
	DLL_LIB_FUNC dll_read = (DLL_LIB_FUNC)GetProcAddress(dll_library, "read");
	dll_read();
	ExitThread(0);
}

void write() {
	DLL_LIB_FUNC dll_write = (DLL_LIB_FUNC)GetProcAddress(dll_library, "write");
	dll_write();
	ExitThread(0);
}