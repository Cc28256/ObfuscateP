# 项目作用：

编译时混淆字符串，确保编译后的二进制PE不是明文保存的字符串。
 ![image](https://github.com/Cc28256/ObfuscateP/blob/main/CObfuscateP/look.png)

# 使用方法：

1 项目需要 CApiCore.h和CObfuscateP.h 两个文件

2 包含头文件 #include "CObfuscateP.h"

3 支持宽字符与窄字符 COSTRA加密const char* 类型 COSTRW加密const wchar_t* 类型 

示例代码：

	auto str1 = COSTRA("hello world!");
	puts(str1);

	char* str2 = COSTRA("test string!");
	printf("%s\r\n", str2);

	wchar_t* str3 = COSTRW(L"test wstring!");
	printf("%ws\r\n", str3);
  
  
  # 注意事项：
  
  1 需要 C++ 14 或以上
  
  2 编译报错 C2975 : LINE __用作 /ZI__ 编译时常量时，会发生 C2975。 一种解决方案是使用 /Zi 而不是 /ZI 进行编译。

  3 随机生成(伪随机)Key加密，可通过定义CC_OBFUSCATE_DEFAULT_KEY来自定义指定key
  
  4 COSTR宏定义Lambda表达式 表达式声明了CObfuscatedDataA对象，对象只在宏所在函数作用域有效，离开函数后，对象将被释放。
  
  5 DEBUG 模式 或项目定义宏COSTR_OFF将关闭混淆，明文字符串将暴露在编译后的二进制文件中。可根据需求自行修改源码。
  
  
  # 项目参考
  
  项目基于开源项目二次更改并添加了const wchar_t* 类型的字符串混淆。
  
  https://github.com/adamyaxley/Obfuscate
