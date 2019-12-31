#ifndef _DLIB_EXCEPTIONS_H
#define _DLIB_EXCEPTIONS_H


#include "exceptions.h"


class DlibException: public NvidiaFanControllerException
{
  using NvidiaFanControllerException::NvidiaFanControllerException;
};


class DlibLoadingException: public DlibException
{
  using DlibException::DlibException;
};


class DlibUnloadingException: public DlibException
{
  using DlibException::DlibException;
};


class DlibFunctionRetrievalException: public DlibException
{
  using DlibException::DlibException;
};


#endif // _DLIB_EXCEPTIONS_H