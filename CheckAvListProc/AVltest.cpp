// AVlist.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CAvList.h"

int main()
{
	_CCAPI AvList avl;
	
	if (!avl.Init()){
		printf("AvList error!\r\n");
		return 0;
	}

	auto pFn = [](bool b, char * c) {b ? printf(" %s exist!\r\n", c) : printf(" %s not find!\r\n", c); };

	pFn(avl[EAT::_Kaspersky], "kaky");
	pFn(avl[EAT::_HuoRong], "huro");
	pFn(avl[EAT::_WinDef], "wind");
	pFn(avl[EAT::_360WD], "30WD");

	system("pause");
}

