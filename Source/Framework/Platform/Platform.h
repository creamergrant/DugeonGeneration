#pragma once

#if defined( _WIN64 )
#define GAMEDEV2D_OS_WINDOWS 1
#define GAMEDEV2D_OS_WIN64 1
#elif defined( _WIN32 )
#define GAMEDEV2D_OS_WINDOWS 1
#define GAMEDEV2D_OS_WIN32 1
#elif defined( __APPLE__ )
#include <TargetConditionals.h>
# if TARGET_OS_MAC
#  if defined( __x86_64__ )
#   define GAMEDEV2D_OS_MAC 1
#   define GAMEDEV2D_OS_X86_64 1
#  else
#   define GAMEDEV2D_OS_MAC 1
#   define GAMEDEV2D_OS_X86_32 1
#  endif
# endif
#else
# error Unsupported platform.
#endif
