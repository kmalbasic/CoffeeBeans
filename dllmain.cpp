#include "pch.h"
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "user32") // NOTE: this DLL will only work on apps that are using user32 API.

BOOL CheckIfValidProc() {
	char szModuleName[MAX_PATH]; // buffer to store the full path
	LPTSTR szProcess;

	if (!GetModuleFileName(NULL, szModuleName, MAX_PATH)) // if bad name, no can do 
		return FALSE;

	szProcess = strrchr(szModuleName, '\\'); // strip the path to get executable name, ghetto
	if (!szProcess)
		return FALSE;
	szProcess++;


	if (!_stricmp(szProcess, "notepad.exe")) // set to whatever you want, compare the names, used _stricmp because it's not a part of standard C/C++ 
		return TRUE;					     // x64/x86, be sure to compile the DLL for the correct architecture
	else
		return FALSE;
}

void SendKeystroke() {
	keybd_event(0x7F, 0, 0, 0); // press key down (preferable to use SendMessage/PostMessage here)
	Sleep(1000);
	keybd_event(0x7F, 0, KEYEVENTF_KEYUP, 0); // release the key
	Sleep(1000); // adjust delay for keystrokes, 45 seconds optimal subjectively
}

void CoffeeBeans() {

	// debug feature, remove if needed
	for (int i = 0; i < 5; i++) {
		Beep(500, 200);
		Sleep(1000);
	}

	// yum, infinite loop
	while (true) {
		SendKeystroke();
	}
}

DWORD APIENTRY hkCoffeeBeans(LPVOID) { // meme "hook" (don't forget to cleanup after you hook other functions when you unload your DLLs, VMT got my ass shaking)
	CoffeeBeans();
	return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		if (CheckIfValidProc()) { // check if our process is valid, run our big codenz within the DLL (a big no-no usually but who cares)
			MessageBox(NULL, "No more sleep for you. I like notepads. lol. spooderman.", "CoffeeBeans", MB_OK); // memes
			CreateThread(NULL, 0, hkCoffeeBeans, NULL, 0, NULL);
			return TRUE;
		}
		else 
			return FALSE; // if not our process, detach, unload, no wasted resources
    case DLL_PROCESS_DETACH:
		// here you would do the big cleanup and release all your hooks (reference to the "hook" meme above)
        break;
    }
    return TRUE;
}
