#include "nvml_proxy.h"
#include "nvml_types.h"

#include <algorithm>
#include <string>


NVMLProxy::Exception::Exception(
  std::string_view arg_message,
  std::string_view arg_details,
  nvmlReturn_t     arg_code
)
: NvidiaFanControllerException {
    std::string(arg_message) + ": " + std::string(arg_details)
  },
  message {arg_message},
  details {arg_details},
  code    {arg_code}
{
}


NVMLProxy::DeviceException::DeviceException(
  std::string_view  arg_message,
  std::string_view  arg_details,
  nvmlReturn_t      arg_code,
  nvmlDeviceIndex_t arg_index
)
: NVMLProxy::Exception {
    std::string(arg_message) + " (device #" + std::to_string(arg_index) + ")",
    arg_details,
    arg_code
  },
  index {arg_index}
{
}


NVMLProxy::NVMLProxy(std::string_view lib_name)
: lib {lib_name}
{
  try_bind_functions();
  try_init();
}


NVMLProxy::~NVMLProxy()
{
  try_shutdown();
}


void
NVMLProxy::try_bind_functions()
{
  nvmlInit = reinterpret_cast<nvmlInit_t>(
    lib.try_retrieve_function("nvmlInit")
  );

  nvmlShutdown = reinterpret_cast<nvmlShutdown_t>(
    lib.try_retrieve_function("nvmlShutdown")
  );

  nvmlErrorString = reinterpret_cast<nvmlErrorString_t>(
    lib.try_retrieve_function("nvmlErrorString")
  );

  nvmlSystemGetDriverVersion = reinterpret_cast<nvmlSystemGetDriverVersion_t>(
    lib.try_retrieve_function("nvmlSystemGetDriverVersion")
  );

  nvmlSystemGetNVMLVersion = reinterpret_cast<nvmlSystemGetNVMLVersion_t>(
    lib.try_retrieve_function("nvmlSystemGetNVMLVersion")
  );

  nvmlDeviceGetCount = reinterpret_cast<nvmlDeviceGetCount_t>(
    lib.try_retrieve_function("nvmlDeviceGetCount")
  );

  nvmlDeviceGetHandleByIndex = reinterpret_cast<nvmlDeviceGetHandleByIndex_t>(
    lib.try_retrieve_function("nvmlDeviceGetHandleByIndex")
  );

  nvmlDeviceGetName = reinterpret_cast<nvmlDeviceGetName_t>(
    lib.try_retrieve_function("nvmlDeviceGetName")
  );

  nvmlDeviceGetSerial = reinterpret_cast<nvmlDeviceGetSerial_t>(
    lib.try_retrieve_function("nvmlDeviceGetSerial")
  );

  nvmlDeviceGetUUID = reinterpret_cast<nvmlDeviceGetUUID_t>(
    lib.try_retrieve_function("nvmlDeviceGetUUID")
  );

  nvmlDeviceGetVbiosVersion = reinterpret_cast<nvmlDeviceGetVbiosVersion_t>(
    lib.try_retrieve_function("nvmlDeviceGetVbiosVersion")
  );

  nvmlDeviceGetFanSpeed = reinterpret_cast<nvmlDeviceGetFanSpeed_t>(
    lib.try_retrieve_function("nvmlDeviceGetFanSpeed")
  );

  nvmlDeviceGetTemperature = reinterpret_cast<nvmlDeviceGetTemperature_t>(
    lib.try_retrieve_function("nvmlDeviceGetTemperature")
  );

  nvmlDeviceGetPowerUsage = reinterpret_cast<nvmlDeviceGetPowerUsage_t>(
    lib.try_retrieve_function("nvmlDeviceGetPowerUsage")
  );

  nvmlDeviceGetUtilizationRates = reinterpret_cast<nvmlDeviceGetUtilizationRates_t>(
    lib.try_retrieve_function("nvmlDeviceGetUtilizationRates")
  );

  nvmlDeviceGetCudaComputeCapability = reinterpret_cast<nvmlDeviceGetCudaComputeCapability_t>(
    lib.try_retrieve_function("nvmlDeviceGetCudaComputeCapability")
  );

  nvmlDeviceGetTemperatureThreshold = reinterpret_cast<nvmlDeviceGetTemperatureThreshold_t>(
    lib.try_retrieve_function("nvmlDeviceGetTemperatureThreshold")
  );

  nvmlDeviceGetMemoryInfo = reinterpret_cast<nvmlDeviceGetMemoryInfo_t>(
    lib.try_retrieve_function("nvmlDeviceGetMemoryInfo")
  );

  nvmlDeviceGetApplicationsClock = reinterpret_cast<nvmlDeviceGetApplicationsClock_t>(
    lib.try_retrieve_function("nvmlDeviceGetApplicationsClock")
  );

  nvmlDeviceSetApplicationsClocks = reinterpret_cast<nvmlDeviceSetApplicationsClocks_t>(
    lib.try_retrieve_function("nvmlDeviceSetApplicationsClocks")
  );

  nvmlDeviceResetApplicationsClocks = reinterpret_cast<nvmlDeviceResetApplicationsClocks_t>(
    lib.try_retrieve_function("nvmlDeviceResetApplicationsClocks")
  );

  nvmlDeviceSetDefaultAutoBoostedClocksEnabled = reinterpret_cast<nvmlDeviceSetDefaultAutoBoostedClocksEnabled_t>(
    lib.try_retrieve_function("nvmlDeviceSetDefaultAutoBoostedClocksEnabled")
  );

  nvmlDeviceGetAutoBoostedClocksEnabled = reinterpret_cast<nvmlDeviceGetAutoBoostedClocksEnabled_t>(
    lib.try_retrieve_function("nvmlDeviceGetAutoBoostedClocksEnabled")
  );

  nvmlDeviceGetSupportedMemoryClocks = reinterpret_cast<nvmlDeviceGetSupportedMemoryClocks_t>(
    lib.try_retrieve_function("nvmlDeviceGetSupportedMemoryClocks")
  );

  nvmlDeviceGetSupportedGraphicsClocks = reinterpret_cast<nvmlDeviceGetSupportedGraphicsClocks_t>(
    lib.try_retrieve_function("nvmlDeviceGetSupportedGraphicsClocks")
  );

  nvmlDeviceGetPowerManagementLimitConstraints = reinterpret_cast<nvmlDeviceGetPowerManagementLimitConstraints_t>(
    lib.try_retrieve_function("nvmlDeviceGetPowerManagementLimitConstraints")
  );
  
  nvmlDeviceGetPowerManagementDefaultLimit = reinterpret_cast<nvmlDeviceGetPowerManagementDefaultLimit_t>(
    lib.try_retrieve_function("nvmlDeviceGetPowerManagementDefaultLimit")
  );

  nvmlDeviceSetPowerManagementLimit = reinterpret_cast<nvmlDeviceSetPowerManagementLimit_t>(
    lib.try_retrieve_function("nvmlDeviceSetPowerManagementLimit")
  );  

  nvmlDeviceGetPowerManagementLimit = reinterpret_cast<nvmlDeviceGetPowerManagementLimit_t>(
    lib.try_retrieve_function("nvmlDeviceGetPowerManagementLimit")
  );  

  nvmlDeviceGetPowerManagementMode = reinterpret_cast<nvmlDeviceGetPowerManagementMode_t>(
    lib.try_retrieve_function("nvmlDeviceGetPowerManagementMode")
  );

  nvmlDeviceGetEnforcedPowerLimit = reinterpret_cast<nvmlDeviceGetEnforcedPowerLimit_t>(
    lib.try_retrieve_function("nvmlDeviceGetEnforcedPowerLimit")
  );
  
  nvmlDeviceGetPersistenceMode = reinterpret_cast<nvmlDeviceGetPersistenceMode_t>(
    lib.try_retrieve_function("nvmlDeviceGetPersistenceMode")
  );

  nvmlDeviceSetPersistenceMode = reinterpret_cast<nvmlDeviceSetPersistenceMode_t>(
    lib.try_retrieve_function("nvmlDeviceSetPersistenceMode")
  );
}


