// BeepHook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <easyhook.h>
#include <string>
#include <iostream>
#include <Windows.h>

DWORD gFreqOffset = 0;
BOOL WINAPI myBeepHook(DWORD dwFreq, DWORD dwDuration)
{
	std::cout << "\n    BeepHook: ****All your beeps belong to us!\n\n";
	return Beep(dwFreq + gFreqOffset, dwDuration);
}

// EasyHook will be looking for this export to support DLL injection. If not found then 
// DLL injection will fail.
extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
	std::cout << "\n\nNativeInjectionEntryPointt(REMOTE_ENTRY_INFO* inRemoteInfo)\n\n" <<
		"IIIII           jjj               tt                dd !!! \n"
		" III  nn nnn          eee    cccc tt      eee       dd !!! \n"
		" III  nnn  nn   jjj ee   e cc     tttt  ee   e  dddddd !!! \n"
		" III  nn   nn   jjj eeeee  cc     tt    eeeee  dd   dd     \n"
		"IIIII nn   nn   jjj  eeeee  ccccc  tttt  eeeee  dddddd !!! \n"
		"              jjjj                                         \n\n";

	std::cout << "NativeInjectionEntryPoint: Injected by process Id: " << inRemoteInfo->HostPID << "\n";
	std::cout << "NativeInjectionEntryPoint: Passed in data size: " << inRemoteInfo->UserDataSize << "\n";
	if (inRemoteInfo->UserDataSize == sizeof(DWORD))
	{
		gFreqOffset = *reinterpret_cast<DWORD *>(inRemoteInfo->UserData);
		std::cout << "NativeInjectionEntryPoint: Adjusting Beep frequency by: " << gFreqOffset << "\n";
	}

	// Perform hooking
	HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook

	std::cout << "\n";
	std::cout << "NativeInjectionEntryPoint: Win32 Beep found at address: " << GetProcAddress(GetModuleHandle(TEXT("kernel32")), "Beep") << "\n";

	// Install the hook
	NTSTATUS result = LhInstallHook(
		GetProcAddress(GetModuleHandle(TEXT("kernel32")), "Beep"),
		myBeepHook,
		NULL,
		&hHook);
	if (FAILED(result))
	{
		std::wstring s(RtlGetLastErrorString());
		std::wcout << "NativeInjectionEntryPoint: Failed to install hook: " << s << "\n";
	}
	else 
	{
		std::cout << "NativeInjectionEntryPoint: Hook 'myBeepHook installed successfully.\n";
	}

	// If the threadId in the ACL is set to 0,
	// then internally EasyHook uses GetCurrentThreadId()
	ULONG ACLEntries[1] = { 0 };

	// Disable the hook for the provided threadIds, enable for all others
	LhSetExclusiveACL(ACLEntries, 1, &hHook);

	return;
}