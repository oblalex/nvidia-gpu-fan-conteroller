#ifndef _DLIB_WINDOWS_H
#define _DLIB_WINDOWS_H

#include <windows.h>


using dlib_handle_t  = HMODULE;
using dfunc_handle_t = FARPROC;

constexpr auto LIBS_PATH_ENV_VAR {"PATH"};


#endif // _DLIB_WINDOWS_H