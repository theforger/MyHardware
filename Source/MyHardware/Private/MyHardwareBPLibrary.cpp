// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyHardwareBPLibrary.h"
#include "GameFramework/GameSession.h"
#include "SocketSubsystem.h"
#include "SlateBasics.h"
#include "CoreGlobals.h"
#include "MyHardware.h"

/*#define GLog GetGlobalLogSingleton();*/

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

const FString UMyHardwareBPLibrary::GetLocalIP(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			bool canBind = false;
			TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
			return (localIp->IsValid() ? localIp->ToString(false) : FString(TEXT("Invalid World Context")));
			//return World->URL.Host;
		}
	}
	return "WorldContextObject is FALSE";
}

bool UMyHardwareBPLibrary::IsGamepadConnected()
{
	auto GamepadDetectApplication = FSlateApplication::Get().GetPlatformApplication();
	if (GamepadDetectApplication.Get() != nullptr && GamepadDetectApplication->IsGamepadAttached())
	{
		return true;
	}
	return false;
}

