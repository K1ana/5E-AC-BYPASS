#include"MinHook/min_hook.h"

#include <iostream>
#include<Windows.h>
#include"hook___.h"

bool blockcall;//阻止调用的标识


char* WstrToAstr(WCHAR* wstr)
{
	unsigned long i = lstrlen(wstr);
	char* astr;
	astr = (char*)malloc(i + 1);
	if (astr == NULL)
		return NULL;
	else
		memset(astr, '\0', i + 1);

	wcstombs(astr, wstr, i + 1);
	return astr;
}

//原函数
using fn1 = ATOM(__stdcall*)(const WNDCLASSW*);
fn1 o_reg = nullptr;

//钩子函数
ATOM __stdcall HKRegisterClassW(_In_ const WNDCLASSW* lpWndClass) {

	std::cout << WstrToAstr((WCHAR*)lpWndClass->lpszClassName) << "已注册\n";
	if (/*WstrToAstr((WCHAR*)lpWndClass->lpszClassName) == "Valve001"*/strstr(WstrToAstr((WCHAR*)lpWndClass->lpszClassName), "Valve0")) {
		std::cout << "Valve001 reg\n";
		blockcall = true;
		return o_reg(lpWndClass);
	}

	if (blockcall) {
		return NULL;
	}
	return o_reg(lpWndClass);
};


void hooks::hook() {
	auto RegisterClassWhk = GetProcAddress(GetModuleHandleW(L"user32.dll"), "RegisterClassW");
	MH_Initialize();
	MH_CreateHook((LPVOID)RegisterClassWhk, &HKRegisterClassW, reinterpret_cast<void**>(&o_reg));
	MH_EnableHook(MH_ALL_HOOKS);
}