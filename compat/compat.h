#ifndef _COMPAT_H
#define _COMPAT_H


#if __has_include(<windows.h>)
  #define __WINDOWS__
#elif __has_include(<dlfcn.h>)
  #define __UNIX__
#else
  #error Unsupported target platform: neither <windows.h> nor <dlfcn.h> is present
#endif


#endif // _NVML_DEVICE_H