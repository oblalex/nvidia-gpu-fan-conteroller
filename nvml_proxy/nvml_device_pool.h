#ifndef _NVML_DEVICE_POOL_H
#define _NVML_DEVICE_POOL_H

#include <vector>

#include "nvml_proxy.h"
#include "nvml_device.h"


class NVMLDevicePool
{
public:
  NVMLDevicePool(const NVMLProxy& api);

private:
  std::vector<NVMLDevice> devices;

public:

  std::vector<NVMLDevice>::const_iterator
  cbegin()
  const noexcept
  {
    return devices.cbegin();
  }


  std::vector<NVMLDevice>::const_iterator
  cend()
  const noexcept
  {
    return devices.cend();
  }


  size_t
  size()
  const noexcept
  {
    return devices.size();
  }


  bool
  empty()
  const noexcept
  {
    return devices.empty();
  }


  const NVMLDevice&
  operator[](size_t n)
  const
  {
    return devices[n];
  }


  const NVMLDevice&
  at(size_t n)
  const
  {
    return devices.at(n);
  }

  
  const NVMLDevice&
  front()
  const
  {
    return devices.front();
  }


  const NVMLDevice&
  back()
  const
  {
    return devices.back();
  }
};


#endif // _NVML_DEVICE_POOL_H