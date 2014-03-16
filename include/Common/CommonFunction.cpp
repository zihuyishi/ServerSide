#include "CommonFunction.h"
#include <locale>
ZHTTPBEGIN
namespace StringFunc
{
	/*
	template<typename stringT, typename charT>
	std::vector<stringT> Split(const stringT& src, charT div)
	{
		ZUInt iPos1 = 0,
			iPos2 = 0;
		std::vector<stringT> strRets;
		iPos2 = src.find(div, iPos1);
		while (iPos2 != stringT::npos)
		{
			strRets.push_back(src.substr(iPos1, iPos2 - iPos1));
			iPos1 = iPos2 + 1;
			iPos2 = src.find(div, iPos1);
		}
		strRets.push_back(src.substr(iPos1));
		return strRets;
	}
	*/
	std::vector<ZStringA> Split(const ZStringA& src, ZCharA div)
	{
		ZUInt iPos1 = 0,
			iPos2 = 0;
		std::vector<ZStringA> strRets;
		iPos2 = src.find(div, iPos1);
		while (iPos2 != ZStringA::npos)
		{
			strRets.push_back(src.substr(iPos1, iPos2 - iPos1));
			iPos1 = iPos2 + 1;
			iPos2 = src.find(div, iPos1);
		}
		strRets.push_back(src.substr(iPos1));
		return strRets;
	}
	std::vector<ZStringW> Split(const ZStringW& src, ZCharW div)
	{
		ZUInt iPos1 = 0,
			iPos2 = 0;
		std::vector<ZStringW> strRets;
		iPos2 = src.find(div, iPos1);
		while (iPos2 != ZStringW::npos)
		{
			strRets.push_back(src.substr(iPos1, iPos2 - iPos1));
			iPos1 = iPos2 + 1;
			iPos2 = src.find(div, iPos1);
		}
		strRets.push_back(src.substr(iPos1));
		return strRets;
	}
	ZStringA StrToUpper(const ZStringA& src)
	{
		ZUInt len = src.length();
		ZCharA* buffer = new ZCharA[len + 1];
		for (ZUInt i = 0; i < len; ++i)
		{
			buffer[i] = ::toupper(src[i]);
		}
		buffer[len] = 0;
		ZStringA strRet(buffer);
		return strRet;
	}
}

ZHTTPEND