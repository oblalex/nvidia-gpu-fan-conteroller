#include <optional>
#include <string>

#include "exceptions.h"


NvidiaFanControllerException::NvidiaFanControllerException(
  std::optional<std::string_view> message
)
: m_message {message}
{
}


const char*
NvidiaFanControllerException::what() const noexcept
{
  return (
    m_message.has_value()
    ? m_message.value().c_str()
    : nullptr
  );
}
