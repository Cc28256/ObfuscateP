#pragma once
#ifndef _CC_OBFUSCATE_PLUS_H_ 
#define _CC_OBFUSCATE_PLUS_H_ 
#include "CApiCore.h"

// 需要 C++ 14 或以上
// 目前可能用于wchar_t* char*类型的字符串加密
// 将 LINE __用作 /ZI__ 编译时常量时，会发生 C2975。 一种解决方案是使用 /Zi 而不是 /ZI 进行编译。
// 用于混淆字符串的默认64位key。
// 可以通过定义CC_OBFUSCATE_DEFAULT_KEY来自定义指定key

#define CC_OBFUSCATE_DEFAULT_KEY _CCAPI RandGenerateKey(__LINE__)

_CCAPI_BEGIN	
	using SIZET = unsigned long long;
	using KEYT = unsigned long long;


	constexpr KEYT RandGenerateKey(KEYT seed)
	{

		KEYT key = seed;
		key ^= (key >> 33);
		key *= 0xff51afd7ed558ccd;
		key ^= (key >> 33);
		key *= 0xc4ceb9fe1a85ec53;
		key ^= (key >> 33);

		key |= 0x0101010101010101ull;

		return key;
	}

	
	constexpr void CipherA(char* data, SIZET size, KEYT key)
	{

		for (SIZET i = 0; i < size; i++)
		{
			data[i] ^= char(key >> ((i % 8) * 8));
		}
	}

	constexpr void CipherW(wchar_t* data, SIZET size, KEYT key)
	{

		for (SIZET i = 0; i < size; i++)
		{
			data[i] ^= wchar_t(key >> ((i % 8) * 8));
		}
	}


	template <SIZET N, KEYT KEY>
	class CObfuscatorA
	{
	public:

		constexpr CObfuscatorA(const char* data)
		{

			for (SIZET i = 0; i < N; i++)
			{
				m_Data[i] = data[i];
			}

			CipherA(m_Data, N, KEY);
		}

		constexpr const char* data() const
		{
			return &m_Data[0];
		}

		constexpr SIZET size() const
		{
			return N;
		}

		constexpr KEYT key() const
		{
			return KEY;
		}

	private:

		char m_Data[N]{};
	};

	template <SIZET N, KEYT KEY>
	class CObfuscatorW
	{
	public:

		constexpr CObfuscatorW(const wchar_t* data)
		{
			for (SIZET i = 0; i < N; i++)
			{
				m_Data[i] = data[i];
			}

			CipherW(m_Data, N, KEY);
		}

		constexpr const wchar_t* data() const
		{
			return &m_Data[0];
		}

		constexpr SIZET size() const
		{
			return N;
		}

		constexpr KEYT key() const
		{
			return KEY;
		}

	private:

		wchar_t m_Data[N]{};
	};

	template <SIZET N, KEYT KEY>
	class CObfuscatedDataA
	{
	public:
		CObfuscatedDataA(const CObfuscatorA<N, KEY>& obA)
		{
			for (SIZET i = 0; i < N; i++)
			{
				m_Data[i] = obA.data()[i];
			}
		}

		~CObfuscatedDataA()
		{

			for (SIZET i = 0; i < N; i++)
			{
				m_Data[i] = 0;
			}
		}

		operator char*()
		{
			Decrypt();
			return m_Data;
		}

		void Decrypt()
		{
			if (m_isEncrypted)
			{
				CipherA(m_Data, N, KEY);
				m_isEncrypted = false;
			}
		}

		void Encrypt()
		{
			if (!m_isEncrypted)
			{
				CipherA(m_Data, N, KEY);
				m_isEncrypted = true;
			}
		}

		bool IsEncrypted() const
		{
			return m_isEncrypted;
		}

	private:

		char m_Data[N];


		bool m_isEncrypted{ true };
	};

	template <SIZET N, KEYT KEY>
	class CObfuscatedDataW
	{
	public:
		CObfuscatedDataW(const CObfuscatorW<N, KEY>& obW)
		{
			for (SIZET i = 0; i < N; i++)
			{
				m_Data[i] = obW.data()[i];
			}
		}

		~CObfuscatedDataW()
		{
			for (SIZET i = 0; i < N; i++)
			{
				m_Data[i] = 0;
			}
		}

		operator wchar_t*()
		{
			Decrypt();
			return m_Data;
		}

		void Decrypt()
		{
			if (m_isEncrypted)
			{
				CipherW(m_Data, N, KEY);
				m_isEncrypted = false;
			}
		}

		void Encrypt()
		{
			if (!m_isEncrypted)
			{
				CipherW(m_Data, N, KEY);
				m_isEncrypted = true;
			}
		}

		bool IsEncrypted() const
		{
			return m_isEncrypted;
		}

	private:

		//字符串的本地存储。调用IsEncrypted()来检查是否为
		//当前字符串是加密的。
		wchar_t m_Data[N];

	
		bool m_isEncrypted{ true };
	};

	//该函数的存在纯粹是为了提取元素'N'的数量
	template <SIZET N, KEYT KEY = CC_OBFUSCATE_DEFAULT_KEY>
	constexpr auto MakeObfuscatorA(const char(&data)[N])
	{
		return CObfuscatorA<N, KEY>(data);
	}

	//该函数的存在纯粹是为了提取元素'N'的数量
	template <SIZET N, KEYT KEY = CC_OBFUSCATE_DEFAULT_KEY>
	constexpr auto MakeObfuscatorW(const wchar_t(&wdata)[N])
	{
		return CObfuscatorW<N, KEY>(wdata);
	}

