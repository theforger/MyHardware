// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyHardwareBPLibrary.h"
#include "GameFramework/GameSession.h"
#include "SocketSubsystem.h"
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif
#include "SlateBasics.h"
#include "Input/Events.h"
#include "CoreGlobals.h"
#include "Engine.h"
#include "MyHardware.h"

/*#define GLog GetGlobalLogSingleton();*/

UMyHardwareBPLibrary::UMyHardwareBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FHardwareDetailsStruct UMyHardwareBPLibrary::GetCurrentHardware()
{
	FHardwareDetailsStruct CurrentHardware;
#if PLATFORM_WINDOWS

	CurrentHardware.CPUBrand = *FWindowsPlatformMisc::GetCPUBrand();
	CurrentHardware.CPUVendor = *FWindowsPlatformMisc::GetCPUVendor();
	CurrentHardware.NumberOfCoresIncludingHyperthread = FWindowsPlatformMisc::NumberOfCoresIncludingHyperthreads();
	CurrentHardware.NumberOfCores = FWindowsPlatformMisc::NumberOfCores();
	CurrentHardware.Is64BitOS = FWindowsPlatformMisc::Is64bitOperatingSystem();
	CurrentHardware.PrimaryGPUBrand = FWindowsPlatformMisc::GetPrimaryGPUBrand();
	CurrentHardware.BatteryLevel = FWindowsPlatformMisc::GetBatteryLevel();
	CurrentHardware.OSVersion = FWindowsPlatformMisc::GetOSVersion();
#endif // PLATFORM_WINDOWS

	CurrentHardware.MacAddress = FGenericPlatformMisc::GetMacAddressString();
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

bool UMyHardwareBPLibrary::IsMouseConnected()
{
	auto MouseDetectApplication = FSlateApplication::Get().GetPlatformApplication();
	if (MouseDetectApplication.Get() != nullptr && MouseDetectApplication->IsMouseAttached())
	{
		return true;
	}
	return false;
}

bool UMyHardwareBPLibrary::IsCapsLockActive()
{
	auto CapslockDetectApplication = FSlateApplication::Get().GetPlatformApplication();
	if (CapslockDetectApplication.Get() != nullptr && CapslockDetectApplication->GetModifierKeys().AreCapsLocked())
	{
		return true;
	}
	return false;
}

void UMyHardwareBPLibrary::ChangeVolume(float Volume)
{
#if PLATFORM_WINDOWS
	HRESULT hr = NULL;
	float newVolume = Volume;

	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	float currentVolume = 0;
	endpointVolume->GetMasterVolumeLevel(&currentVolume);
	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
	hr = endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
	endpointVolume->Release();

	CoUninitialize();
#endif // PLATFORM_WINDOWS
}

void UMyHardwareBPLibrary::MoveScreen(FVector2D Pos)
{
	GEngine->GameViewport->GetWindow().Get()->MoveWindowTo(Pos);
}

