#ifndef _NVML_TYPES_H
#define _NVML_TYPES_H


#include <map>
#include <optional>
#include <string>
#include <vector>


constexpr unsigned int NVML_DEVICE_NAME_BUFFER_SIZE           {64};
constexpr unsigned int NVML_DEVICE_SERIAL_BUFFER_SIZE         {30};
constexpr unsigned int NVML_DEVICE_UUID_BUFFER_SIZE           {80};
constexpr unsigned int NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE  {32};

constexpr unsigned int NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE {80};
constexpr unsigned int NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE   {80};


enum class nvmlReturn_t
{
  NVML_SUCCESS                       = 0,
  NVML_ERROR_UNINITIALIZED           = 1,
  NVML_ERROR_INVALID_ARGUMENT        = 2,
  NVML_ERROR_NOT_SUPPORTED           = 3,
  NVML_ERROR_NO_PERMISSION           = 4,
  NVML_ERROR_ALREADY_INITIALIZED     = 5,
  NVML_ERROR_NOT_FOUND               = 6,
  NVML_ERROR_INSUFFICIENT_SIZE       = 7,
  NVML_ERROR_INSUFFICIENT_POWER      = 8,
  NVML_ERROR_DRIVER_NOT_LOADED       = 9,
  NVML_ERROR_TIMEOUT                 = 10,
  NVML_ERROR_IRQ_ISSUE               = 11,
  NVML_ERROR_LIBRARY_NOT_FOUND       = 12,
  NVML_ERROR_FUNCTION_NOT_FOUND      = 13,
  NVML_ERROR_CORRUPTED_INFOROM       = 14,
  NVML_ERROR_GPU_IS_LOST             = 15,
  NVML_ERROR_RESET_REQUIRED          = 16,
  NVML_ERROR_OPERATING_SYSTEM        = 17,
  NVML_ERROR_LIB_RM_VERSION_MISMATCH = 18,
  NVML_ERROR_IN_USE                  = 19,
  NVML_ERROR_MEMORY                  = 20,
  NVML_ERROR_NO_DATA                 = 21,
  NVML_ERROR_VGPU_ECC_NOT_SUPPORTED  = 22,
  NVML_ERROR_UNKNOWN                 = 999,
};


enum class nvmlEnableState_t
{
  NVML_FEATURE_DISABLED = 0,
  NVML_FEATURE_ENABLED  = 1
};

using nvmlOptionalEnableState_t = std::optional<nvmlEnableState_t>;


enum class nvmlTemperatureSensors_t
{
  NVML_TEMPERATURE_GPU = 0,
  NVML_TEMPERATURE_COUNT,
};


enum class nvmlTemperatureThresholds_t
{
  NVML_TEMPERATURE_THRESHOLD_SHUTDOWN = 0,
  NVML_TEMPERATURE_THRESHOLD_SLOWDOWN = 1,
  NVML_TEMPERATURE_THRESHOLD_MEM_MAX  = 2,
  NVML_TEMPERATURE_THRESHOLD_GPU_MAX  = 3,
  NVML_TEMPERATURE_THRESHOLD_COUNT
};


enum class nvmlClockType_t
{
  NVML_CLOCK_GRAPHICS = 0,
  NVML_CLOCK_SM       = 1,
  NVML_CLOCK_MEM      = 2,
  NVML_CLOCK_VIDEO    = 3,
  NVML_CLOCK_COUNT,
};


constexpr auto nvmlNotSupported = std::nullopt;

using nvmlDevice_t              = struct nvmlDevice_st*;
using nvmlDeviceIndex_t         = unsigned int;
using nvmlMemory_t              = unsigned long long;
using nvmlPercentage_t          = unsigned int;

using nvmlString_t              = std::string;
using nvmlOptionalString_t      = std::optional<nvmlString_t>;

using nvmlFanSpeed_t            = unsigned int;
using nvmlOptionalFanSpeed_t    = std::optional<nvmlFanSpeed_t>;

using nvmlPower_t               = unsigned int;
using nvmlOptionalPower_t       = std::optional<nvmlPower_t>;

