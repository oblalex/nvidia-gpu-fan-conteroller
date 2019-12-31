#ifndef _NVML_PROXY_H
#define _NVML_PROXY_H


#include <optional>
#include <string>

#include "compat.h"
#include "dlib.h"
#include "exceptions.h"
#include "nvml_types.h"


#ifdef __WINDOWS__
  constexpr auto NVML_LIB_NAME {"nvml.dll"};
#endif 
#ifdef __UNIX__
  constexpr auto NVML_LIB_NAME {"libnvidia-ml.so"};
#endif


class NVMLProxy
{
public:
  class Exception: public NvidiaFanControllerException
  {
  public:
    Exception(
      std::string_view arg_message,
      std::string_view arg_details,
      nvmlReturn_t     arg_code
    );

    const std::string  message;
    const std::string  details;
    const nvmlReturn_t code;
  };

  class DeviceException: public Exception
  {
  public:
    DeviceException(
      std::string_view  arg_message,
      std::string_view  arg_details,
      nvmlReturn_t      arg_code,
      nvmlDeviceIndex_t arg_index
    );

    const nvmlDeviceIndex_t index;
  };

  NVMLProxy(std::string_view lib_name = NVML_LIB_NAME);

  NVMLProxy(const NVMLProxy&) = delete;
  NVMLProxy(NVMLProxy&&)      = default;

  NVMLProxy& operator=(const NVMLProxy&) = delete;
  NVMLProxy& operator=(NVMLProxy&&)      = default;

  ~NVMLProxy();

private:
  const Dlib lib;

  nvmlInit_t        nvmlInit        {nullptr};
  nvmlShutdown_t    nvmlShutdown    {nullptr};
  nvmlErrorString_t nvmlErrorString {nullptr};

  nvmlSystemGetDriverVersion_t nvmlSystemGetDriverVersion {nullptr};
  nvmlSystemGetNVMLVersion_t   nvmlSystemGetNVMLVersion   {nullptr};  

  nvmlDeviceGetCount_t  nvmlDeviceGetCount  {nullptr};  
  nvmlDeviceGetName_t   nvmlDeviceGetName   {nullptr};
  nvmlDeviceGetSerial_t nvmlDeviceGetSerial {nullptr};
  nvmlDeviceGetUUID_t   nvmlDeviceGetUUID   {nullptr};  

  nvmlDeviceGetVbiosVersion_t     nvmlDeviceGetVbiosVersion     {nullptr};
  nvmlDeviceGetHandleByIndex_t    nvmlDeviceGetHandleByIndex    {nullptr};
  nvmlDeviceGetFanSpeed_t         nvmlDeviceGetFanSpeed         {nullptr};
  nvmlDeviceGetTemperature_t      nvmlDeviceGetTemperature      {nullptr};
  nvmlDeviceGetPowerUsage_t       nvmlDeviceGetPowerUsage       {nullptr};
  nvmlDeviceGetUtilizationRates_t nvmlDeviceGetUtilizationRates {nullptr};
  nvmlDeviceGetMemoryInfo_t       nvmlDeviceGetMemoryInfo       {nullptr};

  nvmlDeviceGetCudaComputeCapability_t nvmlDeviceGetCudaComputeCapability {nullptr};
  nvmlDeviceGetTemperatureThreshold_t  nvmlDeviceGetTemperatureThreshold  {nullptr};  

  nvmlDeviceGetApplicationsClock_t     nvmlDeviceGetApplicationsClock    {nullptr};
  nvmlDeviceSetApplicationsClocks_t    nvmlDeviceSetApplicationsClocks   {nullptr};
  nvmlDeviceResetApplicationsClocks_t  nvmlDeviceResetApplicationsClocks {nullptr};

  nvmlDeviceSetDefaultAutoBoostedClocksEnabled_t nvmlDeviceSetDefaultAutoBoostedClocksEnabled {nullptr};
  nvmlDeviceGetAutoBoostedClocksEnabled_t        nvmlDeviceGetAutoBoostedClocksEnabled {nullptr};

  nvmlDeviceGetSupportedMemoryClocks_t    nvmlDeviceGetSupportedMemoryClocks    {nullptr};
  nvmlDeviceGetSupportedGraphicsClocks_t  nvmlDeviceGetSupportedGraphicsClocks  {nullptr};

  nvmlDeviceGetPowerManagementLimitConstraints_t nvmlDeviceGetPowerManagementLimitConstraints {nullptr};
  nvmlDeviceGetPowerManagementDefaultLimit_t     nvmlDeviceGetPowerManagementDefaultLimit     {nullptr};

