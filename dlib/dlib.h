#ifndef _DLIB_H
#define _DLIB_H

#include <string>

#include "compat.h"
#include "dlib_exceptions.h"


#ifdef __WINDOWS__
  #include "dlib_windows.h"
#endif
#ifdef __UNIX__
  #include "dlib_unix.h"  
#endif


dlib_handle_t
try_load_dlib(std::string_view lib_name);


void
try_unload_dlib(dlib_handle_t lib_handle);


dfunc_handle_t
try_retrieve_dfunc(
  dlib_handle_t    lib_handle,
  std::string_view function_name
);


class Dlib {
public:
  using Exception                  = DlibException;
  using LoadingException           = DlibLoadingException;
  using UnloadingException         = DlibUnloadingException;
  using FunctionRetrievalException = DlibFunctionRetrievalException;

  Dlib(std::string_view lib_name);  

  Dlib(const Dlib&) = delete;
  Dlib(Dlib&&)      = default;

  Dlib& operator=(const Dlib&) = delete;
  Dlib& operator=(Dlib&&)      = default;

  ~Dlib();

private:  
  dlib_handle_t handle {};

public:
  dfunc_handle_t try_retrieve_function(
    std::string_view function_name
  ) const;

  const std::string name;
};


#endif // _DLIB_H