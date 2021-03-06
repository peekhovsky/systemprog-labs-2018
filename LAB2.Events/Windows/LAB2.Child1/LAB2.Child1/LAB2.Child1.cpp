#define _CRT_SECURE_NO_WARNINGS
// LAB1Processes.cpp : Defines the entry point for the console application.


/*
Вариант 12
Реализовать взаимодействие студента и преподавателя. Студент может сдавать
определенное количество лабораторных по какому - либо предмету (минимум два).
Преподаватель может как принять лабораторные, так и проинформировать студента
об отсутствии времени на прием лабораторных определенного предмета либо об
отсутствии этого предмета в нагрузке. Состояние занятости преподавателя должно
сохраняться.
*/

#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <climits>
#include <wchar.h>

using namespace std;


#define CHILD_PROCESS_LOCATION "C:\\Users\\rosti\\Documents\\GitHub\\systemprog-labs-2018\\LAB2.Events\\Windows\\LAB2.Child2\\Debug\\LAB2.Child2.exe"
#define CHAR_SIZE 100


int getInt(int min, int max)
{
	int res = 0;
	do
	{
		cin >> res;
		if ((res < min) || (res > max))
		{
			cout << "\nIncorrect input. Try again\n";
			rewind(stdin);
			cin.clear();
			continue;
		}
		else break;
	} while (true);
	return res;
}

HANDLE start_event;
HANDLE print_event;
HANDLE end_event;

BOOL WINAPI ConsoleHandler(DWORD CEvent) {

	switch (CEvent) {
	case CTRL_CLOSE_EVENT:
		CloseHandle(start_event);
		CloseHandle(print_event);
		CloseHandle(end_event);
	}
	return TRUE;
}

int main(int argc, char* argv[]) {

	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE) {
		printf("Unable to install handler!\n");
		return -1;
	}

	char* print_event_name = new char[11];
	char* end_event_name = new char[11];

	strcpy(print_event_name, argv[0]);
	strcpy(end_event_name, argv[0]);

	print_event_name[0] = '0';
	end_event_name[0] = '1';


	start_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT(argv[0]));
	print_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT(print_event_name));
	end_event	= OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT(end_event_name));

	do {
		system("cls");
		int index = WaitForSingleObject(start_event, 0);
		Sleep(200);

		if (index == WAIT_OBJECT_0) {
			break;
		} else if ((WaitForSingleObject(print_event, 0) == WAIT_OBJECT_0)) {
			ResetEvent(print_event);
			for (int i = 0; argv[0][i] != '\0', i < 9; i++) {
				cout << argv[0][i];
				Sleep(150);
			}
			SetEvent(end_event);
			
		}
	} while (true);

	cout << "How many labs do you want to pass?" << endl;
	int lab_count = getInt(1, INT_MAX);

	char* subjects = new char[CHAR_SIZE];
	subjects[0] = '\0';


	cout << "Enter names of subject that you want to pass: " << endl;

	for (int i = 0; i < lab_count; i++)
	{
		cout << "Subject " << i << ": " << endl;
		rewind(stdin);
		char* temp = new char[CHAR_SIZE];
		fgets(temp, CHAR_SIZE, stdin);


		for (int i = 0; temp[i] != '\0'; i++)
		{
			if (temp[i] == '\n') temp[i] = ' ';
		}

		strcat_s(subjects, CHAR_SIZE, temp);
	}

	//wcout << subjects;


	//settings
	STARTUPINFO start_up_info;
	PROCESS_INFORMATION process_information;

	ZeroMemory(&start_up_info, sizeof(start_up_info));
	start_up_info.cb = sizeof(start_up_info);
	ZeroMemory(&process_information, sizeof(process_information));


	/*//create process
	if (!CreateProcess(TEXT("..."),		//file location
	NULL,							// Command line
	NULL,							// Process handle not inheritable
	NULL,							// Thread handle not inheritable
	FALSE,							// Set handle inheritance to FALSE
	CREATE_NEW_CONSOLE,				//
	NULL,							// Use parent's environment block
	NULL,							// Use parent's starting directory
	&start_up_info,					// Pointer to STARTUPINFO structure
	&process_infornation)			// Pointer to PROCESS_INFORMATION structure
	)
	{
	printf("CreateProcess failed (%d)\n", GetLastError());
	_getch();
	return -1;
	}
	*/
	//create process
	if (!CreateProcess(TEXT(CHILD_PROCESS_LOCATION), subjects, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &start_up_info, &process_information))
	{
		cout<< "CreateProcess failed: " << GetLastError() << endl;
		_getch();
		return -1;
	}

	SYSTEMTIME lt;

	cout << endl;
	while (WaitForSingleObject(process_information.hProcess, 50))
	{
		GetLocalTime(&lt);
		printf("%02d:%02d:%02d p\r", lt.wHour, lt.wMinute, lt.wSecond);
	}
	cout << endl << endl;

	DWORD exit_code = 0;
	DWORD result = GetExitCodeProcess(process_information.hProcess, &exit_code);

	cout << "Professor can get " << exit_code << " labs." << endl;
	_getch();
		
	CloseHandle(process_information.hProcess);
	CloseHandle(start_event);
	CloseHandle(print_event);
	CloseHandle(end_event);

	return 0;

}
