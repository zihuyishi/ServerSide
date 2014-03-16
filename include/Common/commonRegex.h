#include "ZDef.h"

#ifndef ZHTTP_COMMONREGEX_HEADER
#define ZHTTP_COMMONREGEX_HEADER
ZHTTPBEGIN
namespace CommonDef
{
	const ZCharA regHttpQuery[] = R"((?<=\?)(.*))";
}
ZHTTPEND
#endif