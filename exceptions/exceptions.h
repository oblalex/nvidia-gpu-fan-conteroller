#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H


#include <exception>
#include <optional>
#include <string>


class NvidiaFanControllerException: public std::exception {
public:
  NvidiaFanControllerException(
    std::optional<std::string_view> message = std::nullopt
  );

  virtual const char* what() const noexcept override;

private:
  std::optional<std::string> m_message;
};


#endif // _EXCEPTIONS_H