#if defined(_FDL_USE_CONFIG)
#	include "FDL_Config.hpp"
#endif

#ifdef _FDL_POSIX
#	include "Platform_Posix.hpp"
#elif _FDL_WINDOWS
#	include "Platform_Windows.hpp"
#else
#	error "Platform not designated, FDL failed"
#endif

#include <FDL/FDL.hpp>

// TODO: Create Generic callable functions
// Example: CreateFile would exist, but is first handled outside of OS calls, then handed to a _CreateFile function call
