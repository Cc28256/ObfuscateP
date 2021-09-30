#pragma once
#include"CApiCore.h"

_CCAPI_BEGIN

enum class EnumAvType{
	
	// 每次都要遍历进程才能确定的
	_360WD,			 // 360网盾


	// 只需要遍历一次就可以确定的
	_360,			 // 360
	_QQGuanJia,		 // QQ电脑管家
	_HuoRong,		 // 火绒
	_Norton,		 // 诺顿		和赛门铁克一家
	_Symantec,		 // 赛门铁克	和诺顿一家
	_WinDef,		 // windows def
	_Kaspersky,		 // 卡巴斯基
	_Avast,			 // Avast
	_McAfee,		 // 麦咖啡
	_QuShi,			 // 趋势
	_JinShan,		 // 金山毒霸
	_ESET,			 // eset
	_Avira,			 // 小红伞
	_DrWeb,			 // 大蜘蛛

	//... 在此添加更多

	AvTypeMaxSize
};

typedef struct _AV_INFO{
	unsigned int procBKDRHash;
	EnumAvType AvType;
}AV_INFO,*PAV_INFO;

class AvList
{
public:
	AvList();
	bool Init();
	bool ReviewInit();
	const bool& operator[] (EnumAvType i);

private:
	bool m_IsAvExist[(int)EnumAvType::AvTypeMaxSize]{0};
	bool CheckAllAvProc();
	bool CheckOneAvProc(unsigned int hash);
	bool m_isChecked;
};
_CCAPI_END

#define EAT _CCAPI EnumAvType

