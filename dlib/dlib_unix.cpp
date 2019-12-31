#include <dlfcn.h>

#include <optional>
#include <string>

#include "dlib_exceptions.h"
#include "dlib_unix.h"


namespace {
  
  inline
  void
  maybe_reset_last_error_message() {
    ::dlerror();
  }


  inline
  std::optional<std::string>
  maybe_get_last_error_message() {
    
    if (
      char* message = ::dlerror();
      message != nullptr
    ) {
      return std::string {message};
    }

    return std::nullopt;
  }

} // namespace


dlib_handle_t
try_load_dlib(std::string_view lib_name)
{
  ::maybe_reset_last_error_message();

  dlib_handle_t handle = ::dlopen(
    /* filename = */ lib_name.data(),
    /* flag     = */ RTLD_LAZY
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
  ::maybe_reset_last_error_message();

  if (::dlclose(
    /* handle = */ lib_handle
  ) != 0) {
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
  ::maybe_reset_last_error_message();

  dfunc_handle_t handle = ::dlsym(
    /* handle = */ lib_handle,
    /* symbol = */ function_name.data()
  );

  if (handle) {
    return handle;
  }

  throw DlibFunctionRetrievalException {
    ::maybe_get_last_error_message()
  };
}