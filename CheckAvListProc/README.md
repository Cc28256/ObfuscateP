# 项目作用：

检查工作于当前计算机的杀软或进程 且不会将进程字符串明文暴露。

目前仅支持检查：

	360
	QQ电脑管家
	火绒
	诺顿
	赛门铁克
	windows def
	卡巴斯基
	Avast
	麦咖啡
	趋势
	金山毒霸
	eset
	小红伞
	大蜘蛛
  



# 使用方法：

1 项目需要 CApiCore.h、CAvList.h和CAvList.cpp文件

2 包含头文件 #include "CAvList.h"

3 根据地定义可在代码中添加想要检查的杀软或进程

示例代码：

```c

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
  
```
  
  # 注意事项：
  
  1 constexpr 需要 C++ 14 或以上。
  
  2 直接使用 [EAT::XXX] 可以获取对应杀软或进程存在，但使用前尽可能使用Init()来判断流程都正确。

  3 进程字符串被计算为 unsigned int 类型在编译时期存于二进制文件，因此不会暴露明文。

  4 可通过在 EnumAvType枚举类型、 CheckAllAvProc函数lAvInfoList变量中添加杀软进程名称。
  
  5 重载运算符[]中switch块中添加自定义需要单一的进程检查。
  
  5 在检查杀软时，只会遍历一次所有进程来确定是否存在，如果想每次调用时都遍历所有进程，那么在调用前请使用ReviewInit()进程进行更新类数据，
  
  6 检查自定义单一进程每次会自动遍历。
  

