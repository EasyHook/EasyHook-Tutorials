// Target.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	std::string value;
	while (true)
	{
		HANDLE currentThread = GetCurrentThread();
		std::cout << "Target process id: ";
		std::cout << GetProcessIdOfThread(currentThread);
		std::cout << "\n";
		CloseHandle(currentThread);

		std::cout << "Target: press <enter> to Beep (Ctrl-C to exit): ";
		std::getline(std::cin, value);
		std::cout << "Target: calling Beep...\n";
		Beep(500, 500);
		std::cout << "Target: Beep complete.\n";
	}
	return 0;
}

