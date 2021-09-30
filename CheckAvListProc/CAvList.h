#pragma once
#include"CApiCore.h"

_CCAPI_BEGIN

enum class EnumAvType{
	
	// ÿ�ζ�Ҫ�������̲���ȷ����
	_360WD,			 // 360����


	// ֻ��Ҫ����һ�ξͿ���ȷ����
	_360,			 // 360
	_QQGuanJia,		 // QQ���Թܼ�
	_HuoRong,		 // ����
	_Norton,		 // ŵ��		����������һ��
	_Symantec,		 // ��������	��ŵ��һ��
	_WinDef,		 // windows def
	_Kaspersky,		 // ����˹��
	_Avast,			 // Avast
	_McAfee,		 // �󿧷�
	_QuShi,			 // ����
	_JinShan,		 // ��ɽ����
	_ESET,			 // eset
	_Avira,			 // С��ɡ
	_DrWeb,			 // ��֩��

	//... �ڴ���Ӹ���

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

