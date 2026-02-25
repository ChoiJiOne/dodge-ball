#pragma once

#include <Macro/DebugBreak.h>

#if defined(DEBUG_MODE) || defined(REL_WITH_DEB_INFO_MODE) || defined(RELEASE_MODE)
#ifndef CHECK
#define CHECK(EXP)\
{\
	if(!(bool)(EXP))\
	{\
		debug_break();\
	}\
}
#endif
#ifndef ASSERT
#define ASSERT(EXP, ...)\
{\
	if(!(bool)(EXP))\
	{\
		debug_break();\
	}\
}
#endif
#else // MINSIZEREL
#ifndef CHECK
#define CHECK(EXP) ((void)(EXP))
#endif
#ifndef ASSERT
#define ASSERT(EXP, ...) ((void)(EXP))
#endif
#endif