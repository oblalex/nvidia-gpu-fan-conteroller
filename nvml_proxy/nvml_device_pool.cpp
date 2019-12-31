#include "nvml_device_pool.h"


NVMLDevicePool::NVMLDevicePool(
  const NVMLProxy& api
)
{
  const auto count = api.try_get_devices_count();
  devices.reserve(count);

  for (unsigned int idx = 0; idx < count; ++idx)
  {
    devices.push_back(NVMLDevice {
      idx,
      api.try_get_device_handle(idx),
      api
    });
  }
}
