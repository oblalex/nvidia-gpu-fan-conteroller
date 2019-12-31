#include <windows.h>

#include <optional>
#include <string>

#include "dlib_exceptions.h"
#include "dlib_windows.h"


namespace {

  std::optional<std::string>
  maybe_get_last_error_message() {    

    if (
      DWORD dwLastError = ::GetLastError();
      dwLastError != 0
    ) {
      const DWORD size = FORMAT_MESSAGE_ALLOCATE_BUFFER;
      char buffer[size];

      if (::FormatMessageA(
        /* dwFlags      = */ FORMAT_MESSAGE_FROM_SYSTEM,
        /* lpSource     = */ NULL,
        /* dwMessageId  = */ dwLastError,
        /* dwLanguageId = */ MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        /* lpBuffer     = */ reinterpret_cast<LPSTR>(buffer),
        /* nSize        = */ size,
        /* *Arguments   = */ NULL
      ) != 0)
      {
        return std::string {buffer};
      }
    }
    return std::nullopt;
  }

} // namespace


dlib_handle_t
try_load_dlib(std::string_view lib_name)
{
  dlib_handle_t handle = ::LoadLibraryA(
    /* lpLibFileName = */ lib_name.data()
  );

  if (handle) {
    return handle;
  }

  throw DlibLoadingException {
    ::maybe_get_last_error_message()
  };
}


void
try_unload_dlib(dlib_handle_t lib_handle)
{
  if (::FreeLibrary(
    /* hLibModule = */ lib_handle
  ) != 0)
  {
    throw DlibUnloadingException {
      ::maybe_get_last_error_message()
    };
  }  
}


dfunc_handle_t
try_retrieve_dfunc(
  dlib_handle_t    lib_handle,
  std::string_view function_name
)
{
  dfunc_handle_t handle = ::GetProcAddress(
    /* hModule    = */ lib_handle,
    /* lpProcName = */ function_name.data()
  );

  if (handle) {
    return handle;
  }

  throw DlibFunctionRetrievalException {
    ::maybe_get_last_error_message()
  };
}
