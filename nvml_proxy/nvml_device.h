#ifndef _NVML_DEVICE_H
#define _NVML_DEVICE_H

#include <string>

#include "nvml_proxy.h"
#include "nvml_types.h"


class NVMLDevice
{
public:
  NVMLDevice(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t      handle,
    const NVMLProxy&        api
  );

  NVMLDevice(const NVMLDevice&) = delete;
  NVMLDevice(NVMLDevice&&)      = default;

  NVMLDevice& operator=(const NVMLDevice&) = delete;
  NVMLDevice& operator=(NVMLDevice&&)      = default;

  ~NVMLDevice() = default;

private:
  const nvmlDevice_t handle;
  const NVMLProxy&   api;

public:
  const nvmlDeviceIndex_t    index;
  const nvmlString_t         name;
  const nvmlOptionalString_t serial;
  const nvmlOptionalString_t uuid;  
  const nvmlString_t         VBIOS_version;
  const nvmlMemory_t         memory_total;

  const nvmlCudaComputeCapability_t          CUDA;
  const nvmlTemperatureThresholdsReport_t    temperature_thresholds;  

  const nvmlOptionalAutoboostedClocksState_t initial_clocks_autoboost_config;
  const nvmlApplicationsClockSpeedsReport_t  initial_applications_clocks_speeds;

  const nvmlOptionalPowerManagementLimitConstraints_t power_management_limit_constrains;

  const nvmlOptionalPower_t          power_management_limit_default;
  const nvmlOptionalPower_t          power_management_limit_initial;
  const nvmlOptionalEnableState_t    power_management_mode_initial;

  const nvmlOptionalEnableState_t    persistence_mode_initial;

  const nvmlOptionalClockSpeedsMap_t supported_clock_speeds;


  const nvmlEnableState_t feature_fan_speed;
  const nvmlEnableState_t feature_temperature;
  const nvmlEnableState_t feature_power_usage;
  const nvmlEnableState_t feature_utilization_rates;
  const nvmlEnableState_t feature_autoboosted_clocks;
  const nvmlEnableState_t feature_applications_memory_clocks;
  const nvmlEnableState_t feature_applications_graphics_clocks;
  const nvmlEnableState_t feature_power_management;
  const nvmlEnableState_t feature_enforced_power_limit;
  const nvmlEnableState_t feature_persistence_mode;

  void try_maybe_restore() const;

  nvmlString_t
  get_error_message_from_status(
    const nvmlReturn_t& status
  )
  const
  {
    return api.get_error_message_from_status(status);
  }

  nvmlOptionalFanSpeed_t try_maybe_get_fan_speed()
  const
  {
    return api.try_maybe_get_device_fan_speed(index, handle);
  }

  nvmlReturn_t get_fan_speed_raw(nvmlFanSpeed_t* value)
  const
  {
    return api.get_device_fan_speed_raw(handle, value);
  }


  nvmlOptionalTemperature_t try_get_temperature()
  const
  {
    return api.try_maybe_get_device_temperature(index, handle);
  }

  nvmlReturn_t get_temperature_raw(nvmlTemperature_t* value)
  const
  {
    return api.get_device_temperature_raw(handle, value);
  }


  nvmlOptionalPower_t try_maybe_get_power_usage()
  const
  {
    return api.try_maybe_get_device_power_usage(index, handle);
  }


  nvmlReturn_t try_get_power_usage(nvmlPower_t* value)
  const
  {
    return api.get_device_power_usage_raw(handle, value);
  }


  nvmlOptionalUtilization_t try_maybe_get_utilization_rates()
  const
  {
    return api.try_maybe_get_device_utilization_rates(index, handle);
  }

  nvmlReturn_t get_utilization_rates_raw(nvmlUtilization_t* value)
  const
  {
    return api.get_device_utilization_rates_raw(handle, value);
  }


  nvmlMemoryInfo_t try_get_memory_info()
  const
  {
    return api.try_get_device_memory_info(index, handle);
  }


  void try_set_default_autoboosted_clocks_enabled(nvmlEnableState_t enabled)
  const
  {
    api.try_set_device_default_autoboosted_clocks_enabled(index, handle, enabled);
  }


  nvmlOptionalAutoboostedClocksState_t try_get_autoboosted_clocks_is_enabled()
  const
  {
    return api.try_maybe_get_device_autoboosted_clocks_enabled(index, handle);
  }


  nvmlApplicationsClockSpeedsReport_t try_get_applications_clocks()
  const
  {
    return api.try_get_device_applications_clocks(index, handle);
  }


  void try_set_device_applications_clocks(
    nvmlClockSpeed_t memClockMHz,
    nvmlClockSpeed_t graphicsClockMHz
  )
  const
  {
    api.try_set_device_applications_clocks(
      index,
      handle,
      memClockMHz,
      graphicsClockMHz
    );
  }


  void try_reset_applications_clocks()
  const
  {
    return api.try_reset_device_applications_clocks(index, handle);
  }


  void try_set_power_management_limit(const nvmlPower_t value)
  const
  {
    api.try_set_device_power_management_limit(index, handle, value);
  }  


  nvmlOptionalPower_t try_maybe_get_power_management_limit()
  const
  {
    return api.try_maybe_get_device_power_management_limit(index, handle);
  }


  nvmlOptionalPower_t try_maybe_get_enforced_power_limit()
  const
  {
    return api.try_maybe_get_device_enforced_power_limit(index, handle);
  }  


  nvmlOptionalEnableState_t try_maybe_get_power_management_mode()
  const
  {
    return api.try_maybe_get_device_power_management_mode(index, handle);
  }


  void try_set_persistence_mode(const nvmlEnableState_t value)
  const
  {
    api.try_set_device_persistence_mode(index, handle, value);
  }  


  nvmlOptionalEnableState_t try_maybe_get_persistence_mode()
  const
  {
    return api.try_maybe_get_device_persistence_mode(index, handle);
  }

};


#endif // _NVML_DEVICE_H