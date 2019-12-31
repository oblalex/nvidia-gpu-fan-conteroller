#include <iostream>

#include "nvml_proxy.h"
#include "nvml_device_pool.h"


void
report_nvml_info(const NVMLProxy& nvml)
{
  std::cout
    << "driver_version:               " << nvml.try_get_driver_version() << "\n"
    << "nvml_version:                 " << nvml.try_get_nvml_version()   << "\n"
  ;
}


void
report_initial_states_for_single_nvml_device(const NVMLDevice& device)
{
  std::cout
    << "- device_index:               " << device.index                << "\n"
    << "  name:                       " << device.name                 << "\n"
    << "  uuid:                       " << device.uuid.value_or("n/a") << "\n"
    << "  VBIOS version:              " << device.VBIOS_version        << "\n"
    << "  memory_total:               " << device.memory_total << " B" << "\n"

    << "  CUDA_compute_compatibility: " << "\n"
    << "    full_version:             " << (                
              std::to_string(device.CUDA.major) +
              "." +
              std::to_string(device.CUDA.minor)
            ) << "\n"
    << "    major_version:            " << (
              std::to_string(device.CUDA.major)
            ) << "\n"
    << "    minor_version:            " << (
              std::to_string(device.CUDA.minor)
            ) << "\n"

    << "  persistence_mode:           " << "\n"
    << "    is_supported:             " << (
              device.feature_persistence_mode == nvmlEnableState_t::NVML_FEATURE_ENABLED
              ? "yes"
              : "no"
            ) << "\n"
    << "    is_enabled:               " << (
              device.feature_persistence_mode == nvmlEnableState_t::NVML_FEATURE_ENABLED
              ? (
                device.persistence_mode_initial.value() == nvmlEnableState_t::NVML_FEATURE_ENABLED
                ? "yes"
                : "no"
              )
              : "n/a"
            ) << "\n"

    << "  power_management_limits:    " << "\n"
    << "    is_enabled:               " << (
              device.feature_power_management == nvmlEnableState_t::NVML_FEATURE_ENABLED
              ? "yes"
              : "no"
            ) << "\n"
    << "    current:                  " << (
              device.power_management_limit_initial.has_value()
              ? std::to_string(device.power_management_limit_initial.value()) + " mW"
              : "n/a"
            ) << "\n"
    << "    default:                  " << (
              device.power_management_limit_default.has_value()
              ? std::to_string(device.power_management_limit_default.value()) + " mW"
              : "n/a"
            ) << "\n"
    << "    min:                      " << (
              device.power_management_limit_constrains.has_value()
              ? std::to_string(device.power_management_limit_constrains.value().min_limit) + " mW"
              : "n/a"
            ) << "\n"
    << "    max:                      " << (
              device.power_management_limit_constrains.has_value()
              ? std::to_string(device.power_management_limit_constrains.value().max_limit) + " mW"
              : "n/a"
            ) << "\n"
  
    << "  enforced_power_limit:       " << (
            device.feature_enforced_power_limit == nvmlEnableState_t::NVML_FEATURE_ENABLED
            ? std::to_string(device.try_maybe_get_enforced_power_limit().value()) + " mW"
            : "n/a"
          ) << "\n"

    << "  temperature_thresholds:     " << "\n"
    << "    shutdown:                 " << (
                device.temperature_thresholds.shutdown.has_value()
              ? std::to_string(device.temperature_thresholds.shutdown.value()) + " deg.C"
              : "n/a"
            ) << "\n"
    << "    slowdown:                 " << (
                device.temperature_thresholds.slowdown.has_value()
              ? std::to_string(device.temperature_thresholds.slowdown.value()) + " deg.C"
              : "n/a"
            ) << "\n"
    << "    gpu_max:                  " << (
                device.temperature_thresholds.gpu_max.has_value()
              ? std::to_string(device.temperature_thresholds.gpu_max.value())  + " deg.C"
              : "n/a"
            ) << "\n"
    << "    mem_max:                  " << (
                device.temperature_thresholds.mem_max.has_value()
              ? std::to_string(device.temperature_thresholds.mem_max.value())  + " deg.C"
              : "n/a"
            ) << "\n"

    << "  clocks_autoboost_support:   " << "\n"
    << "    is_supported:             "  << (
              device.feature_autoboosted_clocks == nvmlEnableState_t::NVML_FEATURE_ENABLED
              ? "yes"
              : "no"
            ) << "\n"
    << "    is_default_enabled:       " << (
              device.feature_autoboosted_clocks == nvmlEnableState_t::NVML_FEATURE_ENABLED
              ? (
                device.initial_clocks_autoboost_config.value().is_default_enabled == nvmlEnableState_t::NVML_FEATURE_ENABLED
                ? "yes"
                : "no"
              )
              : "n/a"                
            ) << "\n"
    << "    is_enabled:               " << (
              device.feature_autoboosted_clocks == nvmlEnableState_t::NVML_FEATURE_ENABLED
              ? (
                device.initial_clocks_autoboost_config.value().is_enabled == nvmlEnableState_t::NVML_FEATURE_ENABLED
                ? "yes"
                : "no"
              )
              : "n/a"
            ) << "\n"

    << "  applications_clocks_speeds: " << "\n"
    << "    memory:                   " << (
              device.initial_applications_clocks_speeds.memory.has_value()
              ? std::to_string(device.initial_applications_clocks_speeds.memory.value()) + " MHz"
              : "n/a"
            ) << "\n"
    << "    graphics:                 " << (
                  device.initial_applications_clocks_speeds.graphics.has_value()
                  ? std::to_string(device.initial_applications_clocks_speeds.graphics.value()) + " MHz"
                  : "n/a"
                ) << "\n"

    << "  clock_speeds_supported:     "
  ;

  if (!device.supported_clock_speeds.has_value())
  {
    std::cout << "n/a";
  }

  std::cout << "\n";

  if (device.supported_clock_speeds.has_value())
  {
    for (const auto& [memory_clock, graphics_clocks] : device.supported_clock_speeds.value())
    {
      std::cout
        << "  - memory: "  << std::to_string(memory_clock) << " MHz" << "\n"
        << "    graphics:" << "\n"
        ;

      for (const auto& graphics_clock : graphics_clocks)
      {
        std::cout << "    - " << std::to_string(graphics_clock) << " MHz" << "\n";
      }
    }
  }
}


void report_initial_states_for_all_nvml_devices(const NVMLDevicePool& devices)
{
  const auto count = devices.size();
  if (!count)
  {
    return;
  }

  std::cout
    << "devices_count:                " << count << "\n"
    << "devices:                      " << "\n"
  ;

  for (auto device = devices.cbegin(); device != devices.cend(); ++device)
  {
    report_initial_states_for_single_nvml_device(*device);
  }
}


void
run()
{
  const NVMLProxy nvml;
  report_nvml_info(nvml);

  const NVMLDevicePool devices {nvml};
  report_initial_states_for_all_nvml_devices(devices);
}


int
main()
{ 
  try {    
    run();
    return 0;

  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    throw;
  }
}
