#include <vector>

#include "nvml_device.h"
#include "nvml_proxy.h"
#include "nvml_types.h"


NVMLDevice::NVMLDevice(
  const nvmlDeviceIndex_t arg_index,
  const nvmlDevice_t      arg_handle,
  const NVMLProxy&        arg_api
)
: index  {arg_index},
  handle {arg_handle},
  api    {arg_api},

  name {arg_api.try_get_device_name(
    arg_index,
    arg_handle
  )},

  serial {arg_api.try_maybe_get_device_serial(
    arg_index,
    arg_handle
  )},

  uuid {arg_api.try_maybe_get_device_uuid(
    arg_index,
    arg_handle
  )},

  VBIOS_version {arg_api.try_get_device_VBIOS_version(
    arg_index,
    arg_handle
  )},

  memory_total {arg_api.try_get_device_memory_info(
    arg_index,
    arg_handle
  ).total},

  CUDA {arg_api.try_get_device_CUDA_compute_capability(
    arg_index,
    arg_handle
  )},

  temperature_thresholds {arg_api.try_get_device_temperature_thresholds(
    arg_index,
    arg_handle
  )},

  initial_clocks_autoboost_config {arg_api.try_maybe_get_device_autoboosted_clocks_enabled(
    arg_index,
    arg_handle
  )},

  initial_applications_clocks_speeds {arg_api.try_get_device_applications_clocks(
    arg_index,
    arg_handle
  )},

  power_management_limit_constrains {arg_api.try_maybe_get_device_power_management_limit_constrains(
    arg_index,
    arg_handle
  )},

  power_management_limit_default {arg_api.try_maybe_get_device_power_management_default_limit(
    arg_index,
    arg_handle
  )},

  power_management_limit_initial {arg_api.try_maybe_get_device_power_management_limit(
    arg_index,
    arg_handle
  )},

  power_management_mode_initial {arg_api.try_maybe_get_device_power_management_mode(
    arg_index,
    arg_handle
  )},

  persistence_mode_initial {arg_api.try_maybe_get_device_persistence_mode(
    arg_index,
    arg_handle
  )},

  supported_clock_speeds {arg_api.try_maybe_get_device_supported_clock_speeds(
    arg_index,
    arg_handle
  )},

  feature_fan_speed {arg_api.try_maybe_get_device_fan_speed(index, handle).has_value()
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_temperature {arg_api.try_maybe_get_device_temperature(index, handle).has_value()
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_power_usage {arg_api.try_maybe_get_device_power_usage(index, handle).has_value()
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_utilization_rates {arg_api.try_maybe_get_device_utilization_rates(index, handle).has_value()
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_autoboosted_clocks {arg_api.try_maybe_get_device_autoboosted_clocks_enabled(index, handle).has_value()
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_applications_memory_clocks {arg_api.try_maybe_get_device_applications_memory_clock(index, handle).has_value()          
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_applications_graphics_clocks {arg_api.try_maybe_get_device_applications_graphics_clock(index, handle).has_value()          
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_power_management {arg_api.try_maybe_get_device_power_management_mode(index, handle).has_value()          
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_enforced_power_limit {arg_api.try_maybe_get_device_enforced_power_limit(index, handle).has_value()
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  },

  feature_persistence_mode {arg_api.try_maybe_get_device_persistence_mode(index, handle).has_value()
    ? nvmlEnableState_t::NVML_FEATURE_ENABLED
    : nvmlEnableState_t::NVML_FEATURE_DISABLED
  }
{
}

  void
  NVMLDevice::try_maybe_restore()
  const
  {
    std::vector<NVMLProxy::DeviceException> errors;

    if (feature_autoboosted_clocks == nvmlEnableState_t::NVML_FEATURE_ENABLED)
    {
      try {
        try_set_default_autoboosted_clocks_enabled(
          initial_clocks_autoboost_config.value().is_default_enabled
        );
      } catch (const NVMLProxy::DeviceException& e) {
        errors.push_back(e);
      }
    }

    if (
      (feature_applications_memory_clocks   == nvmlEnableState_t::NVML_FEATURE_ENABLED)
      &&
      (feature_applications_graphics_clocks == nvmlEnableState_t::NVML_FEATURE_ENABLED)
    )
    {
      try {
        try_set_device_applications_clocks(
          initial_applications_clocks_speeds.memory.value(),
          initial_applications_clocks_speeds.graphics.value()
        );
      } catch (const NVMLProxy::DeviceException& e) {
        errors.push_back(e);
      }
    }

    if (feature_power_management == nvmlEnableState_t::NVML_FEATURE_ENABLED)
    {
      try {
        try_set_power_management_limit(power_management_limit_initial.value());
      } catch (const NVMLProxy::DeviceException& e) {
        errors.push_back(e);
      }
    }

    if (feature_persistence_mode == nvmlEnableState_t::NVML_FEATURE_ENABLED)
    {
      try {
        try_set_persistence_mode(persistence_mode_initial.value());
      } catch (const NVMLProxy::DeviceException& e) {
        errors.push_back(e);
      }
    }

    if (!errors.empty())
    {
      // TODO: log all errors
      throw errors.front();
    }        
  }
