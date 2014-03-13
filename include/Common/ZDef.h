#include <iostream>

#ifndef ZHTTP_ZHTTPDEFINITION_HEADER
#define ZHTTP_ZHTTPDEFINITION_HEADER


#define ZHTTPBEGIN	namespace ZHttp {

#define ZHTTPEND	}

ZHTTPBEGIN
//define Type

#define ZInt		int
#define ZUInt		unsigned int
#define ZInt32		__int32
#define ZInt64		__int64
#define ZLong		long
#define ZULong		unsigned long
#define ZStringA	std::string
#define ZStringW	std::wstring

#ifdef _UNICODE
#define ZString		ZStringW
#else
#define ZString		ZStringA
#endif

#define ZDouble		double
#define ZBool		bool
#define ZTrue		true
#define ZFalse		false

class UnCopyable
{
protected:
	UnCopyable()
	{}
	~UnCopyable()
	{}
private:
	UnCopyable(const UnCopyable&);
	UnCopyable& operator=(const UnCopyable&);
};


ZHTTPEND
#endif

//some class
/**
*用来管理资源，需要支持c++11
*
*
*
*
*/


#include <functional>
#ifndef SCOPEGUARD_HEADER
#define SCOPEGUARD_HEADER

ZHTTPBEGIN

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

//释放资源宏
#define ON_SCOPE_EXIT(callback) ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)

class ScopeGuard
{
public:
	explicit ScopeGuard(std::function<void()> onExitScope)
		: _onExitScope(onExitScope), _dismissed(false)
	{}
	~ScopeGuard()
	{
		if (!_dismissed) {
			_onExitScope();
		}
	}

	void Dismiss()
	{
		_dismissed = true;
	}
private:
	ScopeGuard(ScopeGuard const&);
	ScopeGuard& operator=(ScopeGuard const&);
private:
	std::function<void()> _onExitScope;
	bool _dismissed;
};

ZHTTPEND
#endif