  nvmlDeviceGetPowerManagementMode_t  nvmlDeviceGetPowerManagementMode  {nullptr};  
  nvmlDeviceSetPowerManagementLimit_t nvmlDeviceSetPowerManagementLimit {nullptr};
  nvmlDeviceGetPowerManagementLimit_t nvmlDeviceGetPowerManagementLimit {nullptr};
  nvmlDeviceGetEnforcedPowerLimit_t   nvmlDeviceGetEnforcedPowerLimit   {nullptr};

  nvmlDeviceGetPersistenceMode_t nvmlDeviceGetPersistenceMode {nullptr};
  nvmlDeviceSetPersistenceMode_t nvmlDeviceSetPersistenceMode {nullptr};

  void try_bind_functions();

  void try_init()     const;
  void try_shutdown() const;


  nvmlOptionalTemperature_t
  try_maybe_get_device_temperature_threshold(
    const nvmlDeviceIndex_t     index,
    const nvmlDevice_t&         handle,
    nvmlTemperatureThresholds_t thresholdType
  ) const;

public:
  nvmlString_t try_get_driver_version() const;
  nvmlString_t try_get_nvml_version()   const;

  unsigned int try_get_devices_count() const;


  nvmlString_t
  get_error_message_from_status(
    const nvmlReturn_t& status
  )
  const
  {
    return nvmlErrorString(status);
  }


  nvmlDevice_t
  try_get_device_handle(
    const nvmlDeviceIndex_t index
  ) const;


  nvmlString_t
  try_get_device_name(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalString_t
  try_maybe_get_device_serial(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalString_t
  try_maybe_get_device_uuid(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlString_t
  try_get_device_VBIOS_version(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalFanSpeed_t
  try_maybe_get_device_fan_speed(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlReturn_t
  get_device_fan_speed_raw(
    const nvmlDevice_t& handle,
    nvmlFanSpeed_t*     value
  ) const;


  nvmlOptionalTemperature_t
  try_maybe_get_device_temperature(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlReturn_t
  get_device_temperature_raw(
    const nvmlDevice_t& handle,
    nvmlTemperature_t*  temp
  ) const;


  nvmlOptionalPower_t
  try_maybe_get_device_power_usage(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlReturn_t
  get_device_power_usage_raw(
    const nvmlDevice_t& handle,
    nvmlPower_t*        value
  ) const;


  nvmlOptionalUtilization_t
  try_maybe_get_device_utilization_rates(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlReturn_t
  get_device_utilization_rates_raw(
    const nvmlDevice_t& handle,
    nvmlUtilization_t*  value
  ) const;


  nvmlCudaComputeCapability_t
  try_get_device_CUDA_compute_capability(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlTemperatureThresholdsReport_t
  try_get_device_temperature_thresholds(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlMemoryInfo_t
  try_get_device_memory_info(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlReturn_t
  get_device_memory_info_raw(
    const nvmlDevice_t& handle,
    nvmlMemoryInfo_t*   value
  ) const;


  void
  try_set_device_default_autoboosted_clocks_enabled(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle,
    const nvmlEnableState_t enabled
  ) const;


  nvmlOptionalAutoboostedClocksState_t
  try_maybe_get_device_autoboosted_clocks_enabled(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalClockSpeedVector_t
  try_maybe_get_device_supported_memory_clocks(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalClockSpeedVector_t
  try_maybe_get_device_supported_graphics_clocks(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle,
    const nvmlClockSpeed_t  memoryClockMHz
  ) const;


  nvmlOptionalClockSpeedsMap_t
  try_maybe_get_device_supported_clock_speeds(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalClockSpeed_t
  try_maybe_get_device_applications_clock(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle,
    const nvmlClockType_t   clockType
  ) const;


  nvmlOptionalClockSpeed_t
  try_maybe_get_device_applications_memory_clock(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalClockSpeed_t
  try_maybe_get_device_applications_graphics_clock(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlApplicationsClockSpeedsReport_t
  try_get_device_applications_clocks(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  void
  try_set_device_applications_clocks(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle,
    const nvmlClockSpeed_t  memClockMHz,
    const nvmlClockSpeed_t  graphicsClockMHz
  ) const;


  void
  try_reset_device_applications_clocks(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalPowerManagementLimitConstraints_t
  try_maybe_get_device_power_management_limit_constrains(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalPower_t
  try_maybe_get_device_power_management_default_limit(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  void
  try_set_device_power_management_limit(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle,
    const nvmlPower_t       value
  ) const;


  nvmlOptionalPower_t
  try_maybe_get_device_power_management_limit(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalEnableState_t
  try_maybe_get_device_power_management_mode(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  nvmlOptionalPower_t
  try_maybe_get_device_enforced_power_limit(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;


  void
  try_set_device_persistence_mode(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle,
    const nvmlEnableState_t value
  ) const;


  nvmlOptionalEnableState_t
  try_maybe_get_device_persistence_mode(
    const nvmlDeviceIndex_t index,
    const nvmlDevice_t&     handle
  ) const;  
};


#endif // _NVML_PROXY_H