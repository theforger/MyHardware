// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyHardwareBPLibrary.h"
#include "GameFramework/GameSession.h"
#include "SocketSubsystem.h"
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <wtypes.h>
#include <WinUser.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif
#include "SlateBasics.h"
#include "Misc/CommandLine.h"
#include "Framework/Application/SlateApplication.h"
#include "Runtime/Launch/Resources/Windows/resource.h"
#include "Input/Events.h"
#include "CoreGlobals.h"
#include "Engine.h"
#include "MyHardware.h"

#define WM_ICON_NOTIFY WM_USER + 1009
#define WM_ICON_ID 32514
#define IDR_PAUSE 12
#define IDR_START 13

NOTIFYICONDATA m_nfData;

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

bool UMyHardwareBPLibrary::CreateIcon(FString IconPath, FString Tip, FString Content, FString ContentTitle)
{
#if PLATFORM_WINDOWS
	HINSTANCE hInst = NULL;
	HWND hWnd = GetActiveWindow();
	m_nfData.hWnd = hWnd;
	HICON hIcon = (HICON)LoadImage(hInst, *(IconPath), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	m_nfData.cbSize = sizeof(NOTIFYICONDATA);
	m_nfData.uID = IDICON_UE4Game;
	m_nfData.hIcon = hIcon;
	m_nfData.hWnd = hWnd;
	m_nfData.uCallbackMessage = WM_ICON_NOTIFY;
	m_nfData.uVersion = NOTIFYICON_VERSION;
	m_nfData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	
	LPCWSTR tipStr = *Tip;
	lstrcpy(m_nfData.szTip, tipStr);

	m_nfData.dwInfoFlags = NIIF_INFO;
	m_nfData.uTimeout = 1000;

	LPCWSTR contentStr = *Content;
	LPCWSTR infoTitleStr = *ContentTitle;
	lstrcpy(m_nfData.szInfo, contentStr);
	lstrcpy(m_nfData.szInfoTitle, infoTitleStr);

	return Shell_NotifyIcon(NIM_ADD, &m_nfData);
#endif // PLATFORM_WINDOWS
}

void UMyHardwareBPLibrary::MoveToMonitor(int32 MonitorNumber)
{
	if (GEngine && GEngine->GameViewport) {

		FParse::Value(FCommandLine::Get(), L"monitor=", MonitorNumber);


		FDisplayMetrics Display;
		FDisplayMetrics::RebuildDisplayMetrics(Display);


		int8 MonitorIndex = MonitorNumber - 1;
		int32 CurrentMonitorWidth = Display.MonitorInfo[MonitorIndex].NativeWidth;


		float WidthPosition = (MonitorIndex)*Display.PrimaryDisplayWidth - CurrentMonitorWidth;


		float HeightPosition = 0.0f;


		FVector2D WindowPosition = FVector2D((-1)*WidthPosition, HeightPosition);
		GEngine->GameViewport->GetWindow()->MoveWindowTo(WindowPosition);
	}
}