using nvmlTemperature_t         = unsigned int;
using nvmlOptionalTemperature_t = std::optional<nvmlTemperature_t>;

using nvmlClockSpeed_t          = unsigned int;
using nvmlOptionalClockSpeed_t  = std::optional<nvmlClockSpeed_t>;


using nvmlTemperatureThresholdsReport_t = struct nvmlTemperatureThresholdsInfo_st
{
  nvmlOptionalTemperature_t shutdown;
  nvmlOptionalTemperature_t slowdown;
  nvmlOptionalTemperature_t mem_max;
  nvmlOptionalTemperature_t gpu_max;
};


using nvmlUtilization_t = struct nvmlUtilization_st
{
  nvmlPercentage_t gpu;
  nvmlPercentage_t memory;
};

using nvmlOptionalUtilization_t = std::optional<nvmlUtilization_t>;


using nvmlCudaComputeCapability_t = struct nvmlCudaComputeCapability_st
{
  int major;
  int minor;
};


using nvmlMemoryInfo_t = struct nvmlMemoryInfo_st
{
  nvmlMemory_t total; // warning: official docs have these 2 fields swapped
  nvmlMemory_t free;  //          and official version is wrong!
  nvmlMemory_t used;
};


using nvmlAutoboostedClocksState_t = struct nvmlAutoboostedClocksState_st {
  nvmlEnableState_t is_enabled;
  nvmlEnableState_t is_default_enabled;
};

using nvmlOptionalAutoboostedClocksState_t = std::optional<nvmlAutoboostedClocksState_t>;


using nvmlApplicationsClockSpeedsReport_t = struct nvmlApplicationsClockSpeeds_st
{
  nvmlOptionalClockSpeed_t memory;
  nvmlOptionalClockSpeed_t graphics;
};


using nvmlClockSpeedVector_t         = std::vector<nvmlClockSpeed_t>;
using nvmlOptionalClockSpeedVector_t = std::optional<nvmlClockSpeedVector_t>;

using nvmlClockSpeedsMap_t           = std::map<nvmlClockSpeed_t, nvmlClockSpeedVector_t, std::greater<nvmlClockSpeed_t>>;
using nvmlOptionalClockSpeedsMap_t   = std::optional<nvmlClockSpeedsMap_t>;


using nvmlPowerManagementLimitConstraints_t = struct nvmlPowerManagementLimitConstraints_st
{
  nvmlPower_t min_limit;
  nvmlPower_t max_limit;
};

using nvmlOptionalPowerManagementLimitConstraints_t = std::optional<nvmlPowerManagementLimitConstraints_t>;


using nvmlInit_t                      = nvmlReturn_t (*)(void);
using nvmlShutdown_t                  = nvmlReturn_t (*)(void);
using nvmlErrorString_t               = const char*  (*)(nvmlReturn_t result);

using nvmlSystemGetDriverVersion_t    = nvmlReturn_t (*)(char* version, unsigned int length);
using nvmlSystemGetNVMLVersion_t      = nvmlReturn_t (*)(char* version, unsigned int length);

using nvmlDeviceGetCount_t            = nvmlReturn_t (*)(unsigned int* deviceCount);
using nvmlDeviceGetHandleByIndex_t    = nvmlReturn_t (*)(nvmlDeviceIndex_t index, nvmlDevice_t* device);

using nvmlDeviceGetName_t             = nvmlReturn_t (*)(nvmlDevice_t device, char* name,    unsigned int length);
using nvmlDeviceGetSerial_t           = nvmlReturn_t (*)(nvmlDevice_t device, char* serial,  unsigned int length);
using nvmlDeviceGetUUID_t             = nvmlReturn_t (*)(nvmlDevice_t device, char* uuid,    unsigned int length);
using nvmlDeviceGetVbiosVersion_t     = nvmlReturn_t (*)(nvmlDevice_t device, char* version, unsigned int length);