void
NVMLProxy::try_init()
const
{
  if (
    auto nv_status = nvmlInit();
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::Exception(
      "failed to init NVML",
      get_error_message_from_status(nv_status),
      nv_status
    );
  }
}


void NVMLProxy::try_shutdown()
const
{
  if (
    auto nv_status = nvmlShutdown();
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::Exception(
      "failed to shut down NVML",
      get_error_message_from_status(nv_status),
      nv_status
    );
  }
}


nvmlString_t
NVMLProxy::try_get_driver_version()
const
{
  char buffer[NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE];

  if (
    auto nv_status = nvmlSystemGetDriverVersion(
      buffer,
      NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE
    );
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::Exception(
      "failed to get driver version",
      get_error_message_from_status(nv_status),
      nv_status
    );
  }

  return nvmlString_t(buffer);
}


nvmlString_t
NVMLProxy::try_get_nvml_version()
const
{
  char buffer[NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE];

  if (
    auto nv_status = nvmlSystemGetNVMLVersion(
      buffer,
      NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE
    );
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::Exception(
      "failed to get NVML version",
      get_error_message_from_status(nv_status),
      nv_status
    );
  }

  return nvmlString_t(buffer);
}


unsigned int
NVMLProxy::try_get_devices_count()
const
{
  unsigned int device_count {};

  if (
    auto nv_status = nvmlDeviceGetCount(&device_count);
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::Exception(
      "failed to get devices count",
      get_error_message_from_status(nv_status),
      nv_status
    );
  }

  return device_count;
}


