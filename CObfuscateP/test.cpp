#include <iostream>
#include <cstdio>
#include "CObfuscateP.h"
#include "CManageReg.h"
#include "CCalc.h"

void test()
{
	auto str1 = COSTRA("hello world!");
	puts(str1);

	char* str2 = COSTRA("test string!");
	printf("%s\r\n", str2);

	wchar_t* str3 = COSTRW(L"test wstring!");
	printf("%ws\r\n", str3);

}

int main()
{
	test();
	system("pause");
}