_CCAPI_END

// Lambda表达式
// 在编译时混淆字符串'data'并返回一个引用
// CObfuscatedData对象，具有全局生命周期，
// 有函数解密字符串，并且可以隐式转换为char*/wchar_t*
#define CC_OBFUSCATE_KEYA(data, key) \
	[]() -> _CCAPI CObfuscatedDataA<sizeof(data)/sizeof(data[0]), key>& { \
static_assert(sizeof(decltype(key)) == sizeof(_CCAPI KEYT), "key must be a 64 bit unsigned integer"); \
static_assert((key) >= (1ull << 56), "key must span all 8 bytes"); \
constexpr auto n = sizeof(data) / sizeof(data[0]); \
constexpr auto oba = _CCAPI MakeObfuscatorA<n, key>(data); \
static auto obaData = _CCAPI CObfuscatedDataA<n, key>(oba); \
return obaData; \
}()


// 使用static_assert，可以在编译期发现更多的错误，用编译器来强制保证一些契约，
// 帮助我们改善编译信息的可读性，尤其是用于模板时。
#define CC_OBFUSCATE_KEYW(wdata, key) \
	[]() -> _CCAPI CObfuscatedDataW<sizeof(wdata)/sizeof(wdata[0]), key>& { \
static_assert(sizeof(decltype(key)) == sizeof(_CCAPI KEYT), "key must be a 64 bit unsigned integer"); \
static_assert((key) >= (1ull << 56), "key must span all 8 bytes"); \
constexpr auto n = sizeof(wdata) / sizeof(wdata[0]); \
constexpr auto obw = _CCAPI MakeObfuscatorW<n, key>(wdata); \
static auto obwData = _CCAPI CObfuscatedDataW<n, key>(obw); \
return obwData; \
}()

// _DEBUG关闭混淆
#ifdef _DEBUG
#define COSTR_OFF
#endif // _DEBUG

// 定义COSTR_OFF关闭混淆
#ifdef COSTR_OFF
#define CC_OBFUSCATE_STRINGA(data)	data
#define CC_OBFUSCATE_STRINGW(wdata) wdata

#else
// 返回隐式转换为char*/wchar_t*仅仅在宏所在函数作用域内有效，脱离函数作用域将被释放。
#define CC_OBFUSCATE_STRINGA(data)	CC_OBFUSCATE_KEYA(data, CC_OBFUSCATE_DEFAULT_KEY)
#define CC_OBFUSCATE_STRINGW(wdata) CC_OBFUSCATE_KEYW(wdata, CC_OBFUSCATE_DEFAULT_KEY)

#endif

// 简化名称长度
#define COSTRA CC_OBFUSCATE_STRINGA
#define COSTRW CC_OBFUSCATE_STRINGW

#if defined(UNICODE)
#define COSTR COSTRW
#else
#define COSTR COSTRA
#endif

#endif