nvmlDevice_t
NVMLProxy::try_get_device_handle(const nvmlDeviceIndex_t index)
const
{
  nvmlDevice_t handle {};

  if (
    auto nv_status = nvmlDeviceGetHandleByIndex(index, &handle);
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to get device handle",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }

  return handle;
}


nvmlString_t
NVMLProxy::try_get_device_name(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  char buffer[NVML_DEVICE_NAME_BUFFER_SIZE];

  if (
    auto nv_status = nvmlDeviceGetName(
      handle,
      buffer,
      NVML_DEVICE_NAME_BUFFER_SIZE
    );
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to get device name",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }

  return nvmlString_t(buffer);
}


nvmlOptionalString_t
NVMLProxy::try_maybe_get_device_serial(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  char buffer[NVML_DEVICE_SERIAL_BUFFER_SIZE];

  switch (auto nv_status = nvmlDeviceGetSerial(
    handle,
    buffer,
    NVML_DEVICE_SERIAL_BUFFER_SIZE
  ))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return nvmlString_t(buffer);
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device serial",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlOptionalString_t
NVMLProxy::try_maybe_get_device_uuid(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  char buffer[NVML_DEVICE_UUID_BUFFER_SIZE];

  switch (auto nv_status = nvmlDeviceGetUUID(
      handle,
      buffer,
      NVML_DEVICE_UUID_BUFFER_SIZE
  ))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return nvmlString_t(buffer);
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device UUID",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlString_t
NVMLProxy::try_get_device_VBIOS_version(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  char buffer[NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE];

  if (
    auto nv_status = nvmlDeviceGetVbiosVersion(
      handle,
      buffer,
      NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE
    );
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to get device VBIOS version",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }

  return nvmlString_t(buffer);
}


nvmlOptionalFanSpeed_t
NVMLProxy::try_maybe_get_device_fan_speed(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlFanSpeed_t value {};

  switch (auto nv_status = get_device_fan_speed_raw(handle, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device fan speed",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}

nvmlReturn_t
NVMLProxy::get_device_fan_speed_raw(
  const nvmlDevice_t& handle,
  nvmlFanSpeed_t*     value
)
const
{
  return nvmlDeviceGetFanSpeed(handle, value);
}


nvmlOptionalTemperature_t
NVMLProxy::try_maybe_get_device_temperature(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlTemperature_t value {};

  switch (auto nv_status = get_device_temperature_raw(
    handle,
    &value
  ))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device temperature",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlReturn_t
NVMLProxy::get_device_temperature_raw(
  const nvmlDevice_t& handle,
  nvmlTemperature_t*  temp
)
const
{
  return nvmlDeviceGetTemperature(
    handle,
    nvmlTemperatureSensors_t::NVML_TEMPERATURE_GPU,
    temp
  );
}


nvmlOptionalPower_t
NVMLProxy::try_maybe_get_device_power_usage(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlPower_t value {};

  switch (auto nv_status = get_device_power_usage_raw(handle, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device power usage",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlReturn_t
NVMLProxy::get_device_power_usage_raw(
  const nvmlDevice_t& handle,
  nvmlPower_t*        value
)
const
{
  return nvmlDeviceGetPowerUsage(handle, value);
}


nvmlOptionalUtilization_t
NVMLProxy::try_maybe_get_device_utilization_rates(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlUtilization_t value {};

  switch (auto nv_status = get_device_utilization_rates_raw(handle, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device utilization rates",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlReturn_t
NVMLProxy::get_device_utilization_rates_raw(
  const nvmlDevice_t& handle,
  nvmlUtilization_t*  value
)
const
{
  return nvmlDeviceGetUtilizationRates(handle, value);
}


nvmlCudaComputeCapability_t
NVMLProxy::try_get_device_CUDA_compute_capability(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{  
  nvmlCudaComputeCapability_t value {};

  if (
    auto nv_status = nvmlDeviceGetCudaComputeCapability(
      handle,
      &(value.major),
      &(value.minor)
    );
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to get device CUDA compatibility",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }

  return value;
}


nvmlOptionalPowerManagementLimitConstraints_t
NVMLProxy::try_maybe_get_device_power_management_limit_constrains(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlPowerManagementLimitConstraints_t value {};

  switch (auto nv_status = nvmlDeviceGetPowerManagementLimitConstraints(
    handle,
    &(value.min_limit),
    &(value.max_limit)
  ))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device power management limit constrains",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlOptionalPower_t
NVMLProxy::try_maybe_get_device_power_management_default_limit(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlPower_t value {};

  switch (auto nv_status = nvmlDeviceGetPowerManagementDefaultLimit(handle, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device power management default limit",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


void
NVMLProxy::try_set_device_power_management_limit(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle,
  const nvmlPower_t       value
)
const
{
  if (
    auto nv_status = nvmlDeviceSetPowerManagementLimit(handle, value);
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to set device power management limit",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }
}


nvmlOptionalPower_t
NVMLProxy::try_maybe_get_device_power_management_limit(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlPower_t value {};

  switch (auto nv_status = nvmlDeviceGetPowerManagementLimit(handle, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device power management limit",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlOptionalEnableState_t
NVMLProxy::try_maybe_get_device_power_management_mode(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlEnableState_t value {};

  switch (auto nv_status = nvmlDeviceGetPowerManagementMode(handle, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device power management mode",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlOptionalPower_t
NVMLProxy::try_maybe_get_device_enforced_power_limit(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlPower_t value {};

  switch (auto nv_status = nvmlDeviceGetEnforcedPowerLimit(handle, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device enforced power limit",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


void
NVMLProxy::try_set_device_persistence_mode(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle,
  const nvmlEnableState_t value
)
const
{
  if (
    auto nv_status = nvmlDeviceSetPersistenceMode(handle, value);
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to set device persistence mode",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }
}


nvmlOptionalEnableState_t
NVMLProxy::try_maybe_get_device_persistence_mode(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlEnableState_t value {};

  switch (auto nv_status = nvmlDeviceGetPersistenceMode(handle, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device persistence mode",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlTemperatureThresholdsReport_t
NVMLProxy::try_get_device_temperature_thresholds(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlTemperatureThresholdsReport_t value {};

  value.shutdown = try_maybe_get_device_temperature_threshold(
    index,
    handle,
    nvmlTemperatureThresholds_t::NVML_TEMPERATURE_THRESHOLD_SHUTDOWN
  );

  value.slowdown = try_maybe_get_device_temperature_threshold(
    index,
    handle,
    nvmlTemperatureThresholds_t::NVML_TEMPERATURE_THRESHOLD_SLOWDOWN
  );

  value.mem_max = try_maybe_get_device_temperature_threshold(
    index,
    handle,
    nvmlTemperatureThresholds_t::NVML_TEMPERATURE_THRESHOLD_MEM_MAX
  );

  value.gpu_max = try_maybe_get_device_temperature_threshold(
    index,
    handle,
    nvmlTemperatureThresholds_t::NVML_TEMPERATURE_THRESHOLD_GPU_MAX
  );

  return value;
}


nvmlOptionalTemperature_t
NVMLProxy::try_maybe_get_device_temperature_threshold(
  const nvmlDeviceIndex_t     index,
  const nvmlDevice_t&         handle,
  nvmlTemperatureThresholds_t thresholdType
) const
{
  nvmlTemperature_t value {};

  switch (auto nv_status = nvmlDeviceGetTemperatureThreshold(
    handle,
    thresholdType,
    &value
  ))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        (
          "failed to get device temperature threshold of type " +
          std::to_string(static_cast<int>(thresholdType))
        ),
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlMemoryInfo_t
NVMLProxy::try_get_device_memory_info(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlMemoryInfo_t value {};

  if (
    auto nv_status = get_device_memory_info_raw(handle, &value);
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to get device memory info",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }

  return value;
}


nvmlReturn_t
NVMLProxy::get_device_memory_info_raw(
  const nvmlDevice_t& handle,
  nvmlMemoryInfo_t*   value
)
const
{
  return nvmlDeviceGetMemoryInfo(handle, value);
}


void
NVMLProxy::try_set_device_default_autoboosted_clocks_enabled(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle,
  const nvmlEnableState_t enabled
)
const
{
  if (
    auto nv_status = nvmlDeviceSetDefaultAutoBoostedClocksEnabled(
      /* device  = */ handle,
      /* enabled = */ enabled,
      /* flags   = */ 0
    );
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to set device default clocks autoboosting state",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }
}


nvmlOptionalAutoboostedClocksState_t
NVMLProxy::try_maybe_get_device_autoboosted_clocks_enabled(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlAutoboostedClocksState_t result;

  switch (auto nv_status = nvmlDeviceGetAutoBoostedClocksEnabled(
    handle,
    &(result.is_enabled),
    &(result.is_default_enabled)
  ))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return result;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device clocks autoboosting info",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlOptionalClockSpeedVector_t
NVMLProxy::try_maybe_get_device_supported_memory_clocks(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  unsigned int count {};

  switch (auto nv_status = nvmlDeviceGetSupportedMemoryClocks(
    handle,
    &count,
    nullptr
  ))
  {
    case nvmlReturn_t::NVML_ERROR_INSUFFICIENT_SIZE:
      break;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device number of supported memory clocks",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }

  nvmlClockSpeedVector_t clocks (count);

  switch (auto nv_status = nvmlDeviceGetSupportedMemoryClocks(
    handle,
    &count,
    clocks.data()
  ))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      std::sort(clocks.begin(), clocks.end(), std::greater<>());
      return clocks;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device supported memory clocks",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlOptionalClockSpeedVector_t
NVMLProxy::try_maybe_get_device_supported_graphics_clocks(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle,
  const nvmlClockSpeed_t  memoryClockMHz
)
const
{
  unsigned int count {};

  switch (auto nv_status = nvmlDeviceGetSupportedGraphicsClocks(
    handle,
    memoryClockMHz,
    &count,
    nullptr
  ))
  {
    case nvmlReturn_t::NVML_ERROR_INSUFFICIENT_SIZE:
      break;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device number of supported graphics clocks",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }

  nvmlClockSpeedVector_t clocks (count);

  switch (auto nv_status = nvmlDeviceGetSupportedGraphicsClocks(
    handle,
    memoryClockMHz,
    &count,
    clocks.data()
  ))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      std::sort(clocks.begin(), clocks.end(), std::greater<>());
      return clocks;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        "failed to get device supported graphics clocks",
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlOptionalClockSpeedsMap_t
NVMLProxy::try_maybe_get_device_supported_clock_speeds(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  const auto memory_clocks = try_maybe_get_device_supported_memory_clocks(
    index,
    handle
  );

  if (!memory_clocks.has_value())
  {
    return nvmlNotSupported;
  }

  nvmlClockSpeedsMap_t clocks_map;

  for (const auto clock: *memory_clocks)
  {
    auto graphics_clocks = try_maybe_get_device_supported_graphics_clocks(
      index,
      handle,
      clock
    );
    if (graphics_clocks.has_value())
    {
      clocks_map[clock] = *graphics_clocks;
    }    
  }

  if (clocks_map.size()) {
    return clocks_map;
  } else {
    return nvmlNotSupported;
  }
}


nvmlOptionalClockSpeed_t
NVMLProxy::try_maybe_get_device_applications_clock(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle,
  const nvmlClockType_t   clockType
)
const
{
  nvmlClockSpeed_t value {};

  switch (auto nv_status = nvmlDeviceGetApplicationsClock(handle, clockType, &value))
  {
    case nvmlReturn_t::NVML_SUCCESS:
      return value;
    case nvmlReturn_t::NVML_ERROR_NOT_SUPPORTED:
      return nvmlNotSupported;
    default:
      throw NVMLProxy::DeviceException(
        (
          "failed to get device applications clock of type " +
          std::to_string(static_cast<int>(clockType))
        ),
        get_error_message_from_status(nv_status),
        nv_status,
        index
      );
  }
}


nvmlOptionalClockSpeed_t
NVMLProxy::try_maybe_get_device_applications_memory_clock(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  return try_maybe_get_device_applications_clock(
    index,
    handle,
    nvmlClockType_t::NVML_CLOCK_MEM
  );
}


nvmlOptionalClockSpeed_t
NVMLProxy::try_maybe_get_device_applications_graphics_clock(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  return try_maybe_get_device_applications_clock(
    index,
    handle,
    nvmlClockType_t::NVML_CLOCK_GRAPHICS
  );
}


nvmlApplicationsClockSpeedsReport_t
NVMLProxy::try_get_device_applications_clocks(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  nvmlApplicationsClockSpeedsReport_t values;
  values.memory   = try_maybe_get_device_applications_memory_clock(  index, handle);
  values.graphics = try_maybe_get_device_applications_graphics_clock(index, handle);
  return values;
}


void
NVMLProxy::try_set_device_applications_clocks(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle,
  const nvmlClockSpeed_t  memClockMHz,
  const nvmlClockSpeed_t  graphicsClockMHz
)
const
{
  if (
    auto nv_status = nvmlDeviceSetApplicationsClocks(
      handle,
      memClockMHz,
      graphicsClockMHz
    );
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to set device applications clocks",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }
}


void
NVMLProxy::try_reset_device_applications_clocks(
  const nvmlDeviceIndex_t index,
  const nvmlDevice_t&     handle
)
const
{
  if (
    auto nv_status = nvmlDeviceResetApplicationsClocks(handle);
    nv_status != nvmlReturn_t::NVML_SUCCESS
  ) {
    throw NVMLProxy::DeviceException(
      "failed to reset device applications clocks",
      get_error_message_from_status(nv_status),
      nv_status,
      index
    );
  }
}