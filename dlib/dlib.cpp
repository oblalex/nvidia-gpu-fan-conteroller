#include <string>

#include "dlib.h"


Dlib::Dlib(std::string_view lib_name)
: name {lib_name}
{
  try {
    handle = try_load_dlib(name);
  } catch (const Dlib::LoadingException& e) {
    std::string message {
      "failed to load library '" + name + "'; "
      "make sure it is installed and env var '" + LIBS_PATH_ENV_VAR + "' "
      "contains path to the library"
    };

    auto* details = e.what();
    if (details) {
      message += "; details: " + std::string(details);
    }

    throw Dlib::LoadingException {message};
  }
}


Dlib::~Dlib()
{
  try {
    try_unload_dlib(handle);
  } catch (const Dlib::UnloadingException&) {
    // TODO: enlog
  }
}


dfunc_handle_t
Dlib::try_retrieve_function(
  std::string_view function_name
)
const
{
  try {
    return try_retrieve_dfunc(handle, function_name);
  } catch(const Dlib::FunctionRetrievalException& e) {
    std::string message {
      "failed to load function '" + std::string(function_name) + "' "
      "from library '" + name + "'"
    };

    auto* details = e.what();
    if (details) {
      message += "; details: " + std::string(details);
    }

    throw Dlib::FunctionRetrievalException {message};    
  }
}
