// Injector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <cstring>

#include <easyhook.h>

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD processId;
	std::wcout << "Enter the target process Id: ";
	std::cin >> processId;

	DWORD freqOffset = 0;
	std::cout << "Enter a frequency offset in hertz (e.g. 800): ";
	std::cin >> freqOffset;

	WCHAR* dllToInject = L"..\\Debug\\BeepHook.dll";
	wprintf(L"Attempting to inject: %s\n\n", dllToInject);
	
	// Inject dllToInject into the target process Id, passing 
	// freqOffset as the pass through data.
	NTSTATUS nt = RhInjectLibrary(
		processId,   // The process to inject into
		0,           // ThreadId to wake up upon injection
		EASYHOOK_INJECT_DEFAULT,
		dllToInject, // 32-bit
		NULL,		 // 64-bit not provided
		&freqOffset, // data to send to injected DLL entry point
		sizeof(DWORD)// size of data to send
	);

	if (nt != 0)
	{
		printf("RhInjectLibrary failed with error code = %d\n", nt);
		PWCHAR err = RtlGetLastErrorString();
		std::wcout << err << "\n";
	}
	else 
	{
		std::wcout << L"Library injected successfully.\n";
	}

	std::wcout << "Press Enter to exit";
	std::wstring input;
	std::getline(std::wcin, input);
	std::getline(std::wcin, input);
	return 0;
}