using nvmlDeviceGetFanSpeed_t         = nvmlReturn_t (*)(nvmlDevice_t device, nvmlFanSpeed_t*    speed);
using nvmlDeviceGetUtilizationRates_t = nvmlReturn_t (*)(nvmlDevice_t device, nvmlUtilization_t* utilization);
using nvmlDeviceGetMemoryInfo_t       = nvmlReturn_t (*)(nvmlDevice_t device, nvmlMemoryInfo_t*  memory);

using nvmlDeviceGetCudaComputeCapability_t = nvmlReturn_t (*)(nvmlDevice_t device, int* major, int* minor);

using nvmlDeviceGetPowerManagementMode_t             = nvmlReturn_t (*)(nvmlDevice_t device, nvmlEnableState_t* mode);
using nvmlDeviceGetPowerUsage_t                      = nvmlReturn_t (*)(nvmlDevice_t device, nvmlPower_t* power);
using nvmlDeviceGetPowerManagementLimitConstraints_t = nvmlReturn_t (*)(nvmlDevice_t device, nvmlPower_t* minLimit, nvmlPower_t* maxLimit);
using nvmlDeviceGetPowerManagementDefaultLimit_t     = nvmlReturn_t (*)(nvmlDevice_t device, nvmlPower_t* defaultLimit);
using nvmlDeviceGetPowerManagementLimit_t            = nvmlReturn_t (*)(nvmlDevice_t device, nvmlPower_t* limit);
using nvmlDeviceSetPowerManagementLimit_t            = nvmlReturn_t (*)(nvmlDevice_t device, nvmlPower_t  limit);
using nvmlDeviceGetEnforcedPowerLimit_t              = nvmlReturn_t (*)(nvmlDevice_t device, nvmlPower_t* limit);

using nvmlDeviceGetPersistenceMode_t = nvmlReturn_t (*)(nvmlDevice_t device, nvmlEnableState_t* mode);
using nvmlDeviceSetPersistenceMode_t = nvmlReturn_t (*)(nvmlDevice_t device, nvmlEnableState_t  mode);


using nvmlDeviceGetTemperature_t = nvmlReturn_t (*)(
  nvmlDevice_t             device,
  nvmlTemperatureSensors_t sensorType,
  nvmlTemperature_t*       temp
);


using nvmlDeviceGetTemperatureThreshold_t = nvmlReturn_t (*)(
  nvmlDevice_t                device,
  nvmlTemperatureThresholds_t thresholdType,
  nvmlTemperature_t*          temp
);


using nvmlDeviceGetApplicationsClock_t = nvmlReturn_t (*)(
  nvmlDevice_t      device,
  nvmlClockType_t   clockType,
  nvmlClockSpeed_t* clockMHz
);


using nvmlDeviceSetApplicationsClocks_t = nvmlReturn_t (*)(
  nvmlDevice_t     device,
  nvmlClockSpeed_t memClockMHz,
  nvmlClockSpeed_t graphicsClockMHz
);


using nvmlDeviceResetApplicationsClocks_t = nvmlReturn_t (*)(nvmlDevice_t device);


using nvmlDeviceGetSupportedMemoryClocks_t = nvmlReturn_t (*)(
  nvmlDevice_t      device,
  unsigned int*     count,
  nvmlClockSpeed_t* clocksMHz
);


using nvmlDeviceGetSupportedGraphicsClocks_t = nvmlReturn_t (*)(
  nvmlDevice_t      device,
  nvmlClockSpeed_t  memoryClockMHz,
  unsigned int*     count,
  nvmlClockSpeed_t* clocksMHz
);


using nvmlDeviceSetDefaultAutoBoostedClocksEnabled_t = nvmlReturn_t (*)(
  nvmlDevice_t       device,
  nvmlEnableState_t  enabled,
  unsigned int       flags    /* not used */
);


using nvmlDeviceGetAutoBoostedClocksEnabled_t = nvmlReturn_t (*)(
  nvmlDevice_t       device,
  nvmlEnableState_t* isEnabled,
  nvmlEnableState_t* defaultIsEnabled
);


#endif // _NVML_TYPES_H