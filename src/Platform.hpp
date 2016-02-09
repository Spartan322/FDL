#ifndef _FDL_NO_CONFIG
#	include "FDL_Config.hpp"
#endif

#ifdef _FDL_POSIX
#	include "Platform_Posix.hpp"
#	define FDL_IS_POSIX true
#	define FDL_IS_WINDOWS false
#elif defined(_FDL_WINDOWS)
#	include "Platform_Windows.hpp"
#	define FDL_IS_POSIX false
#	define FDL_IS_WINDOWS true
#else
#	include "Platform_Declare"
#endif
