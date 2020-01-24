// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyHardwareBPLibrary.h"
#include "MyHardware.h"

UMyHardwareBPLibrary::UMyHardwareBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FHardwareDetailsStruct UMyHardwareBPLibrary::GetCurrentHardware()
{
	FHardwareDetailsStruct CurrentHardware;
	CurrentHardware.CPUBrand = *FWindowsPlatformMisc::GetCPUBrand();
	CurrentHardware.CPUVendor = *FWindowsPlatformMisc::GetCPUVendor();
	CurrentHardware.NumberOfCoresIncludingHyperthread = FWindowsPlatformMisc::NumberOfCoresIncludingHyperthreads();
	CurrentHardware.NumberOfCores = FWindowsPlatformMisc::NumberOfCores();
	CurrentHardware.Is64BitOS = FWindowsPlatformMisc::Is64bitOperatingSystem();
	CurrentHardware.PrimaryGPUBrand = FWindowsPlatformMisc::GetPrimaryGPUBrand();
	CurrentHardware.MacAddress = FGenericPlatformMisc::GetMacAddressString();
	CurrentHardware.BatteryLevel = FWindowsPlatformMisc::GetBatteryLevel();
	CurrentHardware.OSVersion = FWindowsPlatformMisc::GetOSVersion();
	CurrentHardware.DeviceTemperature = FGenericPlatformMisc::GetDeviceTemperatureLevel();
	CurrentHardware.EpicAccountID = FGenericPlatformMisc::GetEpicAccountId();
	return CurrentHardware;
